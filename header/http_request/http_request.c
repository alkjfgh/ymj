#include "http_request.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int send_http_request(const char *ip, int port, const char *path)
{
    int sockfd;
    struct sockaddr_in server_addr;

    // 소켓 생성
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("소켓 생성 실패");
        return -1;
    }

    // 서버 주소 구조체 초기화
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0)
    {
        perror("잘못된 IP 주소");
        close(sockfd);
        return -1;
    }

    // 서버에 연결
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("연결 실패");
        close(sockfd);
        return -1;
    }

    // HTTP 요청 문자열 생성
    char request[1024];
    snprintf(request, sizeof(request), "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", path, ip);

    // HTTP 요청 보내기
    if (write(sockfd, request, strlen(request)) < 0)
    {
        perror("요청 전송 실패");
        close(sockfd);
        return -1;
    }

    // 응답 받기
    char buffer[4096];
    int bytes;

    while ((bytes = read(sockfd, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytes] = '\0';
        printf("%s", buffer);

        if (strstr(buffer, "0\r\n\r\n") != NULL)
        {
            break;
        }
    }

    close(sockfd);
    return 0;
}