#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Failed to open file\n");
        return 2;
    }
    for (int i = 2; i < argc; i++) {
        fprintf(fp, "%s\n", argv[i]);
    }
    
    fclose(fp);

    printf("File '%s' created successfully!\n", filename);

    return 0;
}