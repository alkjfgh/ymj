#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include "../include/types.h"

CommandResult process_single_command(int *current_arg, int argc, char *argv[]);

#endif