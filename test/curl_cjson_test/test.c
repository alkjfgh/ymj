#include <stdio.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <string.h>
#include <stdlib.h>

// 응답 데이터를 저장하기 위한 구조체
struct MemoryStruct
{
    char *memory;
    size_t size;
};

// CURL 응답 콜백 함수
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr)
        return 0;

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

static size_t WriteFileCallback(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return fwrite(ptr, size, nmemb, stream);
}
int downloadFile(const char *url, const char *filename)
{
    CURL *curl;
    FILE *fp;
    CURLcode res;
    double file_size = 0;

    fp = fopen(filename, "wb");
    if (!fp)
    {
        printf("파일을 열 수 없습니다: %s\n", filename);
        return -1; // 에러 코드 변경
    }

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFileCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            printf("다운로드 실패: %s\n", curl_easy_strerror(res));
        }
        else
        {
            double file_size = 0;
            curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &file_size);
            return (long)file_size;
        }

        curl_easy_cleanup(curl);
    }

    fclose(fp);
    return (res == CURLE_OK) ? (long)file_size : -1; // 성공시 파일 크기 반환, 실패시 -1 반환
}

int main(void)
{
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
    if (curl)
    {
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

        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        else
        {
            // 응답 JSON 파싱
            cJSON *response = cJSON_Parse(chunk.memory);
            if (response)
            {
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

    // 파일 다운로드 테스트
    const char *download_url = "https://ash-speed.hetzner.com/100MB.bin"; // 다운로드할 URL
    const char *save_filename = "100MB.bin";                              // 저장할 파일명

    printf("파일 다운로드 시작...\n");
    int download_result = downloadFile(download_url, save_filename);
    if (download_result >= 0)
    {
        printf("파일 다운로드 성공: %s (크기: %.2f MB)\n",
               save_filename, (double)download_result / (1024 * 1024));
    }
    else
    {
        printf("파일 다운로드 실패\n");
    }

    return 0;
}