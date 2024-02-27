#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BoardSize 9
#define BoxSize 3

int DuplicateNumbersinRow(int board[BoardSize][BoardSize], int x, int num) {
    for (int y = 0; y < BoardSize; y++) {
        if (board[x][y] == num) {
            return 1;
        }
    }
    return 0;
}

int DuplicateNumbersinCol(int board[BoardSize][BoardSize], int y, int num) {
    for (int x = 0; x < BoardSize; x++) {
        if (board[x][y] == num) {
            return 1;
        }
    }
    return 0;
}

int DuplicateNumbersinBox(int board[BoardSize][BoardSize], int startX, int startY, int num) {
    int startRow = startX - startX % BoxSize;
    int startCol = startY - startY % BoxSize;
    for (int x = 0; x < BoxSize; x++) {
        for (int y = 0; y < BoxSize; y++) {
            if (board[startRow + x][startCol + y] == num) {
                return 1;
            }
        }
    }
    return 0;
}

int ValidateBoard(int board[BoardSize][BoardSize], int x, int y, int num) {
    if (DuplicateNumbersinRow(board, x, num)) {
        return 0;
    }
    if (DuplicateNumbersinCol(board, y, num)) {
        return 0;
    }
    if (DuplicateNumbersinBox(board, x, y, num)) {
        return 0;
    }
    return 1;
}

int SolveSudoku(int board[BoardSize][BoardSize], int row, int col) {
    if (row == BoardSize - 1 && col == BoardSize) {
        return 1;
    }

    if (col == BoardSize) {
        row++;
        col = 0;
    }

    if (board[row][col] != 0) {
        return SolveSudoku(board, row, col + 1);
    }

    for (int num = 1; num <= BoardSize; num++) {
        if (ValidateBoard(board, row, col, num)) {
            board[row][col] = num;
            if (SolveSudoku(board, row, col + 1)) {
                return 1;
            }
            board[row][col] = 0;
        }
    }
    return 0;
}

void GenerateUniqueBoard(int board[BoardSize][BoardSize]) {
    srand(time(NULL));
    for (int i = 0; i < BoardSize; i++) {
        for (int j = 0; j < BoardSize; j++) {
            board[i][j] = 0;
        }
    }
    SolveSudoku(board, 0, 0);
}

void RemoveNumbers(int board[BoardSize][BoardSize], int cellsToRemove) {
    srand(time(NULL));
    while (cellsToRemove > 0) {
        int x = rand() % BoardSize;
        int y = rand() % BoardSize;
        if (board[x][y] != 0) {
            board[x][y] = 0;
            cellsToRemove--;
        }
    }
}

void WriteBoardToFile(int board[BoardSize][BoardSize], const char *filename) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    for (int i = 0; i < BoardSize; i++) {
        for (int j = 0; j < BoardSize; j++) {
            fprintf(file, "%d ", board[i][j]);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "\n"); // Separate each Sudoku board with an empty line

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number of Sudoku boards to generate>\n", argv[0]);
        return 1;
    }

    int num_boards = atoi(argv[1]);

    printf("Generating %d Sudoku boards...\n", num_boards);

    FILE *output_file = fopen("sudoku_boards.txt", "w");
    if (output_file == NULL) {
        printf("Error opening output file.\n");
        return 1;
    }

    for (int i = 0; i < num_boards; i++) {
        int board[BoardSize][BoardSize];
        GenerateUniqueBoard(board);
        int cellsToRemove = rand() % 30 + 31; // 31-60 cells to remove
        RemoveNumbers(board, cellsToRemove);
        for (int i = 0; i < BoardSize; i++) {
            for (int j = 0; j < BoardSize; j++) {
                fprintf(output_file, "%d ", board[i][j]);
            }
            fprintf(output_file, "\n");
        }
        fprintf(output_file, "\n"); // Separate each Sudoku board with an empty line
    }

    fclose(output_file);
    printf("Sudoku boards generated and saved to %s\n", "sudoku_boards.txt");

    return 0;
}
