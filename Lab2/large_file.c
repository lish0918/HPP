#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct product
{
char name[50];
double price;
}
product_t;

int main()
{
    char product[100];
    double price;
    int num;

    FILE *fp = fopen("data.txt", "r");
    fscanf(fp, "%d", &num);
    product_t *arr_of_prod = malloc(num * sizeof(num));

    for(int i = 0; i < num; i++)
    {
        fscanf(fp, "%s %lf", product, &price);
        strcpy(arr_of_prod[i].name, product);
        arr_of_prod[i].price = price;
    }
    fclose(fp);

    for (int j = 0; j < num; j++)
    {
        printf("%s %.1f\n", arr_of_prod[j].name, arr_of_prod[j].price);
    }

    return 0;
}