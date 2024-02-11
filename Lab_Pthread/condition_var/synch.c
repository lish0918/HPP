#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS     8

pthread_mutex_t lock;
pthread_cond_t mysignal;
int waiting = 0;
int state = 0;

void barrier() {
  int mystate;
  pthread_mutex_lock (&lock);
  //确保在同一时间只有一个线程可以执行这个函数的内部代码
  mystate=state;
  waiting++;
  //有一个线程已经到达了这个屏障，所以等待的线程数加1
  
  //检查是否所有的线程都已经到达了这个屏障
  if (waiting == NUM_THREADS) {//如果所有的线程都已经到达了这个屏障
    waiting = 0;
    state = 1 - mystate;
    pthread_cond_broadcast(&mysignal); //唤醒所有等待的线程
  }
  while (mystate == state) { //如果没有所有的线程都已经到达了这个屏障
    pthread_cond_wait(&mysignal, &lock); //等待唤醒
  }
  pthread_mutex_unlock(&lock); 
}

void* HelloWorld(void* arg) {
  long id=(long)arg;
  printf("Hello World! %ld\n", id);
  barrier();
  printf("Bye Bye World! %ld\n", id);
  return NULL;
}

int main(int argc, char *argv[]) {
  pthread_t threads[NUM_THREADS];
  long t;
  // Initialize things
  pthread_cond_init(&mysignal, NULL);
  pthread_mutex_init(&lock, NULL);
  // Create threads
  for(t=0; t<NUM_THREADS; t++)
    pthread_create(&threads[t], NULL, HelloWorld, (void*)t);
  // Wait for threads to finish
  for(t=0; t<NUM_THREADS; t++)
    pthread_join(threads[t], NULL);
  // Cleanup
  pthread_cond_destroy(&mysignal);
  pthread_mutex_destroy(&lock);
  // Done!
  return 0;
}