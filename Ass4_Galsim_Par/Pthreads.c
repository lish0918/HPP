#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <pthread.h>

#define EPSILON 0.001

typedef struct {
    double position_x;
    double position_y;
    double mass;
    double velocity_x;
    double velocity_y;
    double brightness;
} Particle;

typedef struct {
    double force_x;
    double force_y;
} Force;

typedef struct {
    Particle *particles;
    Force *forces;
    int N;
    int start_index;
    int end_index;
} ThreadData;

double delta_t;

void read_initial_configuration(const char *filename, Particle *particles, int N) {
    // Implement function to read initial configuration from file

    char filepath[100];

    strcpy(filepath, "input_data/");
    strcat(filepath, filename);
    FILE *file = fopen(filepath, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        exit(1);
    }

    size_t particles_read = fread(particles, sizeof(Particle), N, file);
    if (particles_read == 0) {
        fprintf(stderr, "Error: Could not read from file %s\n", filename);
        exit(1);
    }

    fclose(file);
}

void* compute_forces(void *arg) {
    ThreadData *data = (ThreadData*) arg;

    double G = 100.0 / data->N;

    for (int i = data->start_index; i < data->end_index; i++) {
        data->forces[i].force_x = 0.0;
        data->forces[i].force_y = 0.0;

        double position_x_i = data->particles[i].position_x;
        double position_y_i = data->particles[i].position_y;
        double mass_i = data->particles[i].mass;

        for (int j = 0; j < data->N; j++) {
            if (j != i) {
                double dx = position_x_i - data->particles[j].position_x;
                double dy = position_y_i - data->particles[j].position_y;
                double distance_squared = sqrt(dx * dx + dy * dy) + EPSILON;
                double distance_cubed = distance_squared * distance_squared * distance_squared;
                double force_magnitude = -G * mass_i * data->particles[j].mass / distance_cubed;
                data->forces[i].force_x += force_magnitude * dx;
                data->forces[i].force_y += force_magnitude * dy;
            }
        }
    }

    pthread_exit(NULL);
}

void* update_particles(void *arg) {
    ThreadData *data = (ThreadData*) arg;

    for (int i = data->start_index; i < data->end_index; i++) {
        double delta_t_mass = delta_t / data->particles[i].mass;
        data->particles[i].velocity_x += delta_t_mass * data->forces[i].force_x;
        data->particles[i].velocity_y += delta_t_mass * data->forces[i].force_y;
        data->particles[i].position_x += delta_t * data->particles[i].velocity_x;
        data->particles[i].position_y += delta_t * data->particles[i].velocity_y;
    }

    pthread_exit(NULL);
}

void simulate(Particle *particles, Force *forces, int N, int nsteps, int n_threads) {
    pthread_t threads[n_threads];
    ThreadData thread_data[n_threads];

    int chunk_size = N / n_threads;

    for (int step = 0; step < nsteps; step++) {
        for (int i = 0; i < n_threads; i++) {
            thread_data[i].particles = particles;
            thread_data[i].forces = forces;
            thread_data[i].N = N;
            thread_data[i].start_index = i * chunk_size;
            thread_data[i].end_index = (i == n_threads - 1) ? N : (i + 1) * chunk_size;

            pthread_create(&threads[i], NULL, compute_forces, (void*)&thread_data[i]);
        }

        for (int i = 0; i < n_threads; i++) {
            pthread_join(threads[i], NULL);
        }

        for (int i = 0; i < n_threads; i++) {
            pthread_create(&threads[i], NULL, update_particles, (void*)&thread_data[i]);
        }

        for (int i = 0; i < n_threads; i++) {
            pthread_join(threads[i], NULL);
        }
    }
}

void write_results(const char *filename, Particle *particles, int N) {
    // Implement function to write results to file
    FILE *file = fopen(filename, "wb");

    size_t particles_written = fwrite(particles, sizeof(Particle), N, file);
    if (particles_written != N) {
        fprintf(stderr, "Error: Could not write to file %s\n", filename);
        exit(1);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    // Parse command line arguments
    if (argc != 7) {
        printf("Usage: %s N filename nsteps delta_t graphics n_threads\n", argv[0]);
        return 1;
    }
    int N = atoi(argv[1]);
    char *filename = argv[2];
    int nsteps = atoi(argv[3]);
    delta_t = atof(argv[4]);
    int graphics = atoi(argv[5]);
    int n_threads = atoi(argv[6]);

    Particle *particles = malloc(N * sizeof(Particle));
    if (particles == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    Force *forces = malloc(N * sizeof(Force));
    if (forces == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    read_initial_configuration(filename, particles, N);
    simulate(particles, forces, N, nsteps, n_threads);
    write_results("result.gal", particles, N);

    free(particles);
    free(forces);

    return 0;
}
