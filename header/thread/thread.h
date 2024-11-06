/**
 * @file thread.h
 * @brief 스레드 생성 및 관리를 위한 헤더 파일
 * @details 멀티스레드 작업을 위한 기본적인 스레드 생성 및 실행 기능을 제공합니다.
 */

#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

/** @typedef thread_func
 *  @brief 스레드 함수 포인터 타입 정의
 */
typedef void *(*thread_func)(void *);

/**
 * @brief 여러 스레드를 생성하고 실행하는 함수
 * @param functions 실행할 스레드 함수들의 배열
 * @param num_threads 생성할 스레드의 수
 * @param thread_nums 스레드 번호를 저장할 배열
 * @return 성공 시 0, 실패 시 에러 코드 반환
 * @details 지정된 수만큼의 스레드를 생성하고 각각 주어진 함수를 실행합니다.
 */
int create_and_run_threads(thread_func *functions, int num_threads, int *thread_nums);

#endif // THREAD_H