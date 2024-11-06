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
#include "../../header/thread/thread.h"
#include <unistd.h>

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
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        printf("서버 응답 데이터: %s\n", chunk.memory);
        res = curl_easy_perform(curl);
        fprintf(stderr, "POST request failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        free(chunk.memory);
        free(post_data);
        curl_slist_free_all(headers);
        return NULL;
    }

    long response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    if (response_code != 200)
    {
        fprintf(stderr, "서버 응답 오류: HTTP %ld\n", response_code);
        curl_easy_cleanup(curl);
        free(chunk.memory);
        free(post_data);
        curl_slist_free_all(headers);
        return NULL;
    }

    cJSON *json = cJSON_Parse(chunk.memory);
    if (json == NULL)
    {
        // JSON 파싱 실패 시 새로운 JSON 객체 생성
        cJSON *response = cJSON_CreateObject();
        cJSON_AddStringToObject(response, "status", "error");
        cJSON_AddStringToObject(response, "message", "Invalid JSON response");
        cJSON_AddStringToObject(response, "raw_response", chunk.memory);

        curl_easy_cleanup(curl);
        free(chunk.memory);
        free(post_data);
        curl_slist_free_all(headers);
        return response; // 새로 생성한 JSON 객체 반환
    }

    curl_easy_cleanup(curl);
    free(chunk.memory);
    free(post_data);
    curl_slist_free_all(headers);
    return json;
}

void polling_by_curl(int thread_num)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);

    // POST용 JSON 데이터 생성
    cJSON *post_data = cJSON_CreateObject();
    cJSON_AddStringToObject(post_data, "log_type", "tc_connect_continue");
    cJSON_AddStringToObject(post_data, "user_id", "00-15-5d-a9-c9-8a");
    cJSON_AddStringToObject(post_data, "time_stamp", "2024-08-08 13:51:25");
    cJSON_AddStringToObject(post_data, "mac", "00-15-5d-a9-c9-8a");
    cJSON_AddStringToObject(post_data, "local_ip", "172.22.107.42");
    cJSON_AddStringToObject(post_data, "web_ip", "121.134.218.173");
    cJSON_AddStringToObject(post_data, "config_linux_hash", "88a31795700d20d898b7f2d870bdbddaa47be2e497529a8ac1aea55c45d11f74");
    cJSON_AddNumberToObject(post_data, "tc_cpu_usage", 1.0);
    cJSON_AddNumberToObject(post_data, "cpu_usage", 1.0);
    cJSON_AddNumberToObject(post_data, "memory_load", 5);
    cJSON_AddNumberToObject(post_data, "process_cnt", 538);
    cJSON_AddNumberToObject(post_data, "net_usage", 17188205);
    cJSON_AddStringToObject(post_data, "operating_mode", "detect");

    const char *url = "http://59.6.79.138:8080/tc_connect_continue.php";
    // const char *url = "https://httpbin.org/post";

    cJSON *post_response = test_http_post(url, post_data);
    if (post_response)
    {
        char *json_str = cJSON_Print(post_response);
        printf("[Thread %d] %s POST 응답 JSON:\n%s\n\n\n",
               thread_num, url, json_str);

        free(json_str);
        cJSON_Delete(post_response);
    }

    cJSON_Delete(post_data);

    curl_global_cleanup();
}

void *polling_thread(void *arg)
{
    ThreadContext *context = (ThreadContext *)arg;
    int thread_num = context->thread_num;

    for (int i = 0; i < 10; i++)
    {
        printf("[Thread %d] %d번째 폴링 시작\n", thread_num, i + 1);
        polling_by_curl(thread_num);
        sleep(1);
    }

    return NULL;
}

/**
 * @brief 메인 함수
 * @return 프로그램 종료 상태
 * @retval 0 성공
 */
int main(void)
{
    const int NUM_THREADS = 4;

    int result = create_and_run_threads_with_context(polling_thread, NUM_THREADS, NULL);
    if (result != 0)
    {
        fprintf(stderr, "스레드 실행 실패\n");
        return -1;
    }

    return 0;
}