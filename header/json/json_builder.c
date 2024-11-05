#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json_builder.h"

// 구현 예시
JsonObject *json_create_object(void)
{
    JsonObject *obj = malloc(sizeof(JsonObject));
    obj->size = 0;
    return obj;
}

void json_add_string(JsonObject *obj, const char *key, const char *value)
{
    if (obj->size >= MAX_ITEMS)
        return;

    obj->keys[obj->size] = strdup(key);
    obj->values[obj->size].type = JSON_STRING;
    obj->values[obj->size].string_val = strdup(value);
    obj->size++;
}

void json_add_number(JsonObject *obj, const char *key, double value)
{
    if (obj->size >= MAX_ITEMS)
        return;

    obj->keys[obj->size] = strdup(key);
    obj->values[obj->size].type = JSON_NUMBER;
    obj->values[obj->size].number_val = value;
    obj->size++;
}

void json_add_boolean(JsonObject *obj, const char *key, int value)
{
    if (obj->size >= MAX_ITEMS)
        return;

    obj->keys[obj->size] = strdup(key);
    obj->values[obj->size].type = JSON_BOOL;
    obj->values[obj->size].bool_val = value;
    obj->size++;
}

void json_free(JsonValue *value)
{
    JsonObject *obj = (JsonObject *)value; // 타입 캐스팅 추가
    for (int i = 0; i < obj->size; i++)
    {
        free(obj->keys[i]);
        switch (obj->values[i].type)
        {
        case JSON_STRING:
            free(obj->values[i].string_val);
            break;
        case JSON_OBJECT:
            json_free((JsonValue *)obj->values[i].object_val); // 타입 캐스팅 추가
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
            json_free((JsonValue *)arr->values[i].object_val); // 타입 캐스팅 추가
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

JsonArray *json_create_array(void)
{
    JsonArray *arr = malloc(sizeof(JsonArray));
    arr->size = 0;
    return arr;
}

void json_add_object(JsonObject *obj, const char *key, JsonObject *value)
{
    if (obj->size >= MAX_ITEMS)
        return;

    obj->keys[obj->size] = strdup(key);
    obj->values[obj->size].type = JSON_OBJECT;
    obj->values[obj->size].object_val = value;
    obj->size++;
}

void json_add_array(JsonObject *obj, const char *key, JsonArray *value)
{
    if (obj->size >= MAX_ITEMS)
        return;

    obj->keys[obj->size] = strdup(key);
    obj->values[obj->size].type = JSON_ARRAY;
    obj->values[obj->size].array_val = value;
    obj->size++;
}

void json_add_null(JsonObject *obj, const char *key)
{
    if (obj->size >= MAX_ITEMS)
        return;

    obj->keys[obj->size] = strdup(key);
    obj->values[obj->size].type = JSON_NULL;
    obj->size++;
}

void json_array_add_string(JsonArray *arr, const char *value)
{
    if (arr->size >= MAX_ITEMS)
        return;

    arr->values[arr->size].type = JSON_STRING;
    arr->values[arr->size].string_val = strdup(value);
    arr->size++;
}

void json_array_add_number(JsonArray *arr, double value)
{
    if (arr->size >= MAX_ITEMS)
        return;

    arr->values[arr->size].type = JSON_NUMBER;
    arr->values[arr->size].number_val = value;
    arr->size++;
}

void json_array_add_bool(JsonArray *arr, int value)
{
    if (arr->size >= MAX_ITEMS)
        return;

    arr->values[arr->size].type = JSON_BOOL;
    arr->values[arr->size].bool_val = value;
    arr->size++;
}

void json_array_add_null(JsonArray *arr)
{
    if (arr->size >= MAX_ITEMS)
        return;

    arr->values[arr->size].type = JSON_NULL;
    arr->size++;
}

void json_array_add_object(JsonArray *arr, JsonObject *value)
{
    if (arr->size >= MAX_ITEMS)
        return;

    arr->values[arr->size].type = JSON_OBJECT;
    arr->values[arr->size].object_val = value;
    arr->size++;
}

char *json_stringify(JsonValue *value)
{
    JsonObject *obj = (JsonObject *)value;
    return json_serialize(obj);
}