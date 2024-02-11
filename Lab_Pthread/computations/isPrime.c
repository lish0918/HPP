#include <stdio.h>

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
    
    for (int i = 1; i <= M; i++) {
        if (isPrime(i)) {
            count++;
        }
    }
    printf("Number of prime numbers between 1 and %d: %d\n", M, count);
    
    return 0;
}
