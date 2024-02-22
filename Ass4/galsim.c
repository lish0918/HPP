/*Final Version*/

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


void simulate(double *position_x, double *position_y, double *mass, double *velocity_x, double *velocity_y, double *brightness, double *force_x, double *force_y, int N, int nsteps, double delta_t) {
    double G = 100.0/N;
    for (int step = 0; step < nsteps; step++) {
        for (int i = 0; i < N; i++){
            force_x[i] = 0.0;
            force_y[i] = 0.0;
        } 
        for (int i = 0; i < N; i++) {
            double position_x_i = position_x[i];
            double position_y_i = position_y[i];
            double mass_i = mass[i];

            double forcex = force_x[i];
            double forcey = force_y[i];

            for (int j = i + 1; j < N; j++) {
                double dx = position_x_i - position_x[j];
                double dy = position_y_i - position_y[j];
                double distance_squared = sqrt(dx * dx + dy * dy) + EPSILON;
                double distance_cubed = distance_squared * distance_squared * distance_squared;
                double force_magnitude = -G * mass_i * mass[j] / distance_cubed;
                forcex += force_magnitude * dx;
                forcey += force_magnitude * dy;

                force_x[j] -= force_magnitude * dx;
                force_y[j] -= force_magnitude * dy;
            }
            force_x[i] = forcex;
            force_y[i] = forcey;
        }
        for(int i = 0; i < N; i++){
            double delta_t_mass = delta_t / mass[i];
            velocity_x[i] += delta_t_mass * force_x[i];
            velocity_y[i] += delta_t_mass * force_y[i];
            position_x[i] += delta_t * velocity_x[i];
            position_y[i] += delta_t * velocity_y[i];
        }
    }
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
    simulate(position_x, position_y, mass, velocity_x, velocity_y, brightness, force_x, force_y, N, nsteps, delta_t);
    write_results("result.gal", position_x, position_y, mass, velocity_x, velocity_y, brightness, N);

    return 0;
}