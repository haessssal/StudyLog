#include <stdlib.h>
#include <stdio.h>
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

void err_exit(const char* err) {
	perror(err);  // err에 대응하는 에러 메세지 출력
	exit(1);  // 프로그램 종료
}

// 파일을 전송하는 함수
void send_file(int serv_sock, const struct sockaddr_in* serv_addr) {
	char buffer[BUFF_SIZE];  // 데이터 저장할 버퍼
	char file_path[BUFF_SIZE];  // 전송할 파일의 경로 저장
	ssize_t recv_size;  // 읽은 데이터의 크기 저장
	FILE* file_ptr;  // 파일 포인터

	// 파일 경로 입력받기
	printf("Enter the file path: ");
	fgets(file_path, BUFF_SIZE, stdin);
	file_path[strlen(file_path) - 1] = '\0';  // 마지막 개행 문자를 NULL로 변경

	// 파일 읽기 모드로 열기
	file_ptr = fopen(file_path, "rb");
	// 실패한 경우 에러 처리
	if (file_ptr == NULL) {
		err_exit("File open error\n");
	}

	// 파일 내용을 읽어 버퍼에 저장한 뒤 전송
	while (1) {
		recv_size = fread(buffer, 1, BUFF_SIZE, file_ptr);  // 데이터 읽기(fread 함수는 읽어들인 바이트 수를 반환)
		if (recv_size == 0) break;  // 파일의 끝에 도달하면(읽어들인 데이터 크기가 0이라면) 루프 종료
		// 파일 내용을 서버에 전송
		if (sendto(serv_sock, buffer, recv_size, 0, (struct sockaddr*)serv_addr, sizeof(struct sockaddr)) < 0) {
			err_exit("File send error\n");
		}
	}

	// 파일 닫기
	fclose(file_ptr);
}

// 메시지를 전송하는 함수
void send_message(int serv_sock, const struct sockaddr_in* serv_addr) {
	char buffer[BUFF_SIZE];  // 데이터 저장할 버퍼

	// 메세지 입력받기
	printf("Enter your message: ");
	fgets(buffer, BUFF_SIZE, stdin);
	buffer[strlen(buffer) - 1] = '\0';

	// 입력한 메세지를 서버에 전송
	if (sendto(serv_sock, buffer, strlen(buffer) + 1, 0, (struct sockaddr*)serv_addr, sizeof(struct sockaddr)) < 0) {
		err_exit("Send error\n");
	}

	// 서버로부터 응답 받기
	ssize_t size = recvfrom(serv_sock, buffer, BUFF_SIZE, 0, NULL, NULL);
	// 실패한 경우 에러 처리
	if (size < 0) {
		err_exit("Receive error\n");
	}

	buffer[size] = '\0';  // 데이터의 마지막에 NULL 추가
	printf("%s\n", buffer);  // 응답받은 메세지 출력
}

int main(int argc, char* argv[]) {
	int serv_sock;  // 소켓 파일 디스크립터
	struct sockaddr_in serv_addr;  // server 주소 구조체
	struct sockaddr_in clnt_addr;  // client 주소 구조체
	socklen_t size;  // 주소 구조체 크기
	char buffer[BUFF_SIZE];  // 데이터 저장할 버퍼
	ssize_t recv_size;  // 수신한 데이터 크기
	FILE* file_ptr;  // 파일 포인터
	char choice[BUFF_SIZE];
	char username[BUFF_SIZE];
	char password[BUFF_SIZE];
	int login = 0;

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

	// 로그인 루프
	while (!login) {
		// username, password 초기화
		memset(username, 0, BUFF_SIZE);
		memset(password, 0, BUFF_SIZE);

		// 입력받기 위한 메세지 출력
		printf("Enter username: ");
		fgets(username, BUFF_SIZE, stdin);
		username[strcspn(username, "\n")] = '\0';  // 개행 문자를 NULL 문자로 대체

		printf("Enter password: ");
		fgets(password, BUFF_SIZE, stdin);
		password[strcspn(password, "\n")] = '\0';

		// 입력한 username, password를 서버로 전송
		// 실패한 경우 에러 처리
		if (sendto(serv_sock, username, strlen(username) + 1, 0, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr)) < 0) {
			err_exit("Send error\n");
		}

		if (sendto(serv_sock, password, strlen(password) + 1, 0, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr)) < 0) {
			err_exit("Send error\n");
		}

		// 서버로부터 응답 받기
		size = sizeof(struct sockaddr);
		recv_size = recvfrom(serv_sock, buffer, BUFF_SIZE, 0, (struct sockaddr*)&clnt_addr, &size);
		// 실패한 경우 에러 처리
		if (recv_size < 0) {
			err_exit("Recieve error\n");
		}

		// 로그인 성공 여부 확인
		if (strcmp(buffer, "Login success") == 0) {
			printf("Login success\n");
			login = 1;  // 성공한 경우 상태를 1로 변경
		}
		// 실패한 경우 에러 처리
		else {
			printf("Login failed. Please try again.\n");
		}
	}

	// 파일 혹은 메세지 전송 루프
	while (1) {
		// 사용자에게 선택 입력받기
		printf("Send [file / message / quit]: ");
		fgets(choice, BUFF_SIZE, stdin);
		choice[strlen(choice) - 1] = '\0';  // 개행 문자를 NULL 문자로 대체

		while (1) {
			// 종료를 선택한 경우
			if (strcmp(choice, "quit") == 0) {
				// 프로그램 종료를 서버에게 알림
				if (sendto(serv_sock, choice, strlen(choice) + 1, 0, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr)) < 0) {
					err_exit("Send error\n");
				}
				break;  // 내부 루프 종료
			}

			// 파일 전송을 선택한 경우
			if (strcmp(choice, "file") == 0) {
				send_file(serv_sock, &serv_addr);
				break;
			}

			// 메세지 전송을 선택한 경우
			if (strcmp(choice, "message") == 0) {
				send_message(serv_sock, &serv_addr);
				break;
			}
		}

		if (strcmp(choice, "quit") == 0) break;  // 외부 루프 종료
	}

	// 소켓 닫기
	close(serv_sock);
	return 0;
}
