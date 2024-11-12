#include <stdio.h>
#include <json-c/json.h>
#include <signal.h>
#include <pthread.h>

#include "globaldata.h"



struct json_object* g_rules = NULL; //룰 json  
 
struct json_object* g_config = NULL; //config.json

struct json_object* g_tc_server = NULL; //서버로 부터 받은 정보 (tc_server.php)

// g_config 를 접근하기 위한 뮤텍스 
pthread_mutex_t g_mutexConfig = PTHREAD_MUTEX_INITIALIZER; //뮤텍스
