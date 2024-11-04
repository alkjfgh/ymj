#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

// HTTP GET 요청을 보내고 응답을 받는 함수
// 성공시 0, 실패시 -1 반환
int send_http_request(const char *ip, int port, const char *path);

#endif