#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(void)
{
    int sockfd;
    struct sockaddr_in server_addr;

    // 소켓 생성
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("소켓 생성 실패");
        return 1;
    }

    // 서버 주소 구조체 초기화
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    if (inet_pton(AF_INET, "1.209.148.92", &server_addr.sin_addr) <= 0)
    {
        perror("잘못된 IP 주소");
        return 1;
    }

    // 서버에 연결
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("연결 실패");
        return 1;
    }

    // HTTP 요청 보내기
    char *request = "GET /info1.php HTTP/1.1\r\nHost: 1.209.148.92\r\n\r\n";
    if (write(sockfd, request, strlen(request)) < 0)
    {
        perror("요청 전송 실패");
        return 1;
    }

    // 응답 받기
    char buffer[4096];
    int bytes;

    while ((bytes = read(sockfd, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytes] = '\0';
        printf("%s", buffer);

        // "0\r\n\r\n"을 발견하면 청크 전송이 완료된 것
        if (strstr(buffer, "0\r\n\r\n") != NULL)
        {
            break;
        }
    }

    close(sockfd);
    return 0;
}