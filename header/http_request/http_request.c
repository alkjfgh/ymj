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

// 여러 다운로드의 동시 진행을 위한 전역 라인 카운터
static int g_line_counter = 0;

/**
 * @brief 청크 인코딩된 HTTP 응답 본문을 디코딩하는 함수
 * @param chunked_body 청크 인코딩된 원본 데이터
 * @param decoded_body 디코딩된 데이터를 저장할 버퍼
 */
static void decode_chunked_body(const char *chunked_body, char *decoded_body)
{
    const char *current = chunked_body;
    char *output = decoded_body;

    while (1)
    {
        // 청크 크기 읽기
        long chunk_size;
        sscanf(current, "%lx", &chunk_size);

        // 청크 크기가 0이면 종료
        if (chunk_size == 0)
            break;

        // 청크 데이터의 시작 위치로 이동 (\r\n 건너뛰기)
        current = strchr(current, '\n') + 1;

        // 청크 데이터 복사
        memcpy(output, current, chunk_size);
        output += chunk_size;

        // 다음 청크로 이동 (\r\n 건너뛰기)
        current += chunk_size + 2;
    }
    *output = '\0';
}

/**
 * @brief HTTP 응답을 파싱하여 구조체에 저장하는 함수
 * @param response 원본 HTTP 응답 문자열
 * @param result 파싱된 결과를 저장할 구조체
 * @details 상태 코드, 헤더, 본문을 분리하고 청크 인코딩 처리
 */
static void parse_http_response(const char *response, http_response_t *result)
{
    // 상태 코드와 메시지 파싱
    sscanf(response, "HTTP/1.1 %d %[^\r\n]", &result->status_code, result->status_message);

    // 헤더와 바디 분리
    const char *headers_start = strchr(response, '\n') + 1;
    const char *body_start = strstr(response, "\r\n\r\n");

    if (body_start)
    {
        // 헤더 복사
        int headers_length = body_start - headers_start;
        strncpy(result->headers, headers_start, headers_length);
        result->headers[headers_length] = '\0';

        // 바디 복사
        body_start += 4; // \r\n\r\n 건너뛰기
        strcpy(result->body, body_start);

        // Transfer-Encoding 확인
        result->is_chunked = (strcasestr(result->headers, "Transfer-Encoding: chunked") != NULL);

        // Content-Length 확인
        const char *content_length_str = strcasestr(result->headers, "Content-Length:");
        if (content_length_str)
        {
            sscanf(content_length_str, "Content-Length: %ld", &result->content_length);
        }

        // 청크 인코딩인 경우 디코딩 수행
        if (result->is_chunked)
        {
            char decoded_body[8192] = {0}; // 적절한 크기로 조정 필요
            decode_chunked_body(result->body, decoded_body);
            strcpy(result->body, decoded_body);
        }
    }
}

/**
 * @brief 소켓으로부터 HTTP 응답을 읽고 처리하는 함수
 * @param sockfd 연결된 소켓 디스크립터
 * @details 청크 인코딩 및 일반 응답 모두 처리 가능
 */
static void handle_http_response(int sockfd)
{
    char buffer[16384] = {0};
    int bytes;
    int total_bytes = 0;

    while ((bytes = read(sockfd, buffer + total_bytes, sizeof(buffer) - total_bytes - 1)) > 0)
    {
        total_bytes += bytes;
        buffer[total_bytes] = '\0';

        if (strstr(buffer, "\r\n\r\n") != NULL)
        {
            if (strstr(buffer, "0\r\n\r\n") != NULL || !strstr(buffer, "Transfer-Encoding: chunked"))
            {
                break;
            }
        }
    }

    http_response_t response = {0};
    parse_http_response(buffer, &response);

    // 응답 정보 출력
    printf("상태 코드: %d\n", response.status_code);
    printf("상태 메시지: %s\n", response.status_message);
    printf("\n=== 헤더 ===\n%s\n", response.headers);
    printf("\n=== 바디 ===\n%s\n", response.body);
}

/**
 * @brief HTTP GET 요청을 보내고 응답을 처리하는 함수
 * @param ip 대상 서버 IP
 * @param port 대상 서버 포트
 * @param path 요청 경로
 * @return 성공 시 0, 실패 시 -1
 */
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

/**
 * @brief HTTP GET 요청으로 파일을 다운로드하는 함수
 * @param ip 대상 서버 IP
 * @param port 대상 서버 포트
 * @param path 요청 경로
 * @param save_path 저장할 로컬 파일 경로
 * @return 성공 시 0, 실패 시 -1
 * @details 진행률 표시 및 다운로드 속도 계산 기능 포함
 */
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

/**
 * @brief HTTP POST 요청을 보내고 응답을 처리하는 함수
 * @param ip 대상 서버 IP
 * @param port 대상 서버 포트
 * @param path 요청 경로
 * @param data POST 요청에 포함할 JSON 데이터
 * @return 성공 시 0, 실패 시 -1
 */
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