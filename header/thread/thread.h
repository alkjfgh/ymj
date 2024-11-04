#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

#define NUM_THREADS 2

// Thread function declarations
typedef void *(*thread_func)(void *);

int create_and_run_threads(thread_func *functions, int num_threads);

#endif // THREAD_H