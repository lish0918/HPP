#include <stdio.h>
#ifdef _OPENMP
#include <omp.h>
#endif

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

int main() {
    int M;
    int count = 0;
    printf("Enter the value of M: ");
    scanf("%d", &M);
    double start_time, end_time;

    start_time = omp_get_wtime();
    //#pragma omp parallel for reduction(+:count) num_threads(10)
    //#pragma omp parallel for num_threads(2)
    for (int i = 1; i <= M; i++) {
        if (isPrime(i)) {
            //#pragma omp critical
            count++;
        }
    }
    end_time = omp_get_wtime();

    printf("Number of prime numbers between 1 and %d: %d\n", M, count);
    printf("Time consuming: %lf\n",end_time - start_time);    
    return 0;
}
