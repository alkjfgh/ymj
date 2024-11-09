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

CommandResult add_hash_to_whitelist(const char *path) {
    // 구현 필요
    return SUCCESS;
}

CommandResult delete_from_whitelist(const char *path) {
    // 구현 필요
    return SUCCESS;
}

CommandResult delete_hash_from_whitelist(const char *hash) {
    // 구현 필요
    return SUCCESS;
}