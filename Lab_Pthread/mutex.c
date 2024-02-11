#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long int sum = 0;
int N = 100000;
pthread_mutex_t mutex;

void* the_thread_func(void* arg) {
    for(int i = 1; i <= N; ++i) {
        pthread_mutex_lock(&mutex);
        sum += 1;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
} //使用条件变量来使该线程等待主线程完成操作

int main(int argc, char **argv) {
    if(argc != 2) {
        printf("Usage: %s N\n", argv[0]);
        return -1;
    }

    printf("This is the main() function starting.\n");

    int N = atoi(argv[1]);

    /* Initialize mutex. */
    pthread_mutex_init(&mutex, NULL);

    /* Start thread. */
    printf("the main() function now calling pthread_create().\n");
    pthread_t threads[N];
    for(int i = 0; i < N; i++)
        pthread_create(&threads[i], NULL, the_thread_func, NULL);

    printf("This is the main() function after pthread_create()\n");

    /* Wait for thread to finish. */
    printf("the main() function now calling pthread_join().\n");
    for(int i = 0; i < N; i++)
        pthread_join(threads[i], NULL);

    /* Destroy mutex. */
    pthread_mutex_destroy(&mutex);

    printf("sum = %ld\n", sum);

    return 0;
}