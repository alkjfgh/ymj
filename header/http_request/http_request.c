#define _GNU_SOURCE

#include "http_request.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <strings.h>

static int g_line_counter = 0;

// 새로운 공통 함수 추가
static void handle_http_response(int sockfd)
{
    char buffer[4096];
    int bytes;

    while ((bytes = read(sockfd, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytes] = '\0';
        printf("%s", buffer);

        // chunked 인코딩 종료 확인 (send_http_request에서 사용하던 체크)
        if (strstr(buffer, "0\r\n\r\n") != NULL)
        {
            break;
        }
    }
}

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

    // 응답 받기 부분을 공통 함수로 교체
    handle_http_response(sockfd);

    close(sockfd);
    return 0;
}

int download_http_request(const char *ip, int port, const char *path, const char *save_path)
{
    int sockfd;
    struct sockaddr_in server_addr;
    FILE *fp = NULL;
    long content_length = -1;
    int header_ended = 0;
    int my_line = __sync_fetch_and_add(&g_line_counter, 1);

    // 저장할 파일 열기
    fp = fopen(save_path, "wb");
    if (!fp)
    {
        perror("파일 생성 실패");
        return -1;
    }

    // 소켓 생성
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("소켓 생성 실패");
        fclose(fp);
        return -1;
    }

    // 서버 주소 설정 부분은 동일
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0)
    {
        perror("잘못된 IP 주소");
        close(sockfd);
        fclose(fp);
        return -1;
    }

    // 연결 부분
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("연결 실패");
        close(sockfd);
        fclose(fp);
        return -1;
    }

    // HTTP 요청에 Accept-Encoding 헤더 추가
    char request[1024];
    snprintf(request, sizeof(request),
             "GET %s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Accept-Encoding: identity\r\n" // 압축 해제 방지
             "Connection: close\r\n\r\n",
             path, ip);

    // 요청 전송
    if (write(sockfd, request, strlen(request)) < 0)
    {
        perror("요청 전송 실패");
        close(sockfd);
        fclose(fp);
        return -1;
    }

    // 응답 처리
    char buffer[8192];
    char *header_end;
    size_t total_bytes = 0;
    int bytes;

    time_t start_time = time(NULL);
    time_t current_time;
    float speed = 0.0;

    const char *filename = strrchr(save_path, '/');
    filename = filename ? filename + 1 : save_path;

    printf("%s 다운로드 시작...\n", filename);
    fflush(stdout);
    // 현재 다운로드의 기준 줄 위치 저장
    int base_line = my_line;

    while ((bytes = read(sockfd, buffer, sizeof(buffer))) > 0)
    {
        if (!header_ended)
        {
            // HTTP 헤더 처리
            buffer[bytes] = '\0';
            if ((header_end = strstr(buffer, "\r\n\r\n")) != NULL)
            {
                // Content-Length 찾기
                char *content_length_str = strcasestr(buffer, "Content-Length:");
                if (content_length_str)
                {
                    sscanf(content_length_str, "Content-Length: %ld", &content_length);
                }

                // 헤더 이후의 데이터 크기 계산
                header_end += 4; // \r\n\r\n 건너뛰기
                int body_bytes = bytes - (header_end - buffer);
                if (body_bytes > 0)
                {
                    fwrite(header_end, 1, body_bytes, fp);
                    total_bytes += body_bytes;
                }
                header_ended = 1;
            }
        }
        else
        {
            // 본문 데이터 저장
            fwrite(buffer, 1, bytes, fp);
            total_bytes += bytes;
        }

        current_time = time(NULL);
        double elapsed_time = difftime(current_time, start_time);

        if (elapsed_time > 0)
        {
            speed = (total_bytes / (1024.0 * 1024.0)) / elapsed_time; // MB/s
        }

        // 진행률 표시
        if (content_length > 0)
        {
            float progress = (float)total_bytes / content_length * 100;
            printf("\r\033[K%s: %.1f%% (%.2f MB/%.2f MB) - %.2f MB/s",
                   filename,
                   progress,
                   total_bytes / (1024.0 * 1024.0),
                   content_length / (1024.0 * 1024.0),
                   speed);
        }
        else
        {
            printf("\r\033[K%s: %.2f MB - %.2f MB/s",
                   filename,
                   total_bytes / (1024.0 * 1024.0),
                   speed);
        }
        fflush(stdout);
    }

    // 다운로드 완료 시 줄바꿈을 포함하여 출력
    printf("\n%s: 다운로드 완료 - 총 %.2f MB\n", filename, total_bytes / (1024.0 * 1024.0));
    fflush(stdout);

    fclose(fp);
    close(sockfd);

    __sync_fetch_and_sub(&g_line_counter, 1);

    return 0;
}

int send_http_post_request(const char *ip, int port, const char *path, const char *data)
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

    // HTTP POST 요청 문자열 생성
    char request[4096];
    snprintf(request, sizeof(request),
             "POST %s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Content-Type: application/json\r\n" // JSON 컨텐츠 타입으로 변경
             "Content-Length: %zu\r\n"
             "Connection: close\r\n"
             "\r\n"
             "%s",
             path, ip, strlen(data), data);

    // HTTP 요청 보내기
    if (write(sockfd, request, strlen(request)) < 0)
    {
        perror("요청 전송 실패");
        close(sockfd);
        return -1;
    }

    // 응답 받기 부분을 공통 함수로 교체
    handle_http_response(sockfd);

    close(sockfd);
    return 0;
}