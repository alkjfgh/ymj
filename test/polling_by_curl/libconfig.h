/*
  config.txt 파일 제어(json)
*/
#ifndef LIBCONFIG_H
#define LIBCONFIG_H

#include <json-c/json.h>
#include <stdio.h>
#include <stdlib.h>

struct json_object * config_load_file(const char *filename);
int config_save_file(const char *filename, struct json_object *json_obj);

//int config_set_key_value(struct json_object *json_obj, const char *key, const char *value);
//int config_get_key_value(struct json_object *json_obj, const char *key, char *value, int value_size);
int config_get_key_value_section(struct json_object *json_obj, const char *key, char *value, int value_size, const char *section_key);


/*
int config_add_key_value(struct json_object *json_obj, const char *key, const char *value);
int config_del_key_value(struct json_object *json_obj, const char *key);
*/
int config_free_json(struct json_object *json_obj);


#endif // LIBCONFIG_H