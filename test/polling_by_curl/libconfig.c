#include <json-c/json.h>
#include <stdio.h>
#include <stdlib.h>

#include "libconfig.h"

/*
기능: JSON 파일을 읽어서 json_object 구조체로 반환한다.
인자: 파일 이름
리턴: json_object 구조체 포인터(성공), NULL(실패)
*주의: 사용 후 json_object_put() 함수로 메모리를 해제해야 한다.
*/
struct json_object * config_load_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open config file");
        return NULL;
    }

    // 파일 크기 계산
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // 파일 내용 읽기
    char *buffer = malloc(file_size + 1);
    if (buffer == NULL) {
        perror("Unable to allocate memory");
        fclose(file);
        return NULL;
    }

    //fread(buffer, 1, file_size, file);
    size_t bytes_read = fread(buffer, 1, file_size, file);
    if (bytes_read != file_size) {
        // 오류 처리: 파일에서 예상한 크기만큼 읽어오지 못한 경우
        fprintf(stderr, "파일을 읽는 데 실패했습니다.\n");
        // 필요에 따라 추가적인 오류 처리를 수행합니다.
        fclose(file);
        free(buffer);
        return NULL;
    }

    buffer[file_size] = '\0';
    fclose(file);

    // JSON 파싱
    struct json_object *parsed_json;
    parsed_json = json_tokener_parse(buffer);
    if (parsed_json == NULL) {
        fprintf(stderr, "Error parsing JSON data\n");
        free(buffer);
        return NULL;
    }
    free(buffer);   

    return parsed_json;
    
}
/*
기능: json_object 구조체를 파일에 저장한다.
인자: 파일 이름, json_object 구조체
리턴: 성공(0), 실패(-1)
*/
// save json-c object to file
int config_save_file(const char *filename, struct json_object *json_obj) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Unable to open config file");
        return -1;
    }

    const char *json_str = json_object_to_json_string_ext(json_obj, JSON_C_TO_STRING_PRETTY);

    fprintf(file, "%s\n", json_str);
    fclose(file);

    return 0;
}
/*
기능: json_object 구조체에 key-value 쌍을 추가한다.
인자: json_object 구조체, key, value
리턴: 성공(0), 실패(-1)
*/
/*
int config_set_key_value(struct json_object *json_obj, const char *key, const char *value) {
    struct json_object *new_value = json_object_new_string(value);
    if (new_value == NULL) {
        fprintf(stderr, "Error creating JSON string\n");
        return -1;
    }

    json_object_object_add(json_obj, key, new_value);

    return 0;
}
*/
/*
긴능: json_object 구조체에서 key에 해당하는 value를 가져온다.
인자: json_object 구조체, key, value 버퍼, value 버퍼 크기(사용하지 않음,안전용)
리턴: 성공(0), 실패(-1)
*/
int config_get_key_value(struct json_object *json_obj, const char *key, char *value, int value_size) {
    struct json_object *value_obj = json_object_object_get(json_obj, key);
    if (value_obj == NULL) {
        fprintf(stderr, "111 Error getting JSON value\n");
        return -1;
    }

    const char *value_str = json_object_get_string(value_obj);
    if (value_str == NULL) {
        fprintf(stderr, "Error getting JSON string\n");
        return -1;
    }

    snprintf(value, value_size, "%s", value_str);

    return 0;
}

/*
긴능: json_object 구조체에서 section_key에 있는 key에 해당하는 value를 가져온다.
인자: json_object 구조체, key, value 버퍼, value 버퍼 크기(사용하지 않음,안전용)
리턴: 성공(0), 실패(음수)
*/
int config_get_key_value_section(struct json_object *json_obj, const char *key, char *value, int value_size, const char *section_key) {
    // json_object_object_get(json_obj, section_key);
    struct json_object *section_obj = json_object_object_get(json_obj, section_key);
    if (section_obj == NULL) {
        fprintf(stderr, "Error getting JSON section key:%s ,%s \n",key,section_key);
        return -1;
    }

    struct json_object *value_obj = json_object_object_get(section_obj, key);
    if (value_obj == NULL) {
        fprintf(stderr, "Error getting JSON value:<<<%s,%s >>>\n",key,section_key);
        return -2;
    }

    const char *value_str = json_object_get_string(value_obj);
    if (value_str == NULL) {
        fprintf(stderr, "Error getting JSON string\n");
        return -3;
    }

    snprintf(value, value_size, "%s", value_str);

    return 0;
}


/*
기능: json_object 구조체에 key-value 쌍을 새로 추가한다.
인자: json_object 구조체, key, value
리턴: 성공(0), 실패(-1)
*/
int config_add_key_value(struct json_object *json_obj, const char *key, const char *value) {
    struct json_object *new_value = json_object_new_string(value);
    if (new_value == NULL) {
        fprintf(stderr, "Error creating JSON string\n");
        return -1;
    }

    json_object_object_add(json_obj, key, new_value);

    return 0;
}



/*
기능: json_object 구조체에서 key에 해당하는 value를 삭제한다.
인자: json_object 구조체, key
리턴: 성공(0), 실패(-1)
*/
int config_del_key_value(struct json_object *json_obj, const char *key) {
    json_object_object_del(json_obj, key);

    return 0;
}
/*
기능: json_object 구조체를 해제한다.
인자: json_object 구조체
리턴: 성공(0), 실패(-1)
*/
int config_free_json(struct json_object *json_obj) {
    json_object_put(json_obj);

    return 0;
}
/*
컴파일을 위해 필요한 라이브러리 설치 명령어 및 컴파일 명령어:

1. json-c 라이브러리 설치:
   Ubuntu:
   ```
   sudo apt-get update
   sudo apt-get install libjson-c-dev
   ```

2. 프로그램 컴파일 명령어:
   ```
   gcc -o lab_config lab_config.c -ljson-c -static
   ```
*/
///////////////////////////////

/*
int main_demo ()
{
    struct json_object *parsed_json = config_load_file("config.txt");
    if(parsed_json == NULL) {
        return 1;
    }

    char value[256];
    config_get_key_value(parsed_json, "engine", value, sizeof(value));
    printf("engine: %s\n", value);

    
    // add new key-value pair
    config_add_key_value(parsed_json, "agent_name", "clcc-agent-1");

    // json_object_object_add(parsed_json, "agent_name", json_object_new_string("clcc-agent-1"));
    config_set_key_value(parsed_json, "engine", "monitoring");

    config_save_file("config1.txt", parsed_json);


    // memory free
    config_free_json(parsed_json);

    return 0;
}
*/