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

/** @struct ThreadContext
 *  @brief 스레드 컨텍스트 정보를 담는 구조체
 */
typedef struct
{
    int thread_num;  /**< 스레드 번호 */
    void *user_data; /**< 사용자 정의 데이터 */
} ThreadContext;

/**
 * @brief 스레드 컨텍스트와 함께 스레드들을 생성하고 실행
 * @param function 실행할 스레드 함수
 * @param num_threads 생성할 스레드의 수
 * @param user_data 각 스레드에 전달할 사용자 데이터
 * @return 성공 시 0, 실패 시 -1
 */
int create_and_run_threads_with_context(thread_func function, int num_threads, void *user_data);

#endif // THREAD_H