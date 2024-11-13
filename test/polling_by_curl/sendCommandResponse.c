/**
 * @file sendCommandResponse.c
 * @brief 명령어 응답 처리 및 서버 통신 관련 기능을 구현한 파일
 */

#include "sendCommandResponse.h"
#include "libwww_curl.h"
#include "globaldata.h"
#include <json-c/json.h>
#include <string.h>

/**
 * @brief POST 요청을 위한 JSON 데이터를 생성합니다
 * @param res 명령어 실행 결과 데이터
 * @param command_name 실행된 명령어 이름
 * @return 생성된 POST 데이터 JSON 객체
 */
struct json_object *makePostData(struct json_object *res, const char *command_name)
{
    struct json_object *postData = json_object_new_object();

    json_object_object_add(postData, "log_type", json_object_new_string("tc_command_res"));                       // 로그타입
    json_object_object_add(postData, "user_id", json_object_new_string("00-15-4d-a9-c9-8a"));                     // 사용자 아이디
    json_object_object_add(postData, "time_stamp", json_object_new_string("2024-11-13 14:30:00"));                // 로그 발생 시간
    json_object_object_add(postData, "mac", json_object_new_string("00-15-4d-a9-c9-8a"));                         // 맥 주소
    json_object_object_add(postData, "local_ip", json_object_new_string("172.22.107.42"));                        // 로컬 IP
    json_object_object_add(postData, "web_ip", json_object_new_string("121.134.218.173"));                        // 웹 IP
    json_object_object_add(postData, "command_serial", json_object_new_int64(3457034805));                        // 커맨드 구분 키
    json_object_object_add(postData, "command_hash", json_object_new_string("75689089754548975689576895467957")); // 커맨드 hash
    json_object_object_add(postData, "command_name", json_object_new_string(command_name));                       // 커맨드 이름
    json_object_object_add(postData, "res", res);

    return postData;
}

/**
 * @brief ZIP 파일 정보를 포함하는 JSON 데이터를 생성합니다
 * @return ZIP 파일 정보가 담긴 JSON 객체
 */
struct json_object *makeGetZipData(void)
{
    struct json_object *zipData = json_object_new_object();

    json_object_object_add(zipData, "folder_count", json_object_new_int(2));          // 폴더 개수
    json_object_object_add(zipData, "file_count", json_object_new_int(3));            // 파일 개수
    json_object_object_add(zipData, "zip_file_size", json_object_new_int64(5385934)); // 압축 파일 크기

    return zipData;
}

/**
 * @brief 시스템의 프로세스 목록을 조회합니다
 * @return 프로세스 정보 배열
 * @note 현재는 테스트용 더미 데이터를 반환합니다
 */
const ProcessInfo *getProcessList(void)
{
    // 프로세스 리스트 가져오기 처리 로직
    // TODO: 프로세스 리스트 가져오기 처리 로직 구현
    ProcessInfo processList[2];

    processList[0].pid = 2435;
    strncpy(processList[0].process_name, "/usr/ls", sizeof(processList[0].process_name));
    processList[0].process_desc = 4385934;
    strncpy(processList[0].process_param, "/usr/ls -l /usr", sizeof(processList[0].process_param));
    strncpy(processList[0].process_hash, "4385926546546546546546546754765", sizeof(processList[0].process_hash));
    processList[0].process_excluded = 1;

    // 두 번째 프로세스 정보
    processList[1].pid = 2433;
    strncpy(processList[1].process_name, "/usr/ps", sizeof(processList[1].process_name));
    processList[1].process_desc = 4385934;
    strncpy(processList[1].process_param, "/usr/ps -ef", sizeof(processList[1].process_param));
    strncpy(processList[1].process_hash, "4385926546546546546546546754765", sizeof(processList[1].process_hash));
    processList[1].process_excluded = 1;

    return processList;
}

/**
 * @brief 프로세스 목록 정보를 JSON 형태로 변환합니다
 * @return 프로세스 목록 정보가 담긴 JSON 배열
 */
struct json_object *makeGetProcessListData(void)
{
    struct json_object *processListData = json_object_new_array();
    const ProcessInfo *processList = getProcessList();
    int processCount = sizeof(processList) / sizeof(ProcessInfo);

    for (int i = 0; i < processCount; i++)
    {
        struct json_object *processData = json_object_new_object();
        json_object_object_add(processData, "pid", json_object_new_int(processList[i].pid));                           // 프로세스 ID
        json_object_object_add(processData, "process_name", json_object_new_string(processList[i].process_name));      // 프로세스 이름
        json_object_object_add(processData, "process_desc", json_object_new_int(processList[i].process_desc));         // 프로세스 설명
        json_object_object_add(processData, "process_param", json_object_new_string(processList[i].process_param));    // 프로세스 cmdline
        json_object_object_add(processData, "process_hash", json_object_new_string(processList[i].process_hash));      // 프로세스 해쉬
        json_object_object_add(processData, "process_excluded", json_object_new_int(processList[i].process_excluded)); // 프로세스 제외 여부
        json_object_array_add(processListData, processData);
    }

    return processListData;
}

/**
 * @brief 명령어 실행 결과 데이터를 생성합니다
 * @return 명령어 실행 결과 JSON 객체
 * @todo 실제 명령어 실행 결과 데이터 생성 로직 구현 필요
 */
struct json_object *makeCmdData(void)
{
    // TODO: 명령어 실행 결과 데이터 생성 로직 구현
    return NULL;
}

/**
 * @brief 서버에 명령어 실행 결과를 전송합니다
 * @param postData 전송할 데이터가 담긴 JSON 객체
 * @return 서버 응답 데이터 (실패 시 NULL)
 */
struct json_object *sendCommandResponse(struct json_object *postData)
{
    if (!postData)
        return NULL;

    struct json_object *response = NULL;
    const char *json_str = json_object_to_json_string(postData);
    char *response_str = NULL;

    int isSuccess = post_www(SERVER_IP, PORT_NUMBER, TARGET_URL, IS_HTTPS,
                             json_str, &response_str);

    if (isSuccess == 0)
    {
        printf("데이터 전송 성공\n");
        response = json_tokener_parse(response_str);
    }
    else
    {
        printf("데이터 전송 실패\n");
        return NULL;
    }

    return response;
}
