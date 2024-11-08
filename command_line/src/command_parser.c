#include "command_parser.h"
#include "../include/commands.h"
#include <string.h>
#include <stdio.h>

CommandResult process_single_command(int *current_arg, int argc, char *argv[])
{
    if (*current_arg >= argc)
        return ERROR_INVALID_ARGS;

    const char *command = argv[*current_arg];

    // 설치 관련 명령어
    if (strcmp(command, "--delete-logs-and-restart") == 0)
    {
        return delete_logs_and_restart();
    }
    if (strcmp(command, "--install") == 0)
    {
        return install_without_user_input();
    }
    // ... 다른 명령어들도 비슷한 방식으로 구현

    printf("Unknown command: %s\n", command);
    return ERROR_INVALID_ARGS;
}