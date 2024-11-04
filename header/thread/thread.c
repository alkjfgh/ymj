#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUM_THREADS 2

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
