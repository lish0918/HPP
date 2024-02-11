#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

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

/**
 * This function is the thread function that checks if a number is prime.
 * It takes a void pointer as an argument and casts it to an int pointer.
 * Then it allocates memory for an int to store the result of the isPrime function.
 * Finally, it returns the result as a void pointer.
 *
 * @param arg A void pointer to the number to be checked for primality.
 * @return A void pointer to the result of the isPrime function.
 */
void* the_thread_func(void* arg){
    int num = *(int*)arg;
    int* result = (int*)malloc(sizeof(int));
    *result = isPrime(num);
    return result;
}

int main() {
    int M, count = 0;
    printf("Enter the value of M: ");
    scanf("%d", &M);
    
    pthread_t threads[M];
    int nums[M+1];
    for (int i = 1; i <= M; i++) {
        nums[i] = i;
    }
    // 创建并行线程来判断每个数是否为素数
    for (int i = 1; i <= M; i++) {
        pthread_create(&threads[i], NULL, the_thread_func, &nums[i]);
    }
    /**
     * This loop iterates over the threads and waits for each thread to complete.
     * It retrieves the result from each thread, adds it to the count, and frees the memory allocated for the result.
     *
     * @param M The number of threads.
     * @param threads An array of pthread_t representing the threads.
     * @param count The variable to store the total count.
     */
    for (int i = 1; i <= M; i++) {
        int* result;
        pthread_join(threads[i], (void**)&result);
        count += *result;
        free(result);
    }
    printf("Number of prime numbers between 1 and %d: %d\n", M, count);
    
    return 0;
}

/*在大多数现代的 Linux 发行版和 macOS 中，pthread 库通常已经预装了。*/
/*如果你在 Windows 系统上使用 GCC，你可能需要下载一个支持 pthread 的 GCC 版本。*/