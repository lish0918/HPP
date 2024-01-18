#include <stdio.h>
#include <stdlib.h>

int isprime(int n)
{
    if(n <= 1) return 0;
    for(int i=2; i*i<=n; ++i)
    {
        if(n%i == 0) return 0;
    }
    return 1;
}

int main()
{
    int n;
    scanf("%d", &n);

    int *arr =  malloc(n * sizeof(int));
    for(int i=0; i<n; ++i) 
    scanf("%d", &arr[i]);

    int *nonprime = NULL;
    int s = 0;
    for(int i=0; i<n; ++i)
    {
        if(!isprime(arr[i]))
        {
            nonprime = realloc(nonprime, (s+1) * sizeof(int));
            nonprime[s++] = arr[i];
        }
    }
    printf("%d Non-Prime numbers: ", s);
    for(int i=0; i<s; ++i) printf("%d ", nonprime[i]);
    printf("\n");

    return 0;
}