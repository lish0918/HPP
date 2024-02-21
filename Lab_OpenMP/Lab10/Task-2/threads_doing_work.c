#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

void do_some_work() {
  printf("Now I am going to do some work...\n");
  long int i;
  long int j = 0;
  for(i = 0; i < 3000000000; i++) {
    j += 3;
  }
  printf("Work done! My result j = %ld\n", j);
}

int main(int argc, char** argv) {
  if (argc != 2) {
        printf("Usage: %s <num_threads>\n", argv[0]);
        exit(1);
    }

  int n = atoi(argv[1]);
  double start_time = omp_get_wtime(); // Record start time

  #pragma omp parallel num_threads(n)
    {
      do_some_work();
    }

  double end_time = omp_get_wtime(); // Record end time
  double elapsed_time = end_time - start_time;
  printf("Total time: %f seconds\n", elapsed_time);

  return 0;
}
