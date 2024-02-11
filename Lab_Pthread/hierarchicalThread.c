#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_THREADS 6
pthread_t threads[NUM_THREADS];

void* childThread(void* arg) {
    int threadNum = *(int*)arg;
    printf("Child thread %d created\n", threadNum + 1);

    // Create two new threads for each thread
    if (threadNum == 0 || threadNum == 1) {
        int childThreadNums[2];
        for (int i = 0; i < 2; i++) {
            childThreadNums[i] = threadNum * 2 + i + 2;
            pthread_create(&threads[childThreadNums[i]], NULL, childThread, &childThreadNums[i]);
        }
    }
    return NULL;
}

int main() {
    // Create two threads
    int threadNums[NUM_THREADS];

    for (int i = 0; i < 2; i++) {
        threadNums[i] = i;
        pthread_create(&threads[i], NULL, childThread, &threadNums[i]);
    }

    printf("Main Thread\n");

    // Wait for the threads to finish
    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
