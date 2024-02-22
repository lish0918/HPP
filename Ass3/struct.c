/*Final Version*/
/*Using struct*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>  

#define EPSILON 0.001

typedef struct {
    double position_x;
    double position_y;
    double mass;
    double velocity_x;
    double velocity_y;
    double brightness;
} Particle;

typedef struct{
    double force_x;
    double force_y;
}Force;

void read_initial_configuration(const char *filename, Particle *particles, int N) {
    // Implement function to read initial configuration from file

    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %\n", filename);
        exit(1);
    }

    size_t particles_read = fread(particles, sizeof(Particle), N, file);
    if (particles_read == 0) {
        fprintf(stderr, "Error: Could not read from file %s\n", filename);
        exit(1);
    }

    fclose(file);
}


void simulate(Particle *particles, Force *forces, int N, int nsteps, double delta_t) {
    double G = 100.0/N;
    for (int step = 0; step < nsteps; step++) {
        for (int i = 0; i < N; i++){
            forces[i].force_x = 0.0;
            forces[i].force_y = 0.0;
        } 
        for (int i = 0; i < N; i++) {
            double position_x_i = particles[i].position_x;
            double position_y_i = particles[i].position_y;
            double mass_i = particles[i].mass;

            double forcex = forces[i].force_x;
            double forcey = forces[i].force_y;

            for (int j = i + 1; j < N; j++) {
                double dx = position_x_i - particles[j].position_x;
                double dy = position_y_i - particles[j].position_y;
                double distance_squared = sqrt(dx * dx + dy * dy) + EPSILON;
                double distance_cubed = distance_squared * distance_squared * distance_squared;
                double force_magnitude = -G * mass_i * particles[j].mass / distance_cubed;
                forcex += force_magnitude * dx;
                forcey += force_magnitude * dy;

                forces[j].force_x -= force_magnitude * dx;
                forces[j].force_y -= force_magnitude * dy;
            }
            forces[i].force_x = forcex;
            forces[i].force_y = forcey;
        }
        for(int i = 0; i < N; i++){
            double delta_t_mass = delta_t / particles[i].mass;
            particles[i].velocity_x += delta_t_mass * forces[i].force_x;
            particles[i].velocity_y += delta_t_mass * forces[i].force_y;
            particles[i].position_x += delta_t * particles[i].velocity_x;
            particles[i].position_y += delta_t * particles[i].velocity_y;
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
    if (argc != 6) {
        printf("Usage: %s N filename nsteps delta_t graphics\n", argv[0]);
        return 1;
    }
    int N = atoi(argv[1]);
    char *filename = argv[2];
    int nsteps = atoi(argv[3]);
    double delta_t = atof(argv[4]);
    int graphics = atoi(argv[5]);

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
    simulate(particles, forces, N, nsteps, delta_t);
    write_results("result.gal", particles, N);

    free(particles);
    free(forces);

    return 0;
}