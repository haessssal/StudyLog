#include <linux/kernel.h>  // 커널 관련 헤더 파일
#include <linux/syscalls.h>  // 시스템 호출 관련 헤더 파일
#include <linux/uaccess.h>  // 유저레벨 접근 관련 헤더 파일
#include <linux/net.h>  // 네트워크 관련 헤더 파일
#include <linux/in.h>  // 인터넷 프로토콜 관련 헤더 파일
#include <linux/socket.h>  // 소켓 관련 헤더 파일
#include <net/sock.h>  // 소켓 구조체 관련 헤더 파일
#include <linux/string.h>  // 문자열 처리 관련 헤더 파일
#include <linux/inet.h>  // IP 주소 변환 관련 헤더 파일

// 시스템 콜 정의: mychat
SYSCALL_DEFINE2(mychat, char __user *, ip, char __user *, msg) {
    struct socket *sock;  // 소켓 구조체 선언
    struct sockaddr_in saddr;  // 서버 주소 구조체 선언
    char kmsg[128];  // 커널레벨에서 사용할 메세지 버퍼 선언
    char kip[16];  // 커널레벨에서 사용할 IP 주소 버퍼 선언
    int len, ret;  // 메세지 길이와 반환값 변수 선언
    struct msghdr msg_hdr;  // 메세지 헤더 구조체 선언
    struct kvec vec;  // 커널 벡터 구조체 선언
    unsigned char buf[4];  // IP 주소 변환 버퍼 선언

    // 유저레벨에서 커널레벨로 IP 주소 복사
    if (copy_from_user(kip, ip, sizeof(kip))) {
        return -EFAULT;  // 실패 시 오류 반환
    }

    // 유저레벨에서 커널레벨로 메시지 복사
    if (copy_from_user(kmsg, msg, sizeof(kmsg))) {
        return -EFAULT;  // 실패 시 오류 반환
    }

		// 커널 로그에 메세지 출력
    printk(KERN_INFO "mychat: received message: %s\n", kmsg);

    // IP 주소 변환
    if (in4_pton(kip, -1, buf, -1, NULL) == 0) {
		    // 잘못된 주소이 녁ㄹ루 오류 출력 및 반환
        printk(KERN_ERR "mychat: invalid IP address\n");
        return -EINVAL;
    }

    // 소켓 생성
    ret = sock_create(PF_INET, SOCK_DGRAM, IPPROTO_UDP, &sock);
    if (ret < 0) {
		    // 실패 시 오류 출력 및 반환
        printk(KERN_ERR "mychat: socket creation failed\n");
        return ret;
    }

    // 서버 주소 설정
    memset(&saddr, 0, sizeof(saddr));  // 주소 구조체 초기화
    saddr.sin_family = AF_INET;  // 주소 패밀리 설정
    saddr.sin_port = htons(12345); // 서버 포트 번호 설정
    memcpy(&saddr.sin_addr.s_addr, buf, sizeof(saddr.sin_addr.s_addr));  // IP 주소 설정

    len = strlen(kmsg);  // 메세지 길이 계산
    vec.iov_base = kmsg;  // 벡터의 데이터 포인터 설정
    vec.iov_len = len;  // 벡터의 데이터 길이정

    memset(&msg_hdr, 0, sizeof(msg_hdr));  // 메세지 헤더 초기화
    msg_hdr.msg_name = &saddr;  // 메세지 헤더에 주소 설정
    msg_hdr.msg_namelen = sizeof(saddr);  // 주소 길이 설정

		// 메세지 전송
    ret = kernel_sendmsg(sock, &msg_hdr, &vec, 1, len);
    if (ret < 0) {
		    // 실패 시 오류 출력 및 반환, 소켓 해제
        printk(KERN_ERR "mychat: message sending failed\n");
        sock_release(sock);
        return ret;
    }

    sock_release(sock);  // 소켓 해제
    printk(KERN_INFO "mychat: message sent to %s\n", kip);  // 전송 완료 메세지 출력
    return 0;  // 성공 반환
}
