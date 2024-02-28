/*1m33,218s*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define EPSILON 0.001

void read_initial_configuration(const char *filename, double *position_x, double *position_y, double *mass, double *velocity_x, double *velocity_y, double *brightness, int N) {
    // Implement function to read initial configuration from file

    char filepath[100];

    strcpy(filepath, "input_data/");
    strcat(filepath, filename);
    FILE *file = fopen(filepath, "rb");
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


void compute_forces(double *position_x, double *position_y, double *mass, double *velocity_x, double *velocity_y, double *brightness, double *force_x, double *force_y, int N) {
    // Implement function to compute forces between particles
    double G = 100.0/N;
    for (int i = 0; i < N; i++) {
        force_x[i] = 0.0;
        force_y[i] = 0.0;

        for (int j = 0; j < N; j++) {
            if (j != i) {
                double dx = position_x[i] - position_x[j];
                double dy = position_y[i] - position_y[j];
                double distance = sqrt(dx * dx + dy * dy);
                double force_magnitude = -G * mass[i] * mass[j] / pow(distance + EPSILON, 3);
                force_x[i] += force_magnitude * dx;
                force_y[i] += force_magnitude * dy;
            }
        }
    }
}

void update_particle(double *position_x, double *position_y, double *mass, double *velocity_x, double *velocity_y, double *brightness, double *force_x, double *force_y, double delta_t) {
    // Implement function to update position and velocity of a particle
    *velocity_x += delta_t * (*force_x) / *mass;
    *velocity_y += delta_t * (*force_y) / *mass;
    *position_x += delta_t * (*velocity_x);
    *position_y += delta_t * (*velocity_y);
}

void simulate(double *position_x, double *position_y, double *mass, double *velocity_x, double *velocity_y, double *brightness, double *force_x, double *force_y, int N, int nsteps, double delta_t) {
    for (int step = 0; step < nsteps; step++) {
        compute_forces(position_x, position_y, mass, velocity_x, velocity_y, brightness, force_x, force_y, N);
        for (int i = 0; i < N; i++) {
            update_particle(&position_x[i], &position_y[i], &mass[i], &velocity_x[i], &velocity_y[i], &brightness[i], &force_x[i], &force_y[i], delta_t);
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