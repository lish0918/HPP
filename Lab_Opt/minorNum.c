#include <stdio.h>

int main() {
    float epsilon = 1.0;

    // 不断将 epsilon 除以 2，直到达到精度极限
    while (1.0 + epsilon > 1.0) {
        epsilon /= 2.0;

        if (1.0 + epsilon == 1.0) {
            printf("%.18f: 1 + epsilon = 1 within floating point precision\n", epsilon);
        } else {
            printf("%.18f: 1 + epsilon != 1 within floating point precision\n", epsilon);
        }
    }

    return 0;
}
