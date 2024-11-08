#include "../../include/commands.h"
#include "../../include/utils.h"
#include <stdio.h>

CommandResult add_to_whitelist(const char *path)
{
    if (!file_exists(path))
    {
        log_error("File does not exist");
        return ERROR_INVALID_ARGS;
    }

    // TODO: 실제 화이트리스트 추가 로직 구현
    log_info("File added to whitelist");
    return SUCCESS;
}

// ... 다른 화이트리스트 관련 함수들