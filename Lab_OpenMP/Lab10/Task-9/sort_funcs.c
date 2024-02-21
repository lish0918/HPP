#include <stdlib.h>
#include "sort_funcs.h"

void bubble_sort(intType* list, int N) {
  int i, j;
  for(i = 0; i < N-1; i++)
    for(j = 1+i; j < N; j++) {
      if(list[i] > list[j]) {
	// Swap
	intType tmp = list[i];
	list[i] = list[j];
	list[j] = tmp;
      }
    }
}

void merge_sort(intType* list_to_sort, int N, int nThreads) {
    if (N == 1) {
        // Only one element, no sorting needed. Just return directly in this case.
        return;
    }
    int n1 = N / 2;
    int n2 = N - n1;
    // Allocate new lists
    intType* list = (intType*)malloc(N * sizeof(intType));
    int i;
    for (i = 0; i < N; i++)
        list[i] = list_to_sort[i];
    // Determine the number of threads to use at the next level
    int nextThreads = nThreads / 2;
    // Sort list1 and list2 with the specified number of threads
    if (nextThreads > 1) {
        #pragma omp parallel sections num_threads(2)
        {
            #pragma omp section
            {
                merge_sort(list, n1, nextThreads); // Recursively call merge_sort with updated nThreads
            }
            #pragma omp section
            {
                merge_sort(list + n1, n2, nextThreads); // Recursively call merge_sort with updated nThreads
            }
        }
    } else {
        // Single thread for lower levels
        merge_sort(list, n1, nextThreads);
        merge_sort(list + n1, n2, nextThreads);
    }
    // Merge!
    int i1 = 0;
    int i2 = n1;
    i = 0;
    while (i1 < n1 && i2 < N) {
        if (list[i1] < list[i2]) {
            list_to_sort[i] = list[i1];
            i1++;
        }
        else {
            list_to_sort[i] = list[i2];
            i2++;
        }
        i++;
    }
    while (i1 < n1)
        list_to_sort[i++] = list[i1++];
    while (i2 < N)
        list_to_sort[i++] = list[i2++];
    free(list);
}