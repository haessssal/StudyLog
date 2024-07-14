#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>  // 시스템 콜 관련 헤더 파일
#include <arpa/inet.h>  // 인터넷 주소 변환 관련 헤더 파일

// 시스템 콜 번호
#define SYS_mychat 449

int main(int argc, char *argv[]) {
		// 명령줄 인자의 수가 4개가 아닌 경우 사용법 출력 및 종료
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <IP> <Message> <Port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *ip = argv[1];  // 첫 번째 인자: IP 주소로 설정
    char *msg = argv[2];  // 두 번째 인자: 메세지로 설정
    int port = atoi(argv[3]);  // 세 번째 인자: 포트 번호로 설정

    // 시스템 콜 호출
    long res = syscall(SYS_mychat, ip, msg);
    if (res < 0) {
		    // 실패 시 오류 메세지 출력 및 종료
        perror("syscall failed");
        exit(EXIT_FAILURE);
    }

		// 성공 메세지 출력 및 종료
    printf("Message sent to %s:%d\n", ip, port);
    return 0;
}
