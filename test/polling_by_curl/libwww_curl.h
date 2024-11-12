/**
 * @file libwww_curl.h
 * @brief 웹으로부터 데이터를 받아오는 함수를 정의한 헤더 파일
 * @note 컴파일: -lcurl (동적 라이브러리)
 */
#ifndef LIBWWW_CURL_H
#define LIBWWW_CURL_H

#include <curl/curl.h>

/**
 * @defgroup ErrorCodes 에러 코드 정의
 * @{
 */
#define POST_SUCCESS 0            /**< 요청 성공 */
#define POST_CURL_INIT_FAIL -1    /**< CURL 초기화 실패 */
#define POST_CURL_PERFORM_FAIL -2 /**< CURL 수행 실패 */
#define POST_HTTP_ERROR -3        /**< HTTP 요청 실패 */
/** @} */

/**
 * @defgroup TimeoutSettings 응답시간 제어 디폴트 값
 * @{
 */
#define POST_TOTAL_TIMEOUT 30   /**< 전체 타임아웃 (30초) */
#define POST_CONNECT_TIMEOUT 10 /**< 연결 타임아웃 (10초) */
/** @} */

/**
 * @brief 웹 서버에 POST 요청을 보내고 응답을 받아오는 함수
 *
 * @param server_ip 서버 IP 주소
 * @param port 포트 번호
 * @param url 요청 URL
 * @param is_https HTTPS 사용 여부 (0: HTTP, 1: HTTPS)
 * @param json_data_send 전송할 JSON 문자열
 * @param json_data_recv 응답 JSON 문자열을 받을 변수의 포인터
 *
 * @return int 성공(0) 또는 실패(음수) 상태 코드
 */
int post_www(const char *server_ip, int port, const char *url, int is_https, const char *json_data_send, char **json_data_recv);

/**
 * @brief 웹 서버에 GET 요청을 보내고 응답을 받아오는 함수
 *
 * @param ip 서버 IP 주소
 * @param port 포트 번호
 * @param url 요청 URL
 * @param https HTTPS 사용 여부 (0: HTTP, 1: HTTPS)
 * @param response 응답 문자열을 받을 변수의 포인터
 *
 * @return int 성공(0) 또는 실패(음수) 상태 코드
 */
int get_www(const char *ip, int port, const char *url, int https, char **response);

#endif