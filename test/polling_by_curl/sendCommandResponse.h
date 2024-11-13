/**
 * @file sendCommandResponse.h
 * @brief 명령어 응답 처리 및 서버 통신 관련 기능을 선언한 파일
 * @author 유명재
 * @date 2024-11-13
 */

#ifndef SEND_COMMAND_RESPONSE_H
#define SEND_COMMAND_RESPONSE_H

#include "libwww_curl.h"
#include <json-c/json.h>

#define TARGET_URL "/tc_command_res.php"

#define PROCESS_NAME_MAX_LEN 256
#define PROCESS_PARAM_MAX_LEN 1024
#define PROCESS_HASH_MAX_LEN 256

/**
 * @brief 프로세스 정보를 저장하는 구조체
 *
 * 시스템에서 실행 중인 프로세스의 상세 정보를 담는 구조체입니다.
 */
typedef struct process_info
{
    pid_t pid;            /**< 프로세스의 고유 식별자 */
    char *process_name;   /**< 프로세스의 실행 파일 이름 */
    long process_desc;    /**< 프로세스 부가 설명 */
    char *process_param;  /**< 프로세스 실행 시 전달된 명령행 매개변수 */
    char *process_hash;   /**< 프로세스 실행 파일의 해시값 */
    int process_excluded; /**< 프로세스 제외 플래그 (0: 포함, 1: 제외) */
} ProcessInfo;

/**
 * @brief POST 요청에 사용될 JSON 데이터 생성
 * @param res 명령 실행 결과
 * @param command_name 실행된 명령어 이름
 * @return 생성된 JSON 객체
 */
struct json_object *makePostData(struct json_object *res, const char *command_name);

/**
 * @brief ZIP 파일 데이터 조회를 위한 JSON 데이터 생성
 * @return 생성된 JSON 객체
 */
struct json_object *makeGetZipData(void);

/**
 * @brief 시스템의 프로세스 목록 조회
 * @return ProcessInfo 구조체 배열에 대한 포인터
 */
const ProcessInfo *getProcessList(void);

/**
 * @brief 프로세스 목록 데이터를 JSON 형식으로 생성
 * @return 생성된 JSON 객체
 */
struct json_object *makeGetProcessListData(void);

/**
 * @brief 명령어 실행 데이터 생성
 * @return 생성된 JSON 객체
 */
struct json_object *makeCmdData(void);

/**
 * @brief 서버로 명령어 실행 결과 전송
 * @param postData 전송할 JSON 데이터
 * @return 서버 응답 데이터를 담은 JSON 객체
 */
struct json_object *sendCommandResponse(struct json_object *postData);

#endif /* SEND_COMMAND_RESPONSE_H */