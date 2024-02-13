#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define P 4

double sum[P] = {0.0};
const int intervals = 500000000;
double dx = 1.0/intervals;

void* the_thread_func(void* arg) {
    int thread_args = *(int*)arg;
    int start = thread_args * (intervals/P) + 1;
    int end = (thread_args + 1) * (intervals/P);

    double x;

    for (int i = start; i <= end; i ++) {
        x = dx*(i - 0.5);
        sum[thread_args] += dx*4.0/(1.0 + x*x);
    }

    return NULL;
}
//numerical consistency of floating numbers
//并行时，两个浮点数相加，小数会向大数靠拢（改变指数，失精），导致最终与串行结果不一致
int main(int argc, char *argv[]) {
  int i;
  pthread_t threads[P];
  int thread_args[P];

  for (i = 0; i < P; i++) {
    thread_args[i] = i;
    pthread_create(&threads[i], NULL, the_thread_func, &thread_args[i]);
  }

  for (i = 0; i < P; i++) {
    pthread_join(threads[i], NULL);
  }

  double total_sum = 0.0;
    for (i = 0; i < P; i++) {
        total_sum += sum[i];
    }

  printf("PI is approx. %.16f\n",  total_sum);

  return 0;
}
//有个循环持续相加1.0时，（例子）当sum加到10E17时，sum不再增加（1.0对它来说太小），因为浮点数的精度有限
//因为浮点数相加先对齐