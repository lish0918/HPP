#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define G 100.0
#define EPSILON 0.001

typedef struct {
    double x;
    double y;
} Vector;

typedef struct {
    double mass;
    Vector position;
    Vector velocity;
    double brightness;
    Vector force;
} Particle;

void read_initial_configuration(const char *filename, Particle *particles, int N) {
    // Implement function to read initial configuration from file

    char filepath[100];
    strcpy(filepath, "input_data/");
    strcat(filepath, filename);
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        exit(1);
    }

    for (int i = 0; i < N; i++) {
        fscanf(file, "%lf %lf %lf %lf %lf %lf", &particles[i].mass, &particles[i].position.x, &particles[i].position.y, &particles[i].velocity.x, &particles[i].velocity.y, &particles[i].brightness);
    }

    fclose(file);
}

void compute_forces(Particle *particles, int N) {
    // Implement function to compute forces between particles
    for (int i = 0; i < N; i++) {
        particles[i].force.x = 0;
        particles[i].force.y = 0;
        for (int j = 0; j < N; j++) {
            if (i != j) {
                double dx = particles[j].position.x - particles[i].position.x;
                double dy = particles[j].position.y - particles[i].position.y;
                if (dx != 0 || dy != 0) {
                    double dr2 = dx * dx + dy * dy + EPSILON;
                    double dr = sqrt(dr2);
                    double F = G * particles[i].mass * particles[j].mass / dr2;
                    particles[i].force.x += F * dx / dr;
                    particles[i].force.y += F * dy / dr;
                }
            }
        }
    }
}

void update_particle(Particle *particle, double delta_t) {
    // Implement function to update position and velocity of a particle
    particle->velocity.x += delta_t * particle->force.x / particle->mass;
    particle->velocity.y += delta_t * particle->force.y / particle->mass;
    particle->position.x += delta_t * particle->velocity.x;
    particle->position.y += delta_t * particle->velocity.y;
}

void simulate(Particle *particles, int N, int nsteps, double delta_t) {
    for (int step = 0; step < nsteps; step++) {
        compute_forces(particles, N);
        for (int i = 0; i < N; i++) {
            update_particle(&particles[i], delta_t);
        }
    }
}

void write_results(const char *filename, Particle *particles, int N) {
    // Implement function to write results to file
    FILE *file = fopen(filename, "w");

    for (int i = 0; i < N; i++) {
        fprintf(file, "%lf %lf %lf %lf %lf %lf\n", particles[i].mass, particles[i].position.x, particles[i].position.y, particles[i].velocity.x, particles[i].velocity.y, particles[i].brightness);
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

    read_initial_configuration(filename, particles, N);
    simulate(particles, N, nsteps, delta_t);
    write_results("result.gal", particles, N);

    free(particles);

    return 0;
}
