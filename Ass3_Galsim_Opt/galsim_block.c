/*0m29,948s*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define EPSILON 0.001

void read_initial_configuration(const char *filename, double *position_x, double *position_y, double *mass, double *velocity_x, double *velocity_y, double *brightness, int N) {
    // Implement function to read initial configuration from file

    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        exit(1);
    }

    for(int i = 0; i < N; i++){
        size_t particles_read = fread(&position_x[i], sizeof(double), 1, file);
        particles_read += fread(&position_y[i], sizeof(double), 1, file);
        particles_read += fread(&mass[i], sizeof(double), 1, file);
        particles_read += fread(&velocity_x[i], sizeof(double), 1, file);
        particles_read += fread(&velocity_y[i], sizeof(double), 1, file);
        particles_read += fread(&brightness[i], sizeof(double), 1, file);
    }

    fclose(file);
}


void simulate(double *position_x, double *position_y, double *mass, double *velocity_x, double *velocity_y, double *brightness, int N, int nsteps, double delta_t) {
    double G = 100.0 / N;
    double *temp_force_x = malloc(N * sizeof(double));
    double *temp_force_y = malloc(N * sizeof(double));
    double *temp_velocity_x = malloc(N * sizeof(double));
    double *temp_velocity_y = malloc(N * sizeof(double));
    double *temp_position_x = malloc(N * sizeof(double));
    double *temp_position_y = malloc(N * sizeof(double));

    if (temp_force_x == NULL || temp_force_y == NULL ||
        temp_velocity_x == NULL || temp_velocity_y == NULL ||
        temp_position_x == NULL || temp_position_y == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    int BLOCK_SIZE = 64; //32, 64, 128, 256, 512, 1024
    for (int step = 0; step < nsteps; step++) {
        for (int block_start = 0; block_start < N; block_start += BLOCK_SIZE) {
            int block_end = block_start + BLOCK_SIZE;
            if (block_end > N) {
                block_end = N;
            }

            // Compute forces for particles in the current block
            for (int i = block_start; i < block_end; i++) {
                temp_force_x[i] = 0.0;
                temp_force_y[i] = 0.0;

                double position_x_i = position_x[i];
                double position_y_i = position_y[i];
                double mass_i = mass[i];

                for (int j = 0; j < N; j++) {
                    if (j != i) {
                        double dx = position_x_i - position_x[j];
                        double dy = position_y_i - position_y[j];
                        double distance_squared = sqrt(dx * dx + dy * dy) + EPSILON;
                        double distance_cubed = distance_squared * distance_squared * distance_squared;
                        double force_magnitude = -G * mass_i * mass[j] / distance_cubed;
                        temp_force_x[i] += force_magnitude * dx;
                        temp_force_y[i] += force_magnitude * dy;
                    }
                }
            }

            // Update positions and velocities for particles in the current block
            for (int i = block_start; i < block_end; i++) {
                double delta_t_mass = delta_t / mass[i];
                temp_velocity_x[i] = velocity_x[i] + delta_t_mass * temp_force_x[i];
                temp_velocity_y[i] = velocity_y[i] + delta_t_mass * temp_force_y[i];
                temp_position_x[i] = position_x[i] + delta_t * temp_velocity_x[i];
                temp_position_y[i] = position_y[i] + delta_t * temp_velocity_y[i];
            }
        }

        // Copy temp arrays back to original arrays
        memcpy(position_x, temp_position_x, N * sizeof(double));
        memcpy(position_y, temp_position_y, N * sizeof(double));
        memcpy(velocity_x, temp_velocity_x, N * sizeof(double));
        memcpy(velocity_y, temp_velocity_y, N * sizeof(double));
    }

    free(temp_force_x);
    free(temp_force_y);
    free(temp_velocity_x);
    free(temp_velocity_y);
    free(temp_position_x);
    free(temp_position_y);
}


void write_results(const char *filename, double *position_x, double *position_y, double *mass, double *velocity_x, double *velocity_y, double *brightness, int N) {
    // Implement function to write results to file
    FILE *file = fopen(filename, "wb");

    for(int i = 0; i < N; i++){
        size_t particles_read = fwrite(&position_x[i], sizeof(double), 1, file);
        particles_read += fwrite(&position_y[i], sizeof(double), 1, file);
        particles_read += fwrite(&mass[i], sizeof(double), 1, file);
        particles_read += fwrite(&velocity_x[i], sizeof(double), 1, file);
        particles_read += fwrite(&velocity_y[i], sizeof(double), 1, file);
        particles_read += fwrite(&brightness[i], sizeof(double), 1, file);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    // Parse command line arguments
    if (argc != 6) {
        printf("Usage: %s N filename nsteps delta_t graphics\n", argv[0]);
        return 1;
    }
    int N = atoi(argv[1]);
    char *filename = argv[2];
    int nsteps = atoi(argv[3]);
    double delta_t = atof(argv[4]);
    int graphics = atoi(argv[5]);

    double position_x[N], position_y[N], mass[N], velocity_x[N], velocity_y[N], brightness[N], force_x[N], force_y[N];
    
    read_initial_configuration(filename, position_x, position_y, mass, velocity_x, velocity_y, brightness, N);
    simulate(position_x, position_y, mass, velocity_x, velocity_y, brightness, N, nsteps, delta_t);
    write_results("result.gal", position_x, position_y, mass, velocity_x, velocity_y, brightness, N);

    return 0;
}
