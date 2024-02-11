#include <stdio.h>
#include <string.h>

typedef struct product
{
char name[50];
double price;
}
product_t;

product_t arr_of_prod[100];

int main()
{
    char product[100];
    double price;
    char buffer[100];

    FILE *fp = fopen("data.txt", "r");
    fgets(buffer, 100, fp);
    int i = 0;
    while (fscanf(fp, "%s %lf", product, &price) != EOF)
    {
        strcpy(arr_of_prod[i].name, product);
        arr_of_prod[i].price = price;
        i++;
    }
    fclose(fp);

    for (int j = 0; j < i; j++)
    {
        printf("%s %.1f\n", arr_of_prod[j].name, arr_of_prod[j].price);
    }

    return 0;
}