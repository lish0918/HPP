#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

// 判断一个数是否为质数
int isPrime(int num) {
    if (num <= 1)
        return 0;
    
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return 0;
        }
    }   
    return 1;
}

// 线程函数，用于检查质数
void* checkPrimes(void* arg) {
    int* params = (int*)arg;
    int start = params[0];
    int end = params[1];
    int* count = (int*)malloc(sizeof(int));
    *count = 0;

    for (int num = start; num <= end; num++) {
        if (isPrime(num)) {
            (*count)++;
        }
    }

    return count;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s M N\n", argv[0]);
        return 1;
    }

    int M = atoi(argv[1]); // 获取范围上限M
    int N = atoi(argv[2]); // 获取线程数N

    // 计算每个线程要处理的范围
    int thread_per_iteration = M / N;
    int remainder = M % N;
    int start = 1;
    int end = thread_per_iteration;

    pthread_t threads[N];
    int thread_params[N][2];

    // 创建并启动线程
    for (int i = 0; i < N; i++) {
        if (remainder > 0) {
            end++;
            remainder--;
        }
        thread_params[i][0] = start;
        thread_params[i][1] = end;
        pthread_create(&threads[i], NULL, checkPrimes, (void*)thread_params[i]);
        start = end + 1;
        end += thread_per_iteration;
    }

    // 等待所有线程完成并统计结果
    int total_count = 0;
    for (int i = 0; i < N; i++) {
        int* count;
        pthread_join(threads[i], (void**)&count);
        total_count += *count;
        free(count);
    }

    printf("Number of prime numbers between 1 and %d: %d\n", M, total_count);

    return 0;
}
