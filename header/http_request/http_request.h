#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

// HTTP 응답 구조체
typedef struct
{
    int status_code;
    char status_message[256];
    char headers[4096];
    char body[8192];
    int is_chunked;
    long content_length;
} http_response_t;

// HTTP GET 요청을 보내고 응답을 받는 함수
// 성공시 0, 실패시 -1 반환
int send_http_request(const char *ip, int port, const char *path);

// HTTP GET 요청을 보내고 응답을 받아 파일로 저장하는 함수
// 성공시 0, 실패시 -1 반환
int download_http_request(const char *ip, int port, const char *path, const char *save_path);

// HTTP POST 요청을 보내고 응답을 받는 함수
// 성공시 0, 실패시 -1 반환
int send_http_post_request(const char *ip, int port, const char *path, const char *data);

#endif