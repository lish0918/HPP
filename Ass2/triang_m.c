#include <stdio.h>
#include <stdlib.h>

int binomialCoefficient(int n, int k, int** memo) {
    if (k == 0 || k == n)
        return 1;
    if (memo[n][k] != -1)
        return memo[n][k];
    else {
        memo[n][k] = binomialCoefficient(n - 1, k - 1, memo) + binomialCoefficient(n - 1, k, memo);
        return memo[n][k];
    }
}

int main() {
    int n;
    scanf("%d", &n);

    int** memo = (int**)malloc((n + 1) * sizeof(int*));
    for (int i = 0; i <= n; i++) {
        memo[i] = (int*)malloc((i + 1) * sizeof(int));
        for (int j = 0; j <= i; j++) {
            memo[i][j] = -1;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            printf("%d\t", binomialCoefficient(i, j, memo));
        }
        printf("\n");
    }

    for (int i = 0; i <= n; i++) {
        free(memo[i]);
    }
    free(memo);

    return 0;
}