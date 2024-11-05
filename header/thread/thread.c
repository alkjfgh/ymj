#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUM_THREADS 2

/**
 * @brief 여러 스레드를 생성하고 실행하는 함수
 *
 * @param functions 실행할 스레드 함수들의 배열
 * @param num_threads 생성할 스레드의 수
 * @return int 성공 시 0, 실패 시 -1 반환
 *
 * @details
 * 지정된 수만큼의 스레드를 생성하고, 각각의 스레드에 대해 주어진 함수를 실행합니다.
 * 모든 스레드가 완료될 때까지 대기한 후 자원을 해제합니다.
 *
 * @note 메모리 할당 실패나 스레드 생성 실패 시 -1을 반환하며,
 * 할당된 자원은 자동으로 해제됩니다.
 */
int create_and_run_threads(thread_func *functions, int num_threads)
{
    pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);
    if (!threads)
    {
        return -1;
    }

    // Create threads
    for (int i = 0; i < num_threads; i++)
    {
        int ret = pthread_create(&threads[i], NULL, functions[i], NULL);
        if (ret)
        {
            fprintf(stderr, "Error creating thread %d\n", i);
            free(threads);
            return -1;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    return 0;
}
