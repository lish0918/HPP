#include <stdio.h>
#include <stdlib.h>

int CmpDouble(const void *a, const void *b){
    double *pa = (double *)a;
    double *pb = (double *)b;
    if (*pa > *pb){
        return 1;
    }
    else if (*pa < *pb){
        return -1;
    }
    else{
        return 0;
    }
}

int main(){
    double arrDouble[] = {9.3, -2.3, 1.2, -0.4, 2, 9.2, 1, 2.1, 0, -9.2};
    int arrlen=10;
    qsort (arrDouble, arrlen, sizeof(double), CmpDouble);

    for (int i = 0; i < arrlen; i++){
        printf("%.1f ", arrDouble[i]);
    }
    printf("\n");

    return 0;
}