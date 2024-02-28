/*galsim_test.c:82:27: optimized: loop vectorized using 16 byte vectors
galsim_test.c:47:27: optimized: loop vectorized using 16 byte vectors
galsim_test.c:77:34: optimized: basic block part vectorized using 16 byte vectors*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define EPSILON 0.001

typedef struct {
    double x;
    double y;
} Vector2D;

typedef struct {
    Vector2D position;
    double mass;
    Vector2D velocity;
    double brightness;
    Vector2D force;
} Particle;

void read_initial_configuration(const char *filename, Particle *particles, int N) {
    
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        exit(1);
    }

    for (int i = 0; i < N; i++) {
         size_t particles_read = fread(&particles[i].position, sizeof(Vector2D), 1, file);
        particles_read = fread(&particles[i].mass, sizeof(double), 1, file);
        particles_read = fread(&particles[i].velocity, sizeof(Vector2D), 1, file);
        particles_read = fread(&particles[i].brightness, sizeof(double), 1, file);
    }

    fclose(file);
}

void simulate(Particle *particles, int N, int nsteps, double delta_t) {
    double G = 100.0 / N;

    for (int step = 0; step < nsteps; step++) {
        // Reset forces
        for (int i = 0; i < N; i++) {
            particles[i].force.x = 0.0;
            particles[i].force.y = 0.0;
        }

        // Compute forces
        for (int i = 0; i < N - 1; i++) {
            Vector2D pos_i = particles[i].position;
            double mass_i = particles[i].mass;

            double particles_i_forcex = particles[i].force.x;
            double particles_i_forcey = particles[i].force.y;

            for (int j = i + 1; j < N; j++) {
                Vector2D pos_j = particles[j].position;
                double mass_j = particles[j].mass;

                Vector2D dist;
                dist.x = pos_i.x - pos_j.x;
                dist.y = pos_i.y - pos_j.y;

                double x = dist.x * dist.x + dist.y * dist.y;
                double n = x/2;
                x = x - (x*x - n) / (2*x);
                x = x - (x*x - n) / (2*x);
                x = x - (x*x - n) / (2*x);
                x = x - (x*x - n) / (2*x);
                x = x - (x*x - n) / (2*x);
                x = x - (x*x - n) / (2*x);
                x = x - (x*x - n) / (2*x);
                x = x - (x*x - n) / (2*x);
                x = x - (x*x - n) / (2*x);
                x = x - (x*x - n) / (2*x);

                double distance = x + EPSILON;

                double distance_cubed = distance * distance * distance;
                double force_magnitude = -G * mass_i * mass_j / distance_cubed;

                particles_i_forcex += force_magnitude * dist.x;
                particles_i_forcey += force_magnitude * dist.y;
                particles[j].force.x -= force_magnitude * dist.x; // Newton's third law
                particles[j].force.y -= force_magnitude * dist.y; // Newton's third law
            }
            particles[i].force.x = particles_i_forcex;
            particles[i].force.y = particles_i_forcey;
        }

        // Update positions and velocities
        for (int i = 0; i < N; i++) {
            double delta_t_mass = delta_t / particles[i].mass;
            particles[i].velocity.x += delta_t_mass * particles[i].force.x;
            particles[i].velocity.y += delta_t_mass * particles[i].force.y;
            particles[i].position.x += delta_t * particles[i].velocity.x;
            particles[i].position.y += delta_t * particles[i].velocity.y;
        }
    }
}

void write_results(const char *filename, Particle *particles, int N) {
    FILE *file = fopen(filename, "wb");

    for (int i = 0; i < N; i++) {
        size_t particles_read = fwrite(&particles[i].position, sizeof(Vector2D), 1, file);
        particles_read += fwrite(&particles[i].mass, sizeof(double), 1, file);
        particles_read += fwrite(&particles[i].velocity, sizeof(Vector2D), 1, file);
        particles_read += fwrite(&particles[i].brightness, sizeof(double), 1, file);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
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
        fprintf(stderr, "Error: Memory allocation failed\n");
        return 1;
    }

    read_initial_configuration(filename, particles, N);
    simulate(particles, N, nsteps, delta_t);
    write_results("result.gal", particles, N);

    free(particles);

    return 0;
}
