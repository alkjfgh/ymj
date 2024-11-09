#include "command_parser.h"
#include "../include/commands.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h> 

static CommandResult handle_command_with_arg(int *current_arg, int argc, char *argv[], 
                                           const char *error_msg, 
                                           CommandResult (*handler)(const char*))
{
    if (*current_arg + 1 >= argc)
    {
        printf("Error: %s\n", error_msg);
        return ERROR_INVALID_ARGS;
    }
    (*current_arg)++;
    return handler(argv[*current_arg]);
}

CommandResult process_single_command(int *current_arg, int argc, char *argv[])
{
    if (*current_arg >= argc)
        return ERROR_INVALID_ARGS;

    const char *option = argv[*current_arg];
    CommandResult result = SUCCESS;

    // 도움말 명령어
    if (strcmp(option, "-h") == 0)
    {
        display_help(true);
        return SUCCESS;
    }
    if (strcmp(option, "/?") == 0)
    {
        display_help(false);
        return SUCCESS;
    }

    // 기본 명령어
    if (strcmp(option, "-D") == 0)
    {
        result = delete_logs_and_restart();
    }
    else if (strcmp(option, "-0") == 0)
    {
        result = install_without_user_input();
    }
    else if (strcmp(option, "-1") == 0)
    {
        result = install_with_user_input();
    }
    else if (strcmp(option, "-i") == 0)
    {
        result = handle_command_with_arg(current_arg, argc, argv,
                                       "Server URL required for -i",
                                       connect_to_server);
    }
    else if (strcmp(option, "-a") == 0)
    {
        result = handle_command_with_arg(current_arg, argc, argv,
                                       "Path required for -a",
                                       add_to_whitelist);
    }
    else if (strcmp(option, "-ah") == 0)
    {
        result = handle_command_with_arg(current_arg, argc, argv,
                                       "Path required for -ah",
                                       add_hash_to_whitelist);
    }
    else if (strcmp(option, "-d") == 0)
    {
        result = handle_command_with_arg(current_arg, argc, argv,
                                       "Path required for -d",
                                       delete_from_whitelist);
    }
    else if (strcmp(option, "-dh") == 0)
    {
        result = handle_command_with_arg(current_arg, argc, argv,
                                       "Hash required for -dh",
                                       delete_hash_from_whitelist);
    }
    else if (strncmp(option, "0x", 2) == 0)
    {
        int level = (int)strtol(option + 2, NULL, 16);
        result = set_log_level(level);
    }
    else if (strcmp(option, "-e") == 0)
    {
        result = handle_command_with_arg(current_arg, argc, argv,
                                       "Filename required for -e",
                                       simulate_log);
    }
    else if (strcmp(option, "-s") == 0)
    {
        result = handle_command_with_arg(current_arg, argc, argv,
                                       "Filename required for -s",
                                       simulate_operation);
    }
    else if (strcmp(option, "-x") == 0)
    {
        if (*current_arg + 1 < argc)
        {
            (*current_arg)++;
            result = exit_with_password(argv[*current_arg]);
        }
        else
        {
            result = set_exit_password();
        }
    }
    else if (strcmp(option, "-u") == 0)
    {
        if (*current_arg + 1 < argc)
        {
            (*current_arg)++;
            result = uninstall_with_password(argv[*current_arg]);
        }
        else
        {
            result = set_uninstall_password();
        }
    }
    else if (strcmp(option, "-r") == 0)
    {
        result = delete_operation_logs();
    }
    else if (strcmp(option, "-t") == 0)
    {
        result = handle_command_with_arg(current_arg, argc, argv,
                                       "Configuration required for -t",
                                       test_environment);
    }
    else if (strcmp(option, "-S") == 0)
    {
        result = register_selinux();
    }
    else if (strncmp(option, "-test", 5) == 0)
    {
        const char *test_type = option + 5;
        if (strcmp(test_type, "0") == 0)
        {
            result = run_test(0, 1);
        }
        else if (strcmp(test_type, "1") == 0)
        {
            result = run_test(1, 1);
        }
        else if (strcmp(test_type, "2") == 0)
        {
            result = run_test(2, 1);
        }
        else if (test_type[0] == 'n')
        {
            if (*current_arg + 1 >= argc)
            {
                printf("Error: Repeat count required for -testn\n");
                return ERROR_INVALID_ARGS;
            }
            (*current_arg)++;
            int repeat_count = atoi(argv[*current_arg]);
            result = run_test(0, repeat_count);
        }
    }
    else
    {
        printf("Unknown command: %s\n", option);
        return ERROR_INVALID_ARGS;
    }

    return result;
}