#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void* the_thread_func(void* arg) {
  int* p = malloc(sizeof(int));
  *p = 42;
  pthread_exit(p);
}

int main() {
  printf("This is the main() function starting.\n");

  /* Start thread. */
  pthread_t thread;
  printf("the main() function now calling pthread_create().\n");
  if(pthread_create(&thread, NULL, the_thread_func, NULL) != 0) {
    printf("ERROR: pthread_create failed.\n");
    return -1;
  }

  printf("This is the main() function after pthread_create()\n");

  void* result;
  printf("the main() function now calling pthread_join().\n");
  if(pthread_join(thread, &result) != 0) {
      printf("ERROR: pthread_join failed.\n");
      return -1;
  }

  int *p = (int*)result;
  printf("Result from the thread is %d\n", *p);

  free(p);

  return 0;
}