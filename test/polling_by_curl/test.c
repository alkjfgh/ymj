/**
 * @file test.c
 * @brief login, continue 요청 테스트 및 응답 분석 테스트
 * @author 유명재
 * @date 2024-11-12
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>
#include <unistd.h>
#include "libwww_curl.h"
#include "processCommand.h"

char time_str[20]; // 디버깅용 현재 시간을 저장할 변수

/**
 * @brief TC Connect Continue 데이터를 생성하는 함수
 * @return json_object* 생성된 JSON 객체
 * @details 시스템 상태 정보를 포함하는 JSON 데이터를 생성합니다
 */
struct json_object *makeTcConnectContinueData(void)
{
    struct json_object *post_data = json_object_new_object();

    json_object_object_add(post_data, "log_type", json_object_new_string("tc_connect_continue"));
    json_object_object_add(post_data, "user_id", json_object_new_string("00-15-5d-a9-c9-8a"));
    json_object_object_add(post_data, "time_stamp", json_object_new_string(time_str));
    json_object_object_add(post_data, "mac", json_object_new_string("00-15-5d-a9-c9-8a"));
    json_object_object_add(post_data, "local_ip", json_object_new_string("172.22.107.42"));
    json_object_object_add(post_data, "web_ip", json_object_new_string("121.134.218.173"));
    json_object_object_add(post_data, "config_linux_hash", json_object_new_string("76c7d5f43c3505ac6393406d2005cf4d95211bd4bcfca6c085b04b75062aea60"));
    json_object_object_add(post_data, "tc_cpu_usage", json_object_new_double(1.0));
    json_object_object_add(post_data, "cpu_usage", json_object_new_double(1.0));
    json_object_object_add(post_data, "memory_load", json_object_new_int(5));
    json_object_object_add(post_data, "process_cnt", json_object_new_int(538));
    json_object_object_add(post_data, "net_usage", json_object_new_int(17188205));
    json_object_object_add(post_data, "operating_mode", json_object_new_string("detect"));

    return post_data;
}

/**
 * @brief TC Connect Continue 요청을 수행하는 함수
 * @return json_object* 서버 응답 데이터
 * @details 서버에 POST 요청을 보내고 응답을 JSON 형태로 반환합니다
 */
struct json_object *postTcConnectContinue(void)
{
    struct json_object *post_data = makeTcConnectContinueData();
    char *result = NULL;
    struct json_object *json_response = NULL;
    struct json_object *final_response = json_object_new_object();

    int isSuccess = post_www("59.6.79.138", 8080, "/tc_connect_continue.php", 0,
                             json_object_to_json_string(post_data), &result);

    if (isSuccess == 0)
    {
        printf("POST 요청 성공\n");
        json_response = json_tokener_parse(result);
        if (json_response == NULL)
        {
            json_object_object_add(final_response, "data", json_object_new_string(result));
        }
        else
        {
            json_object_object_add(final_response, "data", json_response);
        }
    }
    else
    {
        printf("POST 요청 실패\n");
        json_object_object_add(final_response, "data", json_object_new_string("request failed"));
    }

    json_object_put(post_data);
    free(result);
    return final_response;
}

/**
 * @brief TC Connect Login 데이터를 생성하는 함수
 * @return json_object* 생성된 JSON 객체
 * @details 시스템 로그인 정보를 포함하는 JSON 데이터를 생성합니다
 */
struct json_object *makeTcConnectLoginData(void)
{
    struct json_object *post_data = json_object_new_object();

    json_object_object_add(post_data, "log_type", json_object_new_string("tc_connect_login"));
    json_object_object_add(post_data, "user_id", json_object_new_string("00-15-5d-a9-c9-8a"));
    json_object_object_add(post_data, "time_stamp", json_object_new_string(time_str));
    json_object_object_add(post_data, "mac", json_object_new_string("00-15-5d-a9-c9-8a"));
    json_object_object_add(post_data, "local_ip", json_object_new_string("172.22.107.42"));
    json_object_object_add(post_data, "web_ip", json_object_new_string("121.134.218.173"));
    json_object_object_add(post_data, "pc_manufacturer", json_object_new_string(""));
    json_object_object_add(post_data, "pc_model", json_object_new_string(""));
    json_object_object_add(post_data, "number_of_processors", json_object_new_int(24));
    json_object_object_add(post_data, "total_memory_in_gb", json_object_new_double(31.231945037841797));
    json_object_object_add(post_data, "total_hd_in_gb", json_object_new_double(1006.853931427002));
    json_object_object_add(post_data, "total_hd_free_gb", json_object_new_double(1002.3908233642578));
    json_object_object_add(post_data, "version", json_object_new_string("1.24.0823"));
    json_object_object_add(post_data, "domain_name", json_object_new_string("리눅스 그룹"));
    json_object_object_add(post_data, "agent_type", json_object_new_string("linux"));
    json_object_object_add(post_data, "computer_name", json_object_new_string("daba"));
    json_object_object_add(post_data, "os", json_object_new_string("ubuntu"));
    json_object_object_add(post_data, "os_version", json_object_new_string("22.04"));
    json_object_object_add(post_data, "login_id", json_object_new_string("root"));
    json_object_object_add(post_data, "login_group", json_object_new_string("root "));
    json_object_object_add(post_data, "organizational_unit1", json_object_new_string("1"));
    json_object_object_add(post_data, "organizational_unit2", json_object_new_string("2"));
    json_object_object_add(post_data, "organizational_unit3", json_object_new_string("3"));
    json_object_object_add(post_data, "organizational_unit4", json_object_new_string("4"));
    json_object_object_add(post_data, "organizational_unit5", json_object_new_string("5"));
    json_object_object_add(post_data, "user_name", json_object_new_string("daba"));
    json_object_object_add(post_data, "description", json_object_new_string("daba test"));
    json_object_object_add(post_data, "install_date", json_object_new_string("2024-08-02 15:13:31"));
    json_object_object_add(post_data, "random_key", json_object_new_string("no randomkey"));
    json_object_object_add(post_data, "random_key_iv", json_object_new_string(""));

    return post_data;
}

/**
 * @brief TC Connect Login 요청을 수행하는 함수
 * @return json_object* 서버 응답 데이터
 * @details 서버에 로그인 요청을 보내고 응답을 JSON 형태로 반환합니다
 */
struct json_object *postTcConnectLogin(void)
{
    struct json_object *post_data = makeTcConnectLoginData();
    char *result = NULL;
    struct json_object *json_response = NULL;
    struct json_object *final_response = json_object_new_object();

    int isSuccess = post_www("59.6.79.138", 8080, "/tc_connect_login.php", 0,
                             json_object_to_json_string(post_data), &result);

    if (isSuccess == 0)
    {
        printf("POST 요청 성공\n");
        json_response = json_tokener_parse(result);
        if (json_response == NULL)
        {
            json_object_object_add(final_response, "data", json_object_new_string(result));
        }
        else
        {
            json_object_object_add(final_response, "data", json_response);
        }
    }
    else
    {
        printf("POST 요청 실패\n");
        json_object_object_add(final_response, "data", json_object_new_string("request failed"));
    }

    json_object_put(post_data);
    free(result);
    return final_response;
}

/**
 * @brief TC Connect Continue 응답을 분석하는 함수
 * @param result 서버로부터 받은 응답 데이터
 * @return int 응답 분석 결과 코드
 * @retval 0 알 수 없는 응답
 * @retval 1 'ok' 응답
 * @retval 2 환경 설정 명령
 * @retval 3 일반 명령
 */
int switchResultOfTcConnectContinue(struct json_object *result)
{
    struct json_object *data_obj = json_object_object_get(result, "data");

    // data가 단순 문자열인 경우
    if (json_object_get_type(data_obj) == json_type_string)
    {
        const char *data_str = json_object_get_string(data_obj);
        if (strcmp(data_str, "ok") == 0)
        {
            return 1;
        }
        return 0;
    }

    // data가 JSON 객체인 경우
    struct json_object *log_type_obj = json_object_object_get(data_obj, "log_type");
    if (log_type_obj && strcmp(json_object_get_string(log_type_obj), "tc_command") == 0)
    {
        struct json_object *content_obj = json_object_object_get(data_obj, "content");
        if (content_obj)
        {
            struct json_object *command_name_obj = json_object_object_get(content_obj, "command_name");
            if (command_name_obj)
            {
                if (strcmp(json_object_get_string(command_name_obj), "tc_config") == 0)
                {
                    return 2;
                }
                return 3;
            }
        }
    }

    return 0;
}

/**
 * @brief 명령어 이름을 추출하는 함수
 * @param result 서버 응답 데이터
 * @return char* 명령어 이름 문자열
 */
const char *getCommandName(struct json_object *result)
{
    return json_object_get_string(json_object_object_get(json_object_object_get(json_object_object_get(result, "data"), "content"), "command_name"));
}

int processCommand(struct json_object *result)
{
    const char *command_name = getCommandName(result);
    if (strcmp(command_name, "tc_end") == 0)
    {
        return processTcEnd();
    }
    else if (strcmp(command_name, "tc_set_delete") == 0)
    {
        return processTcSetDelete();
    }
    else if (strcmp(command_name, "tc_set_zip") == 0)
    {
        return processTcSetZip();
    }
    else if (strcmp(command_name, "tc_get_zip") == 0)
    {
        return processTcGetZip();
    }
    else if (strcmp(command_name, "tc_getprocesslist") == 0)
    {
        return processTcGetProcessList();
    }
    else if (strcmp(command_name, "tc_cmd") == 0)
    {
        const char *command_parameters = json_object_get_string(json_object_object_get(json_object_object_get(json_object_object_get(result, "data"), "content"), "command_parameters"));
        return processTcCmd(command_parameters);
    }
    else if (strcmp(command_name, "tc_set_user_info") == 0)
    {
        struct json_object *content = json_object_object_get(json_object_object_get(result, "data"), "content");
        struct json_object *params = json_object_new_object();

        // content의 모든 키-값 쌍을 순회
        json_object_object_foreach(content, key, val)
        {
            // command_name을 제외한 나머지 데이터를 복사
            if (strcmp(key, "command_name") != 0)
            {
                json_object_object_add(params, key, json_object_get(val));
            }
        }

        int ret = processTcSetUserInfo(params);
        json_object_put(params);
        return ret;
    }

    return 0;
}

/**
 * @brief 테스트 용 메인 함수
 * @return 프로그램 종료 상태
 * @retval 0 성공
 */
int main(void)
{
    time_t now;
    time(&now);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));

    // struct json_object *result = postTcConnectLogin();
    // printf("postTcConnectLogin 응답: %s\n", json_object_to_json_string(result));
    // json_object_put(result);
    // return 0;

    struct json_object *result2 = postTcConnectContinue();
    printf("postTcConnectContinue 응답: %s\n", json_object_to_json_string(result2));

    int switchResult = switchResultOfTcConnectContinue(result2);
    switch (switchResult)
    {
    case 0:
        printf("모르는 응답\n");
        break;
    case 1:
        printf("ok 수신\n");
        break;
    case 2:
        printf("환경 수신\n");
        break;
    case 3:
        printf("커맨드 수신\n");
        printf("command_name: %s\n", json_object_to_json_string(result2));
        int isSuccess = processCommand(result2);
        if (isSuccess == 1)
        {
            printf("커맨드 처리 실패\n");
        }
        else
        {
            printf("커맨드 처리 성공\n");
        }
        break;
    }

    json_object_put(result2);

    return 0;
}