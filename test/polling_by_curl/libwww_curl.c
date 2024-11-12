#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globaldata.h"

#include "libdebug.h"
#include "libconfig.h"

#include "libwww_curl.h"

/////////////////////////////////
//  curl version

// 응답을 저장할 구조체
struct MemoryStruct
{
    char *memory;
    size_t size;
};

// 응답을 메모리에 저장하는 콜백 함수
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL)
    {
        printf("realloc failed\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

/**
 * @brief 웹 서버에 POST 요청을 보내고 응답을 받아오는 함수 (SSL 지원)
 *
 * @param server_ip 서버 IP 주소
 * @param port 포트 번호
 * @param url 요청 URL
 * @param is_https HTTPS 사용 여부 (0: HTTP, 1: HTTPS)
 * @param json_data_send 전송할 JSON 문자열
 * @param json_data_recv 응답 JSON 문자열을 받을 변수의 포인터
 *
 * @return int 성공(0), 실패(음수)
 *         - POST_SUCCESS: 성공
 *         - POST_CURL_INIT_FAIL: CURL 초기화 실패
 *         - POST_CURL_PERFORM_FAIL: CURL 요청 실패
 *         - POST_HTTP_ERROR: HTTP 요청 실패
 *
 * @note web_control 섹션의 설정:
 * - server_ip: 서버 IP 주소
 * - http_port: HTTP 포트
 * - https_port: HTTPS 포트
 * - https: HTTPS 사용 여부
 * - post_total_timeout: 전체 요청 타임아웃
 * - post_connect_timeout: 연결 타임아웃
 */
int post_www(const char *server_ip, int port, const char *url, int is_https, const char *json_data_send, char **json_data_recv)
{
    int level = 3;
    char temp[1024 * 2];
    const char *section = "libwww_curl:post_www";

    CURL *curl;
    CURLcode res;
    long http_code = 0;
    // int is_https = 0;
    char full_url[1024];

    if (is_https == 1)
    {
        sprintf(full_url, "https://%s:%d%s", server_ip, port, url);
    }
    else
    {
        sprintf(full_url, "http://%s:%d%s", server_ip, port, url);
    }

    sprintf(temp, "full_url:%s\n", full_url);
    debug_log(level, NULL, section, temp);

    // 응답 데이터를 저장할 구조체 초기화
    struct MemoryStruct chunk;
    chunk.memory = malloc(1); // 1바이트 할당
    chunk.size = 0;           // 초기 크기 0

    // CURL 초기화
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (!curl)
    {
        // fprintf(stderr, "curl_easy_init() failed\n");
        sprintf(temp, "curl_easy_init() failed\n");
        debug_log(level, NULL, section, temp);

        return POST_CURL_INIT_FAIL;
    }

    // 요청 URL 설정
    sprintf(temp, "full_url:%s\n", full_url);
    debug_log(4, NULL, "post_www", temp);

    curl_easy_setopt(curl, CURLOPT_URL, full_url);

    // POST 요청 설정
    curl_easy_setopt(curl, CURLOPT_POST, 1L);

    // JSON 데이터 설정
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data_send);

    // JSON 형식 지정
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // HTTPS일 경우 SSL 옵션 설정
    if (is_https)
    {
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L); // 서버 인증서 검증
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L); // 호스트 이름 검증
        // 인증서 경로 설정 필요 시
        // curl_easy_setopt(curl, CURLOPT_CAINFO, "/path/to/certificate.pem");
    }

    // 응답 데이터를 WriteMemoryCallback 함수로 처리하도록 설정
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    // 응답시간 제어
    int post_total_timeout = POST_TOTAL_TIMEOUT;
    int post_connect_timeout = POST_CONNECT_TIMEOUT;

    if (0 == config_get_key_value_section(g_config, "post_total_timeout", temp, sizeof(temp), "web_control"))
    {
        post_total_timeout = atoi(temp);
    }
    if (0 == config_get_key_value_section(g_config, "post_connect_timeout", temp, sizeof(temp), "web_control"))
    {
        post_connect_timeout = atoi(temp);
    }

    // 타임아웃 설정
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, post_total_timeout);          // 전체 요청 타임아웃 (예: 10초)
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, post_connect_timeout); // 연결 타임아웃 (예: 5초)

    // 요청 실행
    res = curl_easy_perform(curl);

    // 요청 결과 확인
    if (res != CURLE_OK)
    {
        // fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        sprintf(temp, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        debug_log(level, NULL, section, temp);

        free(chunk.memory);
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        curl_global_cleanup();
        return POST_CURL_PERFORM_FAIL;
    }

    // HTTP 상태 코드 확인
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    if (http_code != 200)
    {
        // fprintf(stderr, "Request failed. HTTP Status Code: %ld\n", http_code);
        sprintf(temp, "Request failed. HTTP Status Code: %ld\n", http_code);
        debug_log(level, NULL, section, temp);

        free(chunk.memory);
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        curl_global_cleanup();
        return POST_HTTP_ERROR;
    }

    // 응답을 json_data_recv에 할당
    *json_data_recv = strdup(chunk.memory);

    // 정리
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    free(chunk.memory);
    curl_global_cleanup();

    return POST_SUCCESS;
}

/**
 * @brief 웹 서버에 GET 요청을 보내고 응답을 받아오는 함수 (SSL 지원)
 *
 * @param ip 서버 IP 주소
 * @param port 포트 번호
 * @param url 요청 URL
 * @param https HTTPS 사용 여부 (0: HTTP, 1: HTTPS)
 * @param response 응답 문자열을 받을 변수의 포인터
 *
 * @return int 성공(0), 실패(음수)
 */
int get_www(const char *ip, int port, const char *url, int https, char **response)
{
    char temp[1024];
    int level = 3;
    const char *section = "libwww_curl:www_get";

    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;

    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl)
    {
        char full_url[256];
        snprintf(full_url, sizeof(full_url), "%s://%s:%d%s", https ? "https" : "http", ip, port, url);

        sprintf(temp, "GET %s\n", full_url);
        debug_log(level, NULL, section, temp);

        curl_easy_setopt(curl, CURLOPT_URL, full_url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            sprintf(temp, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            debug_log(level, NULL, section, temp);
        }
        else
        {
            *response = strdup(chunk.memory);
        }

        curl_easy_cleanup(curl);
    }

    free(chunk.memory);
    curl_global_cleanup();

    return (res == CURLE_OK) ? 0 : -1;
}
