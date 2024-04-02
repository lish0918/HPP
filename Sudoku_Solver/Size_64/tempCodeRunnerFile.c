#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100

int main() {
    FILE *inputFile, *outputFile;
    char line[MAX_LINE_LENGTH];

    // 打开输入文件
    inputFile = fopen("1.txt", "r");
    if (inputFile == NULL) {
        printf("无法打开输入文件。\n");
        return 1;
    }

    // 打开输出文件
    outputFile = fopen("2.txt", "w");
    if (outputFile == NULL) {
        printf("无法打开输出文件。\n");
        return 1;
    }

    // 逐行读取输入文件并处理
    while (fgets(line, MAX_LINE_LENGTH, inputFile) != NULL) {
        // 去除行尾的换行符
        line[strcspn(line, "\n")] = '\0';

        // 分隔字符并写入输出文件
        for (int i = 0; i < strlen(line); i++) {
            char c = line[i];
            int num;
            if (c >= '1' && c <= '9') {
                num = c - '0';
            } else if (c >= 'a' && c <= 'z') {
                num = c - 'a' + 10;
            } else if (c >= 'A' && c <= 'Z') {
                num = c - 'A' + 36;
            } else if (c == '?' || c == '$' || c == '#') {
                num = c - '?' + 62;
            } else {
                num = 0; // Invalid character, set to 0
            }
            fprintf(outputFile, "%2d ", num);
        }
        fprintf(outputFile, "\n");
    }

    // 关闭文件
    fclose(inputFile);
    fclose(outputFile);

    printf("处理完成。\n");

    return 0;
}