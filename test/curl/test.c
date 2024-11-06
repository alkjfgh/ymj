/**
 * @file test.c
 * @brief HTTP 요청을 수행하고 응답을 메모리에 저장하는 예제
 * @author Your Name
 * @date YYYY-MM-DD
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

/**
 * @struct MemoryStruct
 * @brief 응답 데이터를 저장하기 위한 메모리 구조체
 */
struct MemoryStruct
{
    char *memory; /**< 동적으로 할당된 메모리 버퍼 */
    size_t size;  /**< 현재 메모리 버퍼의 크기 */
};

/**
 * @brief libcurl의 쓰기 콜백 함수
 * @param contents 수신된 데이터
 * @param size 각 데이터 멤버의 크기
 * @param nmemb 데이터 멤버의 수
 * @param userp 사용자 정의 포인터 (MemoryStruct 구조체)
 * @return 처리된 바이트 수
 * @retval 0 메모리 할당 실패
 * @retval realsize 성공적으로 처리된 바이트 수
 */
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr)
    {
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

/**
 * @brief HTTP GET 요청 테스트 함수
 * @param url 요청할 URL
 * @return 성공 시 JSON 객체 포인터, 실패 시 NULL
 */
cJSON *test_http_get(const char *url)
{
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl = curl_easy_init();
    if (!curl)
        return NULL;

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        fprintf(stderr, "GET request failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        free(chunk.memory);
        return NULL;
    }

    cJSON *json = cJSON_Parse(chunk.memory);
    if (json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "JSON 파싱 오류: %s\n", error_ptr);
        }
        curl_easy_cleanup(curl);
        free(chunk.memory);
        return NULL;
    }

    curl_easy_cleanup(curl);
    free(chunk.memory);
    return json;
}

/**
 * @brief HTTP POST 요청 테스트 함수
 * @param url 요청할 URL
 * @param json_data POST로 전송할 JSON 데이터
 * @return 성공 시 JSON 객체 포인터, 실패 시 NULL
 */
cJSON *test_http_post(const char *url, const cJSON *json_data)
{
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl = curl_easy_init();
    if (!curl)
        return NULL;

    // JSON 데이터를 문자열로 변환
    char *post_data = cJSON_Print(json_data);
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        fprintf(stderr, "POST request failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        free(chunk.memory);
        free(post_data);
        curl_slist_free_all(headers);
        return NULL;
    }

    cJSON *json = cJSON_Parse(chunk.memory);
    if (json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "JSON 파싱 오류: %s\n", error_ptr);
        }
        curl_easy_cleanup(curl);
        free(chunk.memory);
        free(post_data);
        curl_slist_free_all(headers);
        return NULL;
    }

    curl_easy_cleanup(curl);
    free(chunk.memory);
    free(post_data);
    curl_slist_free_all(headers);
    return json;
}

/**
 * @brief 메인 함수
 * @return 프로그램 종료 상태
 * @retval 0 성공
 */
int main(void)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);

    printf("=== GET 요청 테스트 ===\n");
    cJSON *get_response = test_http_get("https://httpbin.org/get");
    if (get_response)
    {
        char *json_str = cJSON_Print(get_response);
        printf("GET 응답 JSON:\n%s\n", json_str);
        free(json_str);
        cJSON_Delete(get_response);
    }

    printf("\n=== POST 요청 테스트 ===\n");
    // POST용 JSON 데이터 생성
    cJSON *post_data = cJSON_CreateObject();
    cJSON_AddStringToObject(post_data, "name", "test");
    cJSON_AddStringToObject(post_data, "message", "hello");

    cJSON *post_response = test_http_post("https://httpbin.org/post", post_data);
    if (post_response)
    {
        char *json_str = cJSON_Print(post_response);
        printf("POST 응답 JSON:\n%s\n", json_str);
        free(json_str);
        cJSON_Delete(post_response);
    }

    cJSON_Delete(post_data);

    curl_global_cleanup();
    return 0;
}