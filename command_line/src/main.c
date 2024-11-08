#include <stdio.h>
#include "../include/commands.h"
#include "command_parser.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: trueep.exec [options]...\n");
        return 1;
    }

    int arg_index = 1;
    int error_count = 0;
    int success_count = 0;

    while (arg_index < argc)
    {
        CommandResult result = process_single_command(&arg_index, argc, argv);

        if (result == SUCCESS)
        {
            success_count++;
        }
        else
        {
            error_count++;
            printf("Command failed: %s\n", argv[arg_index]);
        }

        arg_index++;
    }

    printf("\nExecution Summary:\n");
    printf("Total commands executed: %d\n", success_count + error_count);
    printf("Successful: %d\n", success_count);
    printf("Failed: %d\n", error_count);

    return (error_count > 0) ? 1 : 0;
}