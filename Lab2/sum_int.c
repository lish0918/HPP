#include <stdio.h>
#include <stdlib.h>

int main(){
    int n = 0;
    int *arr = NULL;
    int num;
    int capacity = 0;

    printf("Input: ");
    while(scanf("%d", &num) == 1){
        if (n == capacity) {
            capacity += 10;
            arr = realloc(arr, capacity * sizeof(int));
            if (arr == NULL) {
                printf("Memory allocation failed\n");
                return 1;
            }
        }
        arr[n++] = num;
    }

    int sum = 0;
    printf("Output:\n");
    for(int i = 0; i < n; ++i) {
        printf("%d ", arr[i]);
        sum += arr[i];
    }
    printf("\nSum: %d\n", sum); 

    return 0;
}