#ifndef PROCESS_COMMAND_H
#define PROCESS_COMMAND_H

#include <json-c/json.h>
int processTcEnd(void);
int processTcSetDelete(void);
int processTcSetZip(void);
int processTcGetZip(void);
int processTcGetProcessList(void);
int processTcCmd(const char *command_parameters);
int processTcSetUserInfo(struct json_object *params);

#endif