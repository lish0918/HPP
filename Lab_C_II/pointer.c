#include <stdio.h>

void print_int_1(int x) {
printf("Here is the number: %d\n", x);
}

void print_int_2(int x) {
printf("Wow, %d is really an impressive number!\n", x);
}

int main(){
    void (*print_func)(int);

    // 设置函数指针指向 print_int_1 函数，并调用
    print_func = print_int_1;
    print_func(5);

    // 设置函数指针指向 print_int_2 函数，并调用
    print_func = print_int_2;
    print_func(10);

    return 0;
}