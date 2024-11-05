/**
 * @file json_builder.h
 * @brief JSON 객체 생성 및 조작을 위한 라이브러리
 */

#define MAX_ITEMS 100          /**< 최대 항목 수 제한 */
#define MAX_STRING_LENGTH 1024 /**< 최대 문자열 길이 제한 */

/**
 * @brief JSON 값의 타입을 나타내는 열거형
 */
typedef enum
{
    JSON_STRING, /**< 문자열 타입 */
    JSON_NUMBER, /**< 숫자 타입 */
    JSON_BOOL,   /**< 불리언 타입 */
    JSON_NULL,   /**< NULL 타입 */
    JSON_OBJECT, /**< 객체 타입 */
    JSON_ARRAY   /**< 배열 타입 */
} JsonType;

typedef struct JsonValue JsonValue;
typedef struct JsonObject JsonObject;
typedef struct JsonArray JsonArray;

/**
 * @brief JSON 값을 저장하는 구조체
 */
struct JsonValue
{
    JsonType type; /**< JSON 값의 타입 */
    union
    {
        char *string_val;       /**< 문자열 값 */
        double number_val;      /**< 숫자 값 */
        int bool_val;           /**< 불리언 값 */
        JsonObject *object_val; /**< 객체 값 */
        JsonArray *array_val;   /**< 배열 값 */
    };
};

/**
 * @brief JSON 객체를 저장하는 구조체
 */
struct JsonObject
{
    char *keys[MAX_ITEMS];       /**< 키 배열 */
    JsonValue values[MAX_ITEMS]; /**< 값 배열 */
    int size;                    /**< 현재 저장된 항목 수 */
};

/**
 * @brief JSON 배열을 저장하는 구조체
 */
struct JsonArray
{
    JsonValue values[MAX_ITEMS]; /**< 값 배열 */
    int size;                    /**< 현재 저장된 항목 수 */
};

/**
 * @brief 새로운 JSON 객체를 생성합니다.
 * @return 생성된 JsonObject 포인터
 */
JsonObject *json_create_object(void);

/**
 * @brief 새로운 JSON 배열을 생성합니다.
 * @return 생성된 JsonArray 포인터
 */
JsonArray *json_create_array(void);

/**
 * @brief JSON 객체와 관련된 모든 메모리를 해제합니다.
 * @param value 해제할 JSON 객체
 */
void json_free(JsonObject *value);

/**
 * @brief JSON 객체에 문자열 값을 추가합니다.
 * @param obj 대상 JSON 객체
 * @param key 키 문자열
 * @param value 추가할 문자열 값
 */
void json_add_string(JsonObject *obj, const char *key, const char *value);

/**
 * @brief JSON 객체에 숫자 값을 추가합니다.
 * @param obj 대상 JSON 객체
 * @param key 키 문자열
 * @param value 추가할 숫자 값
 */
void json_add_number(JsonObject *obj, const char *key, double value);

/**
 * @brief JSON 객체에 불리언 값을 추가합니다.
 * @param obj 대상 JSON 객체
 * @param key 키 문자열
 * @param value 추가할 불리언 값 (0 또는 1)
 */
void json_add_bool(JsonObject *obj, const char *key, int value);

/**
 * @brief JSON 객체에 null 값을 추가합니다.
 * @param obj 대상 JSON 객체
 * @param key 키 문자열
 */
void json_add_null(JsonObject *obj, const char *key);

/**
 * @brief JSON 객체에 다른 JSON 객체를 추가합니다.
 * @param obj 대상 JSON 객체
 * @param key 키 문자열
 * @param value 추가할 JSON 객체
 */
void json_add_object(JsonObject *obj, const char *key, JsonObject *value);

/**
 * @brief JSON 객체에 JSON 배열을 추가합니다.
 * @param obj 대상 JSON 객체
 * @param key 키 문자열
 * @param value 추가할 JSON 배열
 */
void json_add_array(JsonObject *obj, const char *key, JsonArray *value);

/**
 * @brief JSON 배열에 문자열을 추가합니다.
 * @param arr 대상 JSON 배열
 * @param value 추가할 문자열
 */
void json_array_add_string(JsonArray *arr, const char *value);

/**
 * @brief JSON 배열에 숫자를 추가합니다.
 * @param arr 대상 JSON 배열
 * @param value 추가할 숫자
 */
void json_array_add_number(JsonArray *arr, double value);

/**
 * @brief JSON 배열에 불리언 값을 추가합니다.
 * @param arr 대상 JSON 배열
 * @param value 추가할 불리언 값 (0 또는 1)
 */
void json_array_add_bool(JsonArray *arr, int value);

/**
 * @brief JSON 배열에 null 값을 추가합니다.
 * @param arr 대상 JSON 배열
 */
void json_array_add_null(JsonArray *arr);

/**
 * @brief JSON 배열에 JSON 객체를 추가합니다.
 * @param arr 대상 JSON 배열
 * @param value 추가할 JSON 객체
 */
void json_array_add_object(JsonArray *arr, JsonObject *value);

/**
 * @brief JSON 객체를 문자열로 직렬화합니다.
 * @param value 직렬화할 JSON 객체
 * @return 생성된 JSON 문자열 (free 필요)
 */
char *json_stringify(JsonObject *value);

/**
 * @brief JSON 문자열을 해제합니다.
 * @param json_str 해제할 JSON 문자열
 */
void json_free_string(char *json_str);

/**
 * @brief JSON 배열을 해제합니다.
 * @param arr 해제할 JSON 배열
 */
void json_free_array(JsonArray *arr);

/**
 * @brief JSON 배열을 문자열로 직렬화합니다.
 * @param arr 직렬화할 JSON 배열
 * @return 생성된 JSON 문자열 (free 필요)
 */
char *json_serialize_array(JsonArray *arr);

/**
 * @brief JSON 객체의 문자열 값을 수정합니다.
 * @param obj 대상 JSON 객체
 * @param key 수정할 키
 * @param new_value 새로운 문자열 값
 */
void json_update_string(JsonObject *obj, const char *key, const char *new_value);

/**
 * @brief JSON 객체의 숫자 값을 수정합니다.
 * @param obj 대상 JSON 객체
 * @param key 수정할 키
 * @param new_value 새로운 숫자 값
 */
void json_update_number(JsonObject *obj, const char *key, double new_value);

/**
 * @brief JSON 객체의 불리언 값을 수정합니다.
 * @param obj 대상 JSON 객체
 * @param key 수정할 키
 * @param new_value 새로운 불리언 값
 */
void json_update_bool(JsonObject *obj, const char *key, int new_value);

/**
 * @brief JSON 객체의 값을 null로 수정합니다.
 * @param obj 대상 JSON 객체
 * @param key 수정할 키
 */
void json_update_null(JsonObject *obj, const char *key);

/**
 * @brief JSON 객체의 객체 값을 수정합니다.
 * @param obj 대상 JSON 객체
 * @param key 수정할 키
 * @param new_value 새로운 JSON 객체
 */
void json_update_object(JsonObject *obj, const char *key, JsonObject *new_value);

/**
 * @brief JSON 객체의 배열 값을 수정합니다.
 * @param obj 대상 JSON 객체
 * @param key 수정할 키
 * @param new_value 새로운 JSON 배열
 */
void json_update_array(JsonObject *obj, const char *key, JsonArray *new_value);

/**
 * @brief JSON 객체에서 특정 키의 존재 여부를 확인합니다.
 * @param obj 검사할 JSON 객체
 * @param key 검사할 키
 * @return 키가 존재하면 1, 존재하지 않으면 0
 */
int json_has_key(JsonObject *obj, const char *key);