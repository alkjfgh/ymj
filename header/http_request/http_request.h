#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

/**
 * @brief HTTP 응답을 저장하기 위한 구조체
 *
 * HTTP 서버로부터 받은 응답을 저장하는 구조체입니다.
 */
typedef struct
{
    int status_code;          /**< HTTP 상태 코드 (예: 200, 404, 500 등) */
    char status_message[256]; /**< HTTP 상태 메시지 (예: "OK", "Not Found" 등) */
    char headers[4096];       /**< HTTP 응답 헤더들을 저장하는 문자열 */
    char body[8192];          /**< HTTP 응답 본문을 저장하는 문자열 */
    int is_chunked;           /**< chunked 인코딩 사용 여부 (1: 사용, 0: 미사용) */
    long content_length;      /**< 컨텐츠 길이 (바이트 단위) */
} http_response_t;

/**
 * @brief HTTP GET 요청을 전송하고 응답을 받습니다.
 *
 * @param ip 대상 서버의 IP 주소
 * @param port 대상 서버의 포트 번호
 * @param path 요청할 리소스의 경로
 * @return int 성공 시 0, 실패 시 -1을 반환
 */
int send_http_request(const char *ip, int port, const char *path);

/**
 * @brief HTTP GET 요청을 전송하고 응답을 파일로 저장합니다.
 *
 * @param ip 대상 서버의 IP 주소
 * @param port 대상 서버의 포트 번호
 * @param path 요청할 리소스의 경로
 * @param save_path 응답을 저장할 파일 경로
 * @return int 성공 시 0, 실패 시 -1을 반환
 */
int download_http_request(const char *ip, int port, const char *path, const char *save_path);

/**
 * @brief HTTP POST 요청을 전송하고 응답을 받습니다.
 *
 * @param ip 대상 서버의 IP 주소
 * @param port 대상 서버의 포트 번호
 * @param path 요청할 리소스의 경로
 * @param data POST 요청에 포함할 데이터
 * @return int 성공 시 0, 실패 시 -1을 반환
 */
int send_http_post_request(const char *ip, int port, const char *path, const char *data);

#endif