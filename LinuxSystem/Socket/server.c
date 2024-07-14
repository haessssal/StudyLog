#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>

#define PORT 9999  // 포트 번호 정의
#define BUFF_SIZE 1024  // 데이터를 저장할 버퍼의 크기

//username, password를 저장한 파일
#define USER_FILE "/home/sunwoo/linux_ex/users.txt"  

void err_exit(const char* err) {
    perror(err);  // err에 대응하는 에러 메세지 출력
    exit(1);  // 프로그램 종료
}

// 사용자 구조체
typedef struct {
    char username[50];
    char password[50];
} User;

User users[100];  // 구조체의 배열 선언
int n = 0;  // 현재 등록된 사용자 수

// 사용자 정보를 파일에서 읽어와 User 구조체에 저장
void load_users() {
    // 파일을 읽기 모드로 열기
    FILE* file = fopen(USER_FILE, "r");
    if (file != NULL) {
        // 파일에서 사용자 정보를 읽어와 users 배열에 저장: "이름 비번" 꼴
        // EOF에 도달할 때까지 반복
        while (fscanf(file, "%s %s", users[n].username, users[n].password) != EOF) {
            n++;
        }

        // 파일 닫기
        fclose(file);
    }
}

// 수신받은 username, password를 저장한 정보와 비교해 인증
int authenticate(char* username, char* password) {
    // 등록된 사용자 수만큼 반복
    for (int i = 0; i < n; i++) {
        // username, password가 모두 일치하는지 확인
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return 1;  // 인증 성공
        }
    }

    return 0;  // 인증 실패
}

// 파일을 수신하는 함수
void send_file(int serv_sock, const struct sockaddr_in* clnt_addr, const char* filename) {
    char buffer[BUFF_SIZE];  // 데이터 저장할 버퍼
    ssize_t recv_size;  // 수신한 데이터 크기
    FILE* file_ptr;  // 파일 포인터

    // 파일 읽기 모드로 열기
    file_ptr = fopen(filename, "rb");
    // 실패한 경우 에러 처리
    if (file_ptr == NULL) {
        err_exit("File open error\n");
    }

    // client 에게 응답
    while (1) {
        recv_size = fread(buffer, 1, BUFF_SIZE, file_ptr);
        if (recv_size == 0) break;
        if (sendto(serv_sock, buffer, recv_size, 0, (struct sockaddr*)clnt_addr, sizeof(struct sockaddr)) < 0) {
            err_exit("File send error\n");
        }
    }

    // 파일 닫기
    fclose(file_ptr);
}

int main() {
    int serv_sock;  // 소켓 파일 디스크립터
    struct sockaddr_in serv_addr;  // server 주소 구조체
    struct sockaddr_in clnt_addr;  // client 주소 구조체
    socklen_t size;  // 주소 구조체 크기
    char buffer[BUFF_SIZE];  // 데이터 저장할 버퍼
    ssize_t recv_size;  // 수신한 데이터 크기
    FILE* file_ptr;  // 파일 포인터
    char username[BUFF_SIZE];
    char password[BUFF_SIZE];

    load_users();

    // UDP 소켓 생성
    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    // 실패한 경우 에러 처리
    if (serv_sock < 0) {
        err_exit("Socket error\n");
    }

    // 서버 구조체 초기화
    memset(&serv_addr, 0, sizeof(serv_addr));
    // 소켓 주소의 주소 체계를 지정하는 구조체 멤버 sin_family 설정(AF_INET: IPv4 주소 체계를 사용함을 의미)
    serv_addr.sin_family = AF_INET;
    // 포트 번호를 지정하는 구조체 멤버 sin_port 설정(htons 함수: PORT 상수를 네트워크 바이트 순서로 설정)
    serv_addr.sin_port = htons(PORT);
    // IPv4 주소를 지정하는 구조체 멤버 sin_addr 설정(INADDR_ANY: 서버가 어떤 네트워크 인터페이스에서든 클라이언트의 연결 요청을 수락함을 의미)
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // 소켓을 서버 주소에 바인드
    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr)) < 0) {
        err_exit("Bind error\n");
    }

    // client 주소 구조체 초기화
    memset(&clnt_addr, 0, sizeof(struct sockaddr));

    // 메인 루프
    while (1) {
        size = sizeof(struct sockaddr);

        // client로부터 username, password 수신
        recvfrom(serv_sock, username, BUFF_SIZE, 0, (struct sockaddr*)&clnt_addr, &size);
        recvfrom(serv_sock, password, BUFF_SIZE, 0, (struct sockaddr*)&clnt_addr, &size);

        // 사용자 인증
        if (authenticate(username, password)) {
            // 로그인에 성공한 경우 성공 메세지 전송
            sendto(serv_sock, "Login success", strlen("Login success") + 1, 0, (struct sockaddr*)&clnt_addr, sizeof(struct sockaddr));

            while (1) {
                size = sizeof(struct sockaddr);

                // 데이터 수신받기
                recv_size = recvfrom(serv_sock, buffer, BUFF_SIZE, 0, (struct sockaddr*)&clnt_addr, &size);
                if (recv_size < 0) {
                    err_exit("Receive error\n");
                }

                // 종료를 선택한 경우
                if (strcmp(buffer, "quit") == 0) {
                    close(serv_sock);
                    exit(0);
                }

                // client가 파일을 선택한 경우
                if (access(buffer, F_OK) == 0) {
                    // 해당 파일을 전송
                    send_file(serv_sock, &clnt_addr, buffer);
                }
                // client가 메시지를 보내는 경우
                else {
                    // 해당 메시지를 다시 client에게 전송
                    if (sendto(serv_sock, buffer, recv_size, 0, (struct sockaddr*)&clnt_addr, sizeof(struct sockaddr)) < 0) {
                        err_exit("Message send error\n");
                    }
                }

                // 수신한 데이터 출력
                printf("Received from client: %s\n", buffer);
            }
        }

        // 로그인에 실패한 경우
        else {
            sendto(serv_sock, "Login failed", strlen("Login failed") + 1, 0, (struct sockaddr*)&clnt_addr, sizeof(struct sockaddr));
        }
    }

    // 소켓 닫기
    close(serv_sock);
    return 0;
}
