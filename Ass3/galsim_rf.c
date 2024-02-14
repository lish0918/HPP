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

    char filepath[100];

    strcpy(filepath, "input_data/");
    strcat(filepath, filename);
    FILE *file = fopen(filepath, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        exit(1);
    }

    // Read the particles from the file
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
        for (int i = 0; i < N; i++) {
		        forces[i].force_x = 0.0;
		        forces[i].force_y = 0.0;
		
		        double position_x_i = particles[i].position_x;
		        double position_y_i = particles[i].position_y;
		        double mass_i = particles[i].mass;
		
		        for (int j = i + 1; j < N; j++) {
		            double dx = position_x_i - particles[j].position_x;
		            double dy = position_y_i - particles[j].position_y;
		            double distance_squared = sqrt(dx * dx + dy * dy) + EPSILON;
		            double distance_cubed = distance_squared * distance_squared * distance_squared;
		            double force_magnitude = -G * mass_i * particles[j].mass / distance_cubed;
		            forces[i].force_x += force_magnitude * dx;
		            forces[i].force_y += force_magnitude * dy;
		
		            forces[j].force_x -= force_magnitude * dx;
		            forces[j].force_y -= force_magnitude * dy;
		        }
		    }

            // Update particle position and velocity
            double delta_t_mass = delta_t / particles->mass;
			particles->velocity_x += delta_t_mass * forces->force_x;
			particles->velocity_y += delta_t_mass * forces->force_y;
			particles->position_x += delta_t * particles->velocity_x;
			particles->position_y += delta_t * particles->velocity_y;
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

int main() {
    // Read input from user
    int N, nsteps;
    double delta_t;
    char filename[100];

    printf("Enter the number of particles: ");
    scanf("%d", &N);

    printf("Enter the filename: ");
    scanf("%s", filename);

    printf("Enter the number of steps: ");
    scanf("%d", &nsteps);

    printf("Enter the time step: ");
    scanf("%lf", &delta_t);

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
