#include <omp.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  int a, b;
  a = 100;
  b = 20;

#pragma omp parallel num_threads(4) firstprivate(a) 
  {
    a += b;
    printf("a= %d b= %d\n",a,b);
  }

  return 0;
}
