/*
  전역 변수 관리
*/
#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <json-c/json.h>
#include <pthread.h>

// 물리적 파일 이름
#define CONFIG_FILE "config.json"  // 설정 파일
#define RULES_FILE "rules.json"    // 룰 파일
#define EVENT_LOG_FILE "event.log" // 이벤트 로그 파일

// 룰 파일에 사용되는 키 정의
#define K_VERSION "version"
#define K_EXCEPT_PROCESS "except_process"
#define K_BLACK_PROCESS "black_process"
#define K_PROCESS_PATH "process_path"
#define K_PROCESS_HASH "process_hash"
#define K_COMMAND_LINE "command_line"
#define K_IP_EXPIRE_TIME "ip_expire_time"

// 커널 이벤트 관련 정의
#define P_EVENT_KERNEL 0  // 커널 이벤트 exec (fork,exit 나중에 추가)
#define P_EVENT_POLLING 1 // 주기적 폴링 이벤트

// 룰에서 탐지된 유형 정의
#define DETECT_TYPE_ERROR -1
#define DETECT_TYPE_EXCEPT_PATH 1
#define DETECT_TYPE_BLACK_PATH 2
#define DETECT_TYPE_BLACK_HASH 3
#define DETECT_TYPE_EXCEPT_HASH 4
#define DETECT_TYPE_KILL 5 // kill process

// 탐지 유형 정의(사용하지 않고 있음)
#define ACTION_TYPE_DETECT 0       // 탐지 + 웹로그
#define ACTION_TYPE_DETECT_BLOCK 1 // 탐지 + 웹로그 +  차단
#define ACTION_TYPE_MONITOR 2      // 탐지 + 로컬로그

// 관제 서버 정보 추가
#define SERVER_IP "59.6.79.138"
#define PORT_NUMBER 8080
#define IS_HTTPS 0 // 0: http, 1: https

// 전역 변수 관련
extern struct json_object *g_rules;     // 룰 리스트 json
extern struct json_object *g_config;    // config.json
extern struct json_object *g_tc_server; // 서버로 부터 받은 정보 (tc_server.php)

// g_config 를 접근하기 위한 뮤텍스
extern pthread_mutex_t g_mutexConfig; // 아직사용안함

#endif