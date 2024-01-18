#include <stdio.h>

int main()
{
    double a;
    int b;
    char c;

    printf("Enter a double: ");
    scanf("%lf", &a);
    printf("Enter an integer: ");
    scanf("%d", &b);
    printf("Enter a character: ");
    scanf(" %c", &c);

    printf("Value of %lf: %lf, Address of %lf: %p, Size of %lf: %lu bytes\n",a,a,a,&a,a,sizeof(a));
    printf("Value of %d: %d, Address of %d: %p, Size of %d: %lu bytes\n",b,b,b,&b,b,sizeof(b));
    printf("Value of %c: %c, Address of %c: %p, Size of %c: %lu bytes\n",c,c,c,&c,c,sizeof(c));
    
    return 0;
}