#define MAX_ITEMS 100
#define MAX_STRING_LENGTH 1024

typedef enum
{
    JSON_STRING,
    JSON_NUMBER,
    JSON_BOOL,
    JSON_NULL,
    JSON_OBJECT,
    JSON_ARRAY
} JsonType;

typedef struct JsonValue JsonValue;
typedef struct JsonObject JsonObject;
typedef struct JsonArray JsonArray;

struct JsonValue
{
    JsonType type;
    union
    {
        char *string_val;
        double number_val;
        int bool_val;
        JsonObject *object_val;
        JsonArray *array_val;
    };
};

struct JsonObject
{
    char *keys[MAX_ITEMS];
    JsonValue values[MAX_ITEMS];
    int size;
};

struct JsonArray
{
    JsonValue values[MAX_ITEMS];
    int size;
};

// 객체 생성/해제 함수
JsonObject *json_create_object(void);
JsonArray *json_create_array(void);
void json_free(JsonValue *value);

// 값 추가 함수
void json_add_string(JsonObject *obj, const char *key, const char *value);
void json_add_number(JsonObject *obj, const char *key, double value);
void json_add_bool(JsonObject *obj, const char *key, int value);
void json_add_null(JsonObject *obj, const char *key);
void json_add_object(JsonObject *obj, const char *key, JsonObject *value);
void json_add_array(JsonObject *obj, const char *key, JsonArray *value);

// 배열 관련 함수
void json_array_add_string(JsonArray *arr, const char *value);
void json_array_add_number(JsonArray *arr, double value);
void json_array_add_bool(JsonArray *arr, int value);
void json_array_add_null(JsonArray *arr);
void json_array_add_object(JsonArray *arr, JsonObject *value);

// JSON 문자열 생성/해제 함수
char *json_stringify(JsonValue *value);
void json_free_string(char *json_str);

void json_free_array(JsonArray *arr);
char *json_serialize_array(JsonArray *arr);