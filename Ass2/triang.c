/*Inefficient with a double loop and multiple calls to factorial.*/
#include <stdio.h>

int factorial(int n){
    int fact = 1;
    for(int i=1;i<=n;i++){
        fact *= i;
    }
    return fact;
}

int main(){
    int n;
    scanf("%d",&n);

    for(int i=0;i<n;i++){
        for(int j=0;j<=i;j++){
            printf("%d\t",factorial(i) / (factorial(j) * factorial(i - j)));
        }
        printf("\n");
    }
    return 0;
}