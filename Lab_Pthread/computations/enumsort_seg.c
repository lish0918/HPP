#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define NUM_THREADS 5
#define len 100000

static double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}

double indata[len], outdata[len];

typedef struct {
    int start;
    int end;
} Task;

pthread_mutex_t m;

void *findrank(void *arg) {
    Task *task = (Task *)arg;
    int rank;
    int i;

    for (int j = task->start; j < task->end; j++) {
        rank = 0;
        for (i = 0; i < len; i++) {
            if (indata[i] < indata[j]) rank++;
        }
        outdata[rank] = indata[j];
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;
    void *status;
    Task tasks[NUM_THREADS];
    int segment_size = len / NUM_THREADS;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // Generate random numbers
    for (int i = 0; i < len; i++) {
        indata[i] = drand48();
        outdata[i] = -1.0;
    }

    // Enumeration sort
    double startTime = get_wall_seconds();
    for (int j = 0; j < NUM_THREADS; j++) {
        tasks[j].start = j * segment_size;
        tasks[j].end = (j + 1) * segment_size;
        pthread_create(&threads[j], &attr, findrank, (void *)&tasks[j]);
    }

    for (int j = 0; j < NUM_THREADS; j++) {
        pthread_join(threads[j], &status);
    }

    double timeTaken = get_wall_seconds() - startTime;
    printf("Time: %f  NUM_THREADS: %d\n", timeTaken, NUM_THREADS);

    // Check results, -1 implies data same as the previous element
    for (int i = 0; i < len - 1; i++) {
        if (outdata[i] > outdata[i + 1] && outdata[i + 1] > -1) {
            printf("ERROR: %f,%f\n", outdata[i], outdata[i + 1]);
        }
    }

    return 0;
}
