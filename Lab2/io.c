#include <stdio.h>

int main(){
        int i,j,a,b;

        scanf("%d %d",&a,&b);

        for (i=a;i>0;i--){
                printf("*");
                for (j=b-1;j>1;j--){
                        if (i == 1 || i == a) {
                                printf("*");}
                        else{
                                printf(".");}
                }
                printf("*\n");
        }

        return 0;
}