#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int CmpString(const void *p1, const void *p2){
    // 转换指针类型并解引用
    char *str1 = *(char**)p1;
    char *str2 = *(char**)p2;

    // 使用 strcmp 函数比较字符串
    return strcmp(str1, str2);
}

int main(){
    char *arrStr[] = {"daa", "cbab", "bbbb", "bababa", "ccccc", "aaaa"};
    int arrStrLen = sizeof(arrStr) / sizeof(char *);
    qsort(arrStr, arrStrLen, sizeof(char *), CmpString);
    
    for (int i = 0; i < arrStrLen; i++){
        printf("%s\n", arrStr[i]);
    }

    return 0;
}