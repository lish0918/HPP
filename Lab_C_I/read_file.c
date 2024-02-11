#include <stdio.h>

int main()
{
    char product[100];
    double price;
    char buffer[100];

    FILE *fp = fopen("data.txt", "r");
    fgets(buffer, 100, fp);
    //fscanf(fp, "%d", &num);
    while (fscanf(fp, "%s %lf", product, &price) != EOF)
    {
        printf("%s %.1f\n", product, price);
    }
    fclose(fp);

    return 0;
}