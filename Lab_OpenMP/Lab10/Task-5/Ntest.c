#include <stdio.h>
#include <omp.h>

const long int NA = 700000000;
const long int NB = 100000000;

long int thread_func_A() {
  long int i;
  long int sum = 0;
  for(i = 0; i < NA; i++)
    sum += 7;
  return sum;
}

long int thread_func_B() {
  long int i;
  long int sum = 0;
  for(i = 0; i < NB; i++)
    sum += 7;
  return sum;
}

int main() {
  printf("This is the main() function starting.\n");

  long int result_A;
  long int result_B;
  double start_time_A, end_time_A, start_time_B, end_time_B;

#pragma omp parallel num_threads(2)
  {
    int id = omp_get_thread_num();
    if(id == 0){
      start_time_A = omp_get_wtime();
      result_A = thread_func_A();
      end_time_A = omp_get_wtime();
    }   
    else{
      start_time_B = omp_get_wtime();
      result_B = thread_func_B();
      end_time_B = omp_get_wtime();
    }     
  }

  printf("This is the main() function after the parallel block.\n");

  printf("result_A : %ld\n", result_A);
  printf("runningtime_A : %lf\n", end_time_A - start_time_A);
  printf("result_B : %ld\n", result_B);
  printf("runningtime_B : %lf\n", end_time_B - start_time_B);
  long int totalSum = result_A + result_B;
  printf("totalSum : %ld\n", totalSum);

  return 0;
}
