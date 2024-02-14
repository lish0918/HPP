#include <stdio.h>
#include <string.h>

typedef struct {
    double position_x;
    double position_y;
    double mass;
    double velocity_x;
    double velocity_y;
    double brightness;
} Particle;

int main() {
    FILE *file;
    
    // Open the file in binary read mode
    file = fopen("result.gal", "rb");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Read the particles from the file
    Particle particles[100]; // Assuming there are 100 particles in the file
    size_t particles_read = fread(particles, sizeof(Particle), 100, file);

    // Print the particles read from the file
    printf("Read %zu particles from file:\n", particles_read);
    for (size_t i = 0; i < particles_read; i++) {
        printf("Particle %zu:\n", i);
        printf("Position: (%lf, %lf)\n", particles[i].position_x, particles[i].position_y);
        printf("Mass: %lf\n", particles[i].mass);
        printf("Velocity: (%lf, %lf)\n", particles[i].velocity_x, particles[i].velocity_y);
        printf("Brightness: %lf\n", particles[i].brightness);
        printf("\n");
    }

    // Close the file
    fclose(file);

    return 0;
}
