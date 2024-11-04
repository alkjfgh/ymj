#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 2

void* thread_function_1(void* arg) {
    printf("Thread 1: Starting task\n");
    sleep(1);  // Simulating work


    printf("Thread 1: Task complete\n");
    return NULL;
}

void* thread_function_2(void* arg) {
    printf("Thread 2: Starting task\n");
    sleep(1);  // Simulating work
    
    
    printf("Thread 2: Task complete\n");
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t threads[NUM_THREADS];
    int ret;

    // Create threads
    ret = pthread_create(&threads[0], NULL, thread_function_1, NULL);
    if (ret) {
        fprintf(stderr, "Error creating thread 1\n");
        exit(EXIT_FAILURE);
    }

    ret = pthread_create(&threads[1], NULL, thread_function_2, NULL);
    if (ret) {
        fprintf(stderr, "Error creating thread 2\n");
        exit(EXIT_FAILURE);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("All threads have completed. Exiting program.\n");

    return 0;
}
 
