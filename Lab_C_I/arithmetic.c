#include <stdio.h>
#include <math.h>

int main(){
        int a,b;
        double c,d,e;

        scanf("%d %d",&a,&b);

        if (a%2==0 && b%2==0)
            printf("%d\n",a+b);
        else
            printf("%d\n",a*b);   

        double maxnum;
        scanf("%lf %lf %lf",&c,&d,&e);
        if (c>d && c>e)
            maxnum=c;
        else if (d>c && d>e)
            maxnum=d;
        else
            maxnum=e;
        printf("%lf\n",maxnum);

        double secnum;
        scanf("%lf %lf %lf",&c,&d,&e);
        if (c>d && c<e || c>e && c<d)
            secnum=c;
        else if (d>c && d<e || d>e && d<c)
            secnum=d;
        else
            secnum=e;
        printf("%lf\n",secnum);

        return 0;
}