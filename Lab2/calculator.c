#include <stdio.h>
#include <math.h>

int main(){
    double a,b;
    char op;

    scanf("%lf %c %lf",&a,&op,&b);
    if(op=='+')
        printf("%lf\n",a+b);
    else if(op=='-')
        printf("%lf\n",a-b);
    else if(op=='*')
        printf("%lf\n",a*b);
    else if(op=='/')
        printf("%lf\n",a/b);

    return 0;
}