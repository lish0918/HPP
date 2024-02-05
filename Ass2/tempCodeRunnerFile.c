#include <stdio.h>

int main()
{
    int a;
    double b;
    char c;
    float d;

    FILE *fp = fopen("little_bin_file", "rb");
    if (fp == NULL) {
        printf("Failed to open file\n");
        return 1;
    }

    // Read the data from the file
    fread(&a, sizeof(int), 1, fp);
    fread(&b, sizeof(double), 1, fp);
    fread(&c, sizeof(char), 1, fp);
    fread(&d, sizeof(float), 1, fp);

    // Print the data
    printf("%d\n", a);
    printf("%lf\n", b);
    printf("%c\n", c);
    printf("%f\n", d);

    fclose(fp);

    return 0;
}