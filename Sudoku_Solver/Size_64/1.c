#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1000

int main() {
    FILE *inputFile, *outputFile;
    char inputFileName[] = "3.txt";
    char outputFileName[] = "4.txt";
    char line[MAX_LINE_LENGTH];

    // 打开输入文件
    inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        printf("无法打开输入文件 %s\n", inputFileName);
        return 1;
    }

    // 打开输出文件
    outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        printf("无法打开输出文件 %s\n", outputFileName);
        return 1;
    }

    // 逐行读取输入文件的内容，并进行处理
    while (fgets(line, MAX_LINE_LENGTH, inputFile) != NULL) {
        // 在每行前添加"{"
        fputs("{", outputFile);

        // 分割每个数字，并在数字之间添加", "
        char *token = strtok(line, " ");
        while (token != NULL) {
            fputs(token, outputFile);
            token = strtok(NULL, " ");
            if (token != NULL) {
                fputs(", ", outputFile);
            }
        }

        // 在每行后添加"},"
        fputs("},\n", outputFile);
    }

    // 关闭文件
    fclose(inputFile);
    fclose(outputFile);

    printf("处理完成，结果已写入到文件 %s\n", outputFileName);

    return 0;
}
