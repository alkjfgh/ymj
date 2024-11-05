/**
 * @file json_builder.c
 * @brief JSON 객체 생성 및 조작을 위한 구현 파일
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json_builder.h"

/**
 * @brief 새로운 JSON 객체를 생성합니다.
 * @return 새로 생성된 JsonObject 포인터
 */
JsonObject *json_create_object(void)
{
    JsonObject *obj = malloc(sizeof(JsonObject));
    obj->size = 0;
    return obj;
}

/**
 * @brief JSON 객체에 문자열 값을 추가합니다.
 * @param obj 대상 JSON 객체
 * @param key 키 문자열
 * @param value 값 문자열
 */
void json_add_string(JsonObject *obj, const char *key, const char *value)
{
    if (obj->size >= MAX_ITEMS)
        return;

    obj->keys[obj->size] = strdup(key);
    obj->values[obj->size].type = JSON_STRING;
    obj->values[obj->size].string_val = strdup(value);
    obj->size++;
}

/**
 * @brief JSON 객체에 숫자 값을 추가합니다.
 * @param obj 대상 JSON 객체
 * @param key 키 문자열
 * @param value 숫자 값
 */
void json_add_number(JsonObject *obj, const char *key, double value)
{
    if (obj->size >= MAX_ITEMS)
        return;

    obj->keys[obj->size] = strdup(key);
    obj->values[obj->size].type = JSON_NUMBER;
    obj->values[obj->size].number_val = value;
    obj->size++;
}

/**
 * @brief JSON 객체에 불리언 값을 추가합니다.
 * @param obj 대상 JSON 객체
 * @param key 키 문자열
 * @param value 불리언 값 (0 또는 1)
 */
void json_add_boolean(JsonObject *obj, const char *key, int value)
{
    if (obj->size >= MAX_ITEMS)
        return;

    obj->keys[obj->size] = strdup(key);
    obj->values[obj->size].type = JSON_BOOL;
    obj->values[obj->size].bool_val = value;
    obj->size++;
}

/**
 * @brief JSON 객체의 메모리를 해제합니다.
 * @param obj 해제할 JSON 객체
 */
void json_free(JsonObject *obj)
{
    for (int i = 0; i < obj->size; i++)
    {
        free(obj->keys[i]);
        switch (obj->values[i].type)
        {
        case JSON_STRING:
            free(obj->values[i].string_val);
            break;
        case JSON_OBJECT:
            json_free(obj->values[i].object_val); // 타입 캐스팅 추가
            break;
        case JSON_ARRAY:
            json_free_array(obj->values[i].array_val);
            break;
        default:
            break;
        }
    }
    free(obj);
}

/**
 * @brief JSON 배열의 메모리를 해제합니다.
 * @param arr 해제할 JSON 배열
 */
void json_free_array(JsonArray *arr)
{
    for (int i = 0; i < arr->size; i++)
    {
        switch (arr->values[i].type)
        {
        case JSON_STRING:
            free(arr->values[i].string_val);
            break;
        case JSON_OBJECT:
            json_free(arr->values[i].object_val); // 타입 캐스팅 추가
            break;
        case JSON_ARRAY:
            json_free_array(arr->values[i].array_val);
            break;
        default:
            break;
        }
    }
    free(arr);
}

/**
 * @brief JSON 객체를 문자열로 직렬화합니다.
 * @param obj 직렬화할 JSON 객체
 * @return 직렬화된 JSON 문자열
 */
char *json_serialize(JsonObject *obj)
{
    char *buffer = malloc(4096);
    char temp[1024];
    buffer[0] = '{';
    buffer[1] = '\0';

    for (int i = 0; i < obj->size; i++)
    {
        if (i > 0)
        {
            strcat(buffer, ",");
        }

        sprintf(temp, "\"%s\":", obj->keys[i]);
        strcat(buffer, temp);

        switch (obj->values[i].type)
        {
        case JSON_STRING:
            sprintf(temp, "\"%s\"", obj->values[i].string_val);
            break;
        case JSON_NUMBER:
            sprintf(temp, "%.2f", obj->values[i].number_val);
            break;
        case JSON_BOOL:
            sprintf(temp, "%s", obj->values[i].bool_val ? "true" : "false");
            break;
        case JSON_NULL:
            sprintf(temp, "null");
            break;
        case JSON_OBJECT:
        {
            char *nested = json_serialize(obj->values[i].object_val);
            strcpy(temp, nested);
            free(nested);
        }
        break;
        case JSON_ARRAY:
        {
            char *nested = json_serialize_array(obj->values[i].array_val);
            strcpy(temp, nested);
            free(nested);
        }
        break;
        }
        strcat(buffer, temp);
    }

    strcat(buffer, "}");
    return buffer;
}

/**
 * @brief JSON 배열을 문자열로 직렬화합니다.
 * @param arr 직렬화할 JSON 배열
 * @return 직렬화된 JSON 배열 문자열
 */
char *json_serialize_array(JsonArray *arr)
{
    char *buffer = malloc(4096);
    char temp[1024];
    buffer[0] = '[';
    buffer[1] = '\0';

    for (int i = 0; i < arr->size; i++)
    {
        if (i > 0)
        {
            strcat(buffer, ",");
        }

        switch (arr->values[i].type)
        {
        case JSON_STRING:
            sprintf(temp, "\"%s\"", arr->values[i].string_val);
            break;
        case JSON_NUMBER:
            sprintf(temp, "%.2f", arr->values[i].number_val);
            break;
        case JSON_BOOL:
            sprintf(temp, "%s", arr->values[i].bool_val ? "true" : "false");
            break;
        case JSON_NULL:
            sprintf(temp, "null");
            break;
        case JSON_OBJECT:
        {
            char *nested = json_serialize(arr->values[i].object_val);
            strcpy(temp, nested);
            free(nested);
        }
        break;
        case JSON_ARRAY:
        {
            char *nested = json_serialize_array(arr->values[i].array_val);
            strcpy(temp, nested);
            free(nested);
        }
        break;
        }
        strcat(buffer, temp);
    }

    strcat(buffer, "]");
    return buffer;
}

/**
 * @brief 새로운 JSON 배열을 생성합니다.
 * @return 새로 생성된 JsonArray 포인터
 */
JsonArray *json_create_array(void)
{
    JsonArray *arr = malloc(sizeof(JsonArray));
    arr->size = 0;
    return arr;
}

/**
 * @brief JSON 객체에 JSON 객체를 추가합니다.
 * @param obj 대상 JSON 객체
 * @param key 키 문자열
 * @param value 추가할 JSON 객체
 */
void json_add_object(JsonObject *obj, const char *key, JsonObject *value)
{
    if (obj->size >= MAX_ITEMS)
        return;

    obj->keys[obj->size] = strdup(key);
    obj->values[obj->size].type = JSON_OBJECT;
    obj->values[obj->size].object_val = value;
    obj->size++;
}

/**
 * @brief JSON 객체에 JSON 배열을 추가합니다.
 * @param obj 대상 JSON 객체
 * @param key 키 문자열
 * @param value 추가할 JSON 배열
 */
void json_add_array(JsonObject *obj, const char *key, JsonArray *value)
{
    if (obj->size >= MAX_ITEMS)
        return;

    obj->keys[obj->size] = strdup(key);
    obj->values[obj->size].type = JSON_ARRAY;
    obj->values[obj->size].array_val = value;
    obj->size++;
}

/**
 * @brief JSON 객체에 NULL 값을 추가합니다.
 * @param obj 대상 JSON 객체
 * @param key 키 문자열
 */
void json_add_null(JsonObject *obj, const char *key)
{
    if (obj->size >= MAX_ITEMS)
        return;

    obj->keys[obj->size] = strdup(key);
    obj->values[obj->size].type = JSON_NULL;
    obj->size++;
}

/**
 * @brief JSON 배열에 문자열 값을 추가합니다.
 * @param arr 대상 JSON 배열
 * @param value 추가할 문자열 값
 */
void json_array_add_string(JsonArray *arr, const char *value)
{
    if (arr->size >= MAX_ITEMS)
        return;

    arr->values[arr->size].type = JSON_STRING;
    arr->values[arr->size].string_val = strdup(value);
    arr->size++;
}

/**
 * @brief JSON 배열에 숫자 값을 추가합니다.
 * @param arr 대상 JSON 배열
 * @param value 추가할 숫자 값
 */
void json_array_add_number(JsonArray *arr, double value)
{
    if (arr->size >= MAX_ITEMS)
        return;

    arr->values[arr->size].type = JSON_NUMBER;
    arr->values[arr->size].number_val = value;
    arr->size++;
}

/**
 * @brief JSON 배열에 불리언 값을 추가합니다.
 * @param arr 대상 JSON 배열
 * @param value 추가할 불리언 값
 */
void json_array_add_bool(JsonArray *arr, int value)
{
    if (arr->size >= MAX_ITEMS)
        return;

    arr->values[arr->size].type = JSON_BOOL;
    arr->values[arr->size].bool_val = value;
    arr->size++;
}

/**
 * @brief JSON 배열에 NULL 값을 추가합니다.
 * @param arr 대상 JSON 배열
 */
void json_array_add_null(JsonArray *arr)
{
    if (arr->size >= MAX_ITEMS)
        return;

    arr->values[arr->size].type = JSON_NULL;
    arr->size++;
}

/**
 * @brief JSON 배열에 JSON 객체를 추가합니다.
 * @param arr 대상 JSON 배열
 * @param value 추가할 JSON 객체
 */
void json_array_add_object(JsonArray *arr, JsonObject *value)
{
    if (arr->size >= MAX_ITEMS)
        return;

    arr->values[arr->size].type = JSON_OBJECT;
    arr->values[arr->size].object_val = value;
    arr->size++;
}

/**
 * @brief JSON 객체를 문자열로 직렬화합니다.
 * @param obj 직렬화할 JSON 객체
 * @return 직렬화된 JSON 문자열
 */
char *json_stringify(JsonObject *obj)
{
    return json_serialize(obj);
}

/**
 * @brief JSON 객체에 특정 키가 있는지 확인합니다.
 * @param obj 대상 JSON 객체
 * @param key 확인할 키 문자열
 * @return 키가 있으면 1, 없으면 0
 */
int json_has_key(JsonObject *obj, const char *key)
{
    for (int i = 0; i < obj->size; i++)
    {
        if (strcmp(obj->keys[i], key) == 0)
        {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief JSON 객체에 특정 키의 문자열 값을 업데이트합니다.
 * @param obj 대상 JSON 객체
 * @param key 업데이트할 키 문자열
 * @param new_value 새로운 문자열 값
 */
void json_update_string(JsonObject *obj, const char *key, const char *new_value)
{
    for (int i = 0; i < obj->size; i++)
    {
        if (strcmp(obj->keys[i], key) == 0 && obj->values[i].type == JSON_STRING)
        {
            free(obj->values[i].string_val);
            obj->values[i].string_val = strdup(new_value);
            return;
        }
    }
}

/**
 * @brief JSON 객체에 특정 키의 숫자 값을 업데이트합니다.
 * @param obj 대상 JSON 객체
 * @param key 업데이트할 키 문자열
 * @param new_value 새로운 숫자 값
 */
void json_update_number(JsonObject *obj, const char *key, double new_value)
{
    for (int i = 0; i < obj->size; i++)
    {
        if (strcmp(obj->keys[i], key) == 0 && obj->values[i].type == JSON_NUMBER)
        {
            obj->values[i].number_val = new_value;
            return;
        }
    }
}

/**
 * @brief JSON 객체에 특정 키의 불리언 값을 업데이트합니다.
 * @param obj 대상 JSON 객체
 * @param key 업데이트할 키 문자열
 * @param new_value 새로운 불리언 값
 */
void json_update_bool(JsonObject *obj, const char *key, int new_value)
{
    for (int i = 0; i < obj->size; i++)
    {
        if (strcmp(obj->keys[i], key) == 0 && obj->values[i].type == JSON_BOOL)
        {
            obj->values[i].bool_val = new_value;
            return;
        }
    }
}

/**
 * @brief JSON 객체에 특정 키의 NULL 값을 업데이트합니다.
 * @param obj 대상 JSON 객체
 * @param key 업데이트할 키 문자열
 */
void json_update_null(JsonObject *obj, const char *key)
{
    for (int i = 0; i < obj->size; i++)
    {
        if (strcmp(obj->keys[i], key) == 0)
        {
            switch (obj->values[i].type)
            {
            case JSON_STRING:
                free(obj->values[i].string_val);
                break;
            case JSON_OBJECT:
                json_free(obj->values[i].object_val);
                break;
            case JSON_ARRAY:
                json_free_array(obj->values[i].array_val);
                break;
            }
            obj->values[i].type = JSON_NULL;
            return;
        }
    }
}

/**
 * @brief JSON 객체에 특정 키의 JSON 객체를 업데이트합니다.
 * @param obj 대상 JSON 객체
 * @param key 업데이트할 키 문자열
 * @param new_value 새로운 JSON 객체
 */
void json_update_object(JsonObject *obj, const char *key, JsonObject *new_value)
{
    for (int i = 0; i < obj->size; i++)
    {
        if (strcmp(obj->keys[i], key) == 0 && obj->values[i].type == JSON_OBJECT)
        {
            json_free(obj->values[i].object_val);
            obj->values[i].object_val = new_value;
            return;
        }
    }
}

/**
 * @brief JSON 객체에 특정 키의 JSON 배열을 업데이트합니다.
 * @param obj 대상 JSON 객체
 * @param key 업데이트할 키 문자열
 * @param new_value 새로운 JSON 배열
 */
void json_update_array(JsonObject *obj, const char *key, JsonArray *new_value)
{
    for (int i = 0; i < obj->size; i++)
    {
        if (strcmp(obj->keys[i], key) == 0 && obj->values[i].type == JSON_ARRAY)
        {
            json_free_array(obj->values[i].array_val);
            obj->values[i].array_val = new_value;
            return;
        }
    }
}

/**
 * @brief JSON 객체를 들여쓰기가 포함된 문자열로 직렬화합니다.
 * @param obj 직렬화할 JSON 객체
 * @param indent 들여쓰기 레벨
 * @return 직렬화된 JSON 문자열
 */
char *json_serialize_pretty(JsonObject *obj, int indent)
{
    char *buffer = malloc(8192); // 더 큰 버퍼 할당
    char temp[2048];
    char indent_str[64] = "";

    // 들여쓰기 문자열 생성
    for (int i = 0; i < indent; i++)
    {
        strcat(indent_str, "    "); // 4칸 공백
    }

    strcpy(buffer, "{\n");

    for (int i = 0; i < obj->size; i++)
    {
        strcat(buffer, indent_str);
        strcat(buffer, "    "); // 추가 들여쓰기

        sprintf(temp, "\"%s\": ", obj->keys[i]);
        strcat(buffer, temp);

        switch (obj->values[i].type)
        {
        case JSON_STRING:
            sprintf(temp, "\"%s\"", obj->values[i].string_val);
            break;
        case JSON_NUMBER:
            sprintf(temp, "%.2f", obj->values[i].number_val);
            break;
        case JSON_BOOL:
            sprintf(temp, "%s", obj->values[i].bool_val ? "true" : "false");
            break;
        case JSON_NULL:
            sprintf(temp, "null");
            break;
        case JSON_OBJECT:
        {
            char *nested = json_serialize_pretty(obj->values[i].object_val, indent + 1);
            strcpy(temp, nested);
            free(nested);
            break;
        }
        case JSON_ARRAY:
        {
            char *nested = json_serialize_array_pretty(obj->values[i].array_val, indent + 1);
            strcpy(temp, nested);
            free(nested);
            break;
        }
        }
        strcat(buffer, temp);

        if (i < obj->size - 1)
        {
            strcat(buffer, ",");
        }
        strcat(buffer, "\n");
    }

    strcat(buffer, indent_str);
    strcat(buffer, "}");
    return buffer;
}

/**
 * @brief JSON 배열을 들여쓰기가 포함된 문자열로 직렬화합니다.
 * @param arr 직렬화할 JSON 배열
 * @param indent 들여쓰기 레벨
 * @return 직렬화된 JSON 배열 문자열
 */
char *json_serialize_array_pretty(JsonArray *arr, int indent)
{
    char *buffer = malloc(8192);
    char temp[2048];
    char indent_str[64] = "";

    for (int i = 0; i < indent; i++)
    {
        strcat(indent_str, "    ");
    }

    strcpy(buffer, "[\n");

    for (int i = 0; i < arr->size; i++)
    {
        strcat(buffer, indent_str);
        strcat(buffer, "    ");

        switch (arr->values[i].type)
        {
        case JSON_STRING:
            sprintf(temp, "\"%s\"", arr->values[i].string_val);
            break;
        case JSON_NUMBER:
            sprintf(temp, "%.2f", arr->values[i].number_val);
            break;
        case JSON_BOOL:
            sprintf(temp, "%s", arr->values[i].bool_val ? "true" : "false");
            break;
        case JSON_NULL:
            sprintf(temp, "null");
            break;
        case JSON_OBJECT:
        {
            char *nested = json_serialize_pretty(arr->values[i].object_val, indent + 1);
            strcpy(temp, nested);
            free(nested);
            break;
        }
        case JSON_ARRAY:
        {
            char *nested = json_serialize_array_pretty(arr->values[i].array_val, indent + 1);
            strcpy(temp, nested);
            free(nested);
            break;
        }
        }
        strcat(buffer, temp);

        if (i < arr->size - 1)
        {
            strcat(buffer, ",");
        }
        strcat(buffer, "\n");
    }

    strcat(buffer, indent_str);
    strcat(buffer, "]");
    return buffer;
}

/**
 * @brief JSON 객체를 들여쓰기가 포함된 문자열로 직렬화합니다.
 * @param obj 직렬화할 JSON 객체
 * @return 직렬화된 JSON 문자열
 */
char *json_stringify_pretty(JsonObject *obj)
{
    return json_serialize_pretty(obj, 0);
}