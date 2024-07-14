#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

// 포트 번호 정
#define PORT 12345

int main() {
    int sockfd;  // 소켓 파일 디스크립터 선언
    struct sockaddr_in server_addr, client_addr;  // 서버와 클라이언트 주소 구조체 선언
    char buffer[128];  // 메세지 저장할 버퍼 선언
    socklen_t addr_len = sizeof(client_addr);  // 클라이언트 주소 길이 변수 선언

    // UDP 소켓 생성
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		    // 실패 시 오류 메세지 출력 및 종
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 서버 주소 설정
    memset(&server_addr, 0, sizeof(server_addr));  // 서버 주소 구조체 초기화
    server_addr.sin_family = AF_INET;  // 주소 패밀리 설정
    server_addr.sin_addr.s_addr = INADDR_ANY;  // 모든 인터페이스에서 수신하도록 설정
    server_addr.sin_port = htons(PORT);  // 포트 번호 설정

    // 소켓 바인딩
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
	       // 실패 시 오류 메세지 출력, 소켓 기 및 종료
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

		// 서버가 수신 대기중임을 출력
    printf("Server listening on port %d\n", PORT);

    // 메시지 수신 대기
    while (1) {
        int len = recvfrom(sockfd, (char *)buffer, 128, 0, (struct sockaddr *)&client_addr, &addr_len);
        buffer[len] = '\0';  // 수신된 메세지의 끝에 NULL 문자 추가
        printf("Received message: %s\n", buffer);  // 수신된 메세지 출력
    }

		// 소켓 닫기 및 종료
    close(sockfd);
    return 0;
}
