#include "processCommand.h"
#include "sendCommandResponse.h"
#include <json-c/json.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief 에이전트를 종료하는 함수
 * @return 성공 시 0, 실패 시 1을 반환
 * @details TC 종료 처리를 수행하며, ps 명령어로 에이전트 프로세스 존재 여부를 확인할 수 있음
 */
int processTcEnd(void)
{
    // TC 종료 처리 로직
    // 검증: ps로 에이전트 프로세스 존재 확인
    return 0;
}

/**
 * @brief 에이전트를 삭제하는 함수
 * @return 성공 시 0, 실패 시 1을 반환
 * @details 에이전트 삭제 처리를 수행하며, ls -l -a 명령어로 에이전트와 설치 폴더 존재 여부를 확인할 수 있음
 */
int processTcSetDelete(void)
{
    // 삭제 처리 로직
    // 검증: ls -l -a 로 에이전트와 설치 폴더 존재 확인
    return 0;
}

/**
 * @brief 에이전트의 config 폴더를 압축하여 서버로 전송하는 함수
 * @return 성공 시 0, 실패 시 1을 반환
 * @details 압축 파일명은 user_id + .tar.gz 형식으로 생성됨
 */
int processTcSetZip(void)
{
    // ZIP 처리 로직
    // 검증: 서버에서 수신된 파일이 사용자별 구분되고 파일의 압축을 풀어 확인한다.
    return 0;
}

/**
 * @brief config 폴더의 정보를 서버로 전송하는 함수
 * @return 성공 시 0, 실패 시 1을 반환
 * @details 폴더 개수, 파일 개수, 전체 용량을 tc_command_res.php로 전송
 */
int processTcGetZip(void)
{
    // ZIP 정보 가져오기
    struct json_object *zipData = makeGetZipData();
    struct json_object *postData = makePostData(zipData, "tc_get_zip_res");

    // 서버에 데이터 전송
    struct json_object *response = sendCommandResponse(postData);

    // 서버에 데이터 전송 실패
    if (response == NULL)
    {
        return 1;
    }

    // 데이터 해제
    json_object_put(response);
    json_object_put(postData);
    json_object_put(zipData);

    // 검증방법 : 서버에 수시된 데이터와 ls로 해당 폴더의 정보가 일치하는지 확인한다.
    return 0;
}

/**
 * @brief 실행 중인 모든 프로세스 정보를 서버로 전송하는 함수
 * @return 성공 시 0, 실패 시 1을 반환
 * @details ps -ef 명령어로 프로세스 정보를 확인할 수 있음
 */
int processTcGetProcessList(void)
{
    // 프로세스 리스트 가져오기 처리 로직
    struct json_object *processListData = makeGetProcessListData();
    struct json_object *postData = makePostData(processListData, "tc_getprocesslist_res");

    // 서버에 데이터 전송
    struct json_object *response = sendCommandResponse(postData);

    // 서버에 데이터 전송 실패
    if (response == NULL)
    {
        return 1;
    }

    // 데이터 해제
    json_object_put(response);
    json_object_put(postData);
    json_object_put(processListData);

    // 검증방법 : ps -ef 로 확인한다.
    return 0;
}

/**
 * @brief 전달받은 shell 명령어를 실행하고 결과를 서버로 전송하는 함수
 * @param command_parameters 실행할 shell 명령어
 * @return 성공 시 0, 실패 시 1을 반환
 * @details 명령어 실행 실패 시 실패 사유를 서버로 전송
 */
int processTcCmd(const char *command_parameters)
{
    if (command_parameters == NULL)
    {
        return 1;
    }

    // 명령어 실행 처리 로직
    struct json_object *cmdData = makeCmdData();
    struct json_object *postData = makePostData(cmdData, "tc_cmd_res");
    json_object_object_add(postData, "command_parameters", json_object_new_string(command_parameters)); // 명령어 파라미터

    // 서버에 데이터 전송
    struct json_object *response = sendCommandResponse(postData);

    // 서버에 데이터 전송 실패
    if (response == NULL)
    {
        return 1;
    }

    // 데이터 해제
    json_object_put(response);
    json_object_put(postData);
    json_object_put(cmdData);

    // 검증방법 : 서버에 수신된 자료와 command_parameters의 shell을 직접 로컬에서 실행한 결과가 동일 한지 확인한다.

    return 0;
}

/**
 * @brief 사용자 정보를 수정하는 함수
 * @param params JSON 형식의 사용자 정보 파라미터
 * @return 성공 시 0, 실패 시 1을 반환
 * @details 에이전트 재시작 후 서버에서 변경된 로그인 정보 확인 가능
 */
int processTcSetUserInfo(struct json_object *params)
{
    if (params == NULL)
    {
        return 1;
    }

    // 사용자 정보 설정 처리 로직
    // 검증방법 : 에이전트 재식작후 서버에 사용자 로그인 정보가 변경 되었는지 확인한다.

    return 0;
}