#include <stdio.h>

int main()
{
        int i;

        for (i=100;i>=0;i=i-4)
        {
                printf("%d ",i);
        }
        printf("\n");

        i = 100;
        while (i>=0)
        {
                printf("%d ",i);
                i = i - 4;
        }
        printf("\n");

        i = 100;
        do{
                printf("%d ",i);
                i = i - 4;
        }while(i>=0);
        printf("\n");

        return 0;
}