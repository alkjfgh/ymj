#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

#define NUM_THREADS 2

// Thread function declarations
typedef void *(*thread_func)(void *);

#endif // THREAD_H