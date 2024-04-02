#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>

// Function to generate random numbers with normal distribution
float generateNormalRandomNumber(float mean, float stddev) {
    float u = rand() / (float)RAND_MAX; // Uniform distribution
    float v = rand() / (float)RAND_MAX; // Uniform distribution
    float z = sqrt(-2 * log(u)) * cos(2 * M_PI * v); // Box-Muller transform
    return mean + stddev * z;
}

// Function to generate random numbers with exponential distribution
float generateExponentialRandomNumber(float lambda) {
    float u = rand() / (float)RAND_MAX; // Uniform distribution
    return -log(1 - u) / lambda;
}

// Function to generate random numbers with uniform distribution
float generateUniformRandomNumber(float min, float max) {
    float u = rand() / (float)RAND_MAX; // Uniform distribution
    return min + u * (max - min);
}

int compare(const void* a, const void* b) {
    float arg1 = *(const float*)a;
    float arg2 = *(const float*)b;
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

// Function to sort an array using Bucket Sort
void bucketSort(float arr[], int n, int num_threads) {
    // Find the maximum and minimum values in the array
    float max_val = arr[0];
    float min_val = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max_val) {
            max_val = arr[i];
        } else if (arr[i] < min_val) {
            min_val = arr[i];
        }
    }

    // Number of buckets
    int bucket_count = n;
    // Create an array of buckets
    float** buckets = (float**)malloc(bucket_count * sizeof(float*));
    int* bucket_sizes = (int*)calloc(bucket_count, sizeof(int));
    for (int i = 0; i < bucket_count; i++) {
        buckets[i] = (float*)malloc(n * sizeof(float));
    }

    // Put array elements in different buckets
    float range = max_val - min_val;
    #pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < n; i++) {
        int bucket_index = (int)((arr[i] - min_val) / range * (bucket_count - 1));
        buckets[bucket_index][bucket_sizes[bucket_index]] = arr[i];
        #pragma omp atomic
        bucket_sizes[bucket_index]++;
    }

    // Sort individual buckets
    #pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < bucket_count; i++) {
        qsort(buckets[i], bucket_sizes[i], sizeof(float), compare);
    }

    // Concatenate all buckets into array
    int index = 0;
    #pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < bucket_count; i++) {
        for (int j = 0; j < bucket_sizes[i]; j++) {
            arr[index++] = buckets[i][j];
        }
    }

    // Print the sorted sequence
    for (int i = 0; i < n; i++) {
        printf("%.2f ", arr[i]);
    }
    printf("\n"); 

    // Free memory
    for (int i = 0; i < bucket_count; i++) {
        free(buckets[i]);
    }
    free(buckets);
    free(bucket_sizes);
}

// Function to check if an array is sorted in ascending order
int isSorted(float arr[], int n) {
    for (int i = 1; i < n; i++) {
        if (arr[i] < arr[i - 1]) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char** argv) {

    if(argc != 3) {printf("Usage: %s length num_threads\n", argv[0]); return -1; }
    int length = atoi(argv[1]);
    int num_threads = atoi(argv[2]);

    // Generate random numbers with different distributions
    float normalNumbers[length];
    float exponentialNumbers[length];
    float uniformNumbers[length];

    for (int i = 0; i < length; i++) {
        normalNumbers[i] = generateNormalRandomNumber(0.5, 0.1);
        exponentialNumbers[i] = generateExponentialRandomNumber(0.5);
        uniformNumbers[i] = generateUniformRandomNumber(0.0, 1.0);
    }

    // Start time
    double start_time = omp_get_wtime();  
    clock_t start = clock();  

    printf("Sorted array (Normal distribution): ");
    bucketSort(normalNumbers, length, num_threads);
    printf("\n");
    printf("Is the array sorted? %s\n", isSorted(normalNumbers, length) ? "Yes" : "No");

    printf("Sorted array (Exponential distribution): ");
    bucketSort(exponentialNumbers, length, num_threads);
    printf("\n");
    printf("Is the array sorted? %s\n", isSorted(exponentialNumbers, length) ? "Yes" : "No");

    printf("Sorted array (Uniform distribution): ");
    bucketSort(uniformNumbers, length, num_threads);
    printf("\n");
    printf("Is the array sorted? %s\n", isSorted(uniformNumbers, length) ? "Yes" : "No");

    // End time
    double end_time = omp_get_wtime();  // End time using omp_get_wtime()
    double time_wtime = end_time - start_time;
    printf("Time wtime: %f seconds\n", time_wtime);

    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_taken);

    return 0;
}