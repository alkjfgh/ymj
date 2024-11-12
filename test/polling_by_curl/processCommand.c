#include "processCommand.h"
#include <json-c/json.h>

int processTcEnd(void)
{
    // TC 종료 처리 로직
    return 0;
}

int processTcSetDelete(void)
{
    // 삭제 설정 처리 로직
    return 0;
}

int processTcSetZip(void)
{
    // ZIP 설정 처리 로직
    return 0;
}

int processTcGetZip(void)
{
    // ZIP 정보 가져오기 처리 로직
    return 0;
}

int processTcGetProcessList(void)
{
    // 프로세스 리스트 가져오기 처리 로직
    return 0;
}

int processTcCmd(const char *command_parameters)
{
    // 명령어 실행 처리 로직
    if (command_parameters == NULL)
    {
        return 1;
    }
    return 0;
}

int processTcSetUserInfo(struct json_object *params)
{
    // 사용자 정보 설정 처리 로직
    if (params == NULL)
    {
        return 1;
    }
    return 0;
}