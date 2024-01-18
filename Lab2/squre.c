#include <stdio.h>
#include <math.h>

int main(){
    int a;
    scanf("%d",&a);
    if(sqrt(a)==(int)sqrt(a))
        printf("YES\n");
    else
        printf("NO\n");
        
    return 0;
}