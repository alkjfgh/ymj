/**
 * @file processCommand.h
 * @brief 커맨드 수신 처리 함수 선언
 * @author 유명재
 * @date 2024-11-13
 */
#ifndef PROCESS_COMMAND_H
#define PROCESS_COMMAND_H

#include <json-c/json.h>

/**
 * @brief TC 종료 처리 함수
 * @return 성공 시 0, 실패 시 음수 값 반환
 */
int processTcEnd(void);

/**
 * @brief TC 삭제 설정 처리 함수
 * @return 성공 시 0, 실패 시 음수 값 반환
 */
int processTcSetDelete(void);

/**
 * @brief TC ZIP 설정 처리 함수
 * @return 성공 시 0, 실패 시 음수 값 반환
 */
int processTcSetZip(void);

/**
 * @brief TC ZIP 정보 조회 처리 함수
 * @return 성공 시 0, 실패 시 음수 값 반환
 */
int processTcGetZip(void);

/**
 * @brief TC 프로세스 목록 조회 처리 함수
 * @return 성공 시 0, 실패 시 음수 값 반환
 */
int processTcGetProcessList(void);

/**
 * @brief TC 명령어 처리 함수
 * @param command_parameters 명령어 파라미터 문자열
 * @return 성공 시 0, 실패 시 음수 값 반환
 */
int processTcCmd(const char *command_parameters);

/**
 * @brief TC 사용자 정보 설정 처리 함수
 * @param params JSON 형식의 사용자 정보 파라미터
 * @return 성공 시 0, 실패 시 음수 값 반환
 */
int processTcSetUserInfo(struct json_object *params);

#endif