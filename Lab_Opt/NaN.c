#include <stdio.h>
#include <math.h>

int main() {
    float f = 1.0f; // Initialize a float variable
    double d = 1.0;  // Initialize a double variable

    // Increase the values by 100 times in a loop and print them
    for (int i = 0; i < 100; i++) {
        f *= 100.0f;
        printf("Float: %e\n", f);
    }

    for (int i = 0; i < 200; i++) {
        d *= 100.0;
        printf("Double: %le\n", d);
    }

    // Test for generating "NaN" result
    double negativeNumber = -1.0;
    double result = sqrt(negativeNumber);
    printf("Result of sqrt(-1): %lf\n", result);

    // Test for operations on "inf" or "nan" values
    double infValue = INFINITY;
    double nanValue = NAN;

    // Example: Adding something to "inf" value
    double sum = infValue + 10.0;
    printf("Sum of inf + 10: %lf\n", sum);
    sum = nanValue + 10.0;
    printf("Sum of nan + 10: %lf\n", sum);

    return 0;
}
