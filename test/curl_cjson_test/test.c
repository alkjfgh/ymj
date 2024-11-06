#include <stdio.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <string.h>
#include <stdlib.h>

// 응답 데이터를 저장하기 위한 구조체
struct MemoryStruct {
    char *memory;
    size_t size;
};

// CURL 응답 콜백 함수
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) return 0;

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

int main(void) {
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;
    
    // 메모리 초기화
    chunk.memory = malloc(1);
    chunk.size = 0;

    // 보낼 JSON 데이터 생성
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "테스트");
    cJSON_AddNumberToObject(root, "age", 25);
    char *json_str = cJSON_Print(root);

    curl = curl_easy_init();
    if (curl) {
        // CURL 옵션 설정
        curl_easy_setopt(curl, CURLOPT_URL, "https://httpbin.org/anything");
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str);
        
        // 헤더 설정
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // 응답 데이터 콜백 설정
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        // 요청 실행
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            // 응답 JSON 파싱
            cJSON *response = cJSON_Parse(chunk.memory);
            if (response) {
                char *pretty = cJSON_Print(response);
                printf("응답 데이터:\n%s\n", pretty);
                free(pretty);
                cJSON_Delete(response);
            }
        }

        // 정리
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    // 메모리 해제
    free(json_str);
    cJSON_Delete(root);
    free(chunk.memory);

    return 0;
}