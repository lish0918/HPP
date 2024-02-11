#include <stdio.h>
#include <pthread.h>

// Struct to hold thread information
typedef struct {
    int index;
} ThreadInfo;

// Function to be executed by each thread
void* threadFunction(void* arg) {
    ThreadInfo* info = (ThreadInfo*)arg;
    printf("Thread %d\n", info->index);
    return NULL;
}

int main() {
    int N; // Number of threads
    printf("Enter the number of threads: ");
    scanf("%d", &N);

    pthread_t threads[N];
    ThreadInfo threadInfo[N];

    // Create N threads
    for (int i = 0; i < N; i++) {
        threadInfo[i].index = i;
        pthread_create(&threads[i], NULL, threadFunction, (void*)&threadInfo[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
