#include <stdio.h>
#include <omp.h>

void thread_func() {
  int thread_id = omp_get_thread_num();
  int num_threads = omp_get_num_threads();
  printf("Thread ID: %d, Total Threads: %d\n", thread_id, num_threads);

}

int main(int argc, char** argv) {

#pragma omp parallel num_threads(6)
  {
    thread_func();
  }

  return 0;
}
