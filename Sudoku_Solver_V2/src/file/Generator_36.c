#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BoardSize 36
#define BoxSize 6

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
    // Go through each cell in the board, making sure the Sudoku is valid
    if (row == BoardSize - 1 && col == BoardSize) {
        return 1;
    } // Reached the end of the board
    if (col == BoardSize) {
        row++;
        col = 0;
    } // Move to the next row
    if (board[row][col] > 0) {
        return SolveSudoku(board, row, col + 1);
    } // Skip the cells that are already filled
    for (int num = 1; num <= BoardSize; num++) {
        if (ValidateBoard(board, row, col, num)) {
            board[row][col] = num;
            if (SolveSudoku(board, row, col + 1)) {
                return 1;
            }
        }
        board[row][col] = 0; // If the number is not valid, reset the cell
    }
    return 0;
}

void SwapBoard(int board[BoardSize][BoardSize]) {
    // Swap rows and columns to get a new board
    int choice[90][2] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {2, 3}, {2, 4}, {2, 5}, {3, 4}, {3, 5}, {4, 5},
                         {6, 7}, {6, 8}, {6, 9}, {6, 10}, {6, 11}, {7, 8}, {7, 9}, {7, 10}, {7, 11}, {8, 9}, {8, 10}, {8, 11}, {9, 10}, {9, 11}, {10, 11},
                         {12, 13}, {12, 14}, {12, 15}, {12, 16}, {12, 17}, {13, 14}, {13, 15}, {13, 16}, {13, 17}, {14, 15}, {14, 16}, {14, 17}, {15, 16}, {15, 17}, {16, 17},
                         {18, 19}, {18, 20}, {18, 21}, {18, 22}, {18, 23}, {19, 20}, {19, 21}, {19, 22}, {19, 23}, {20, 21}, {20, 22}, {20, 23}, {21, 22}, {21, 23}, {22, 23},
                         {24, 25}, {24, 26}, {24, 27}, {24, 28}, {24, 29}, {25, 26}, {25, 27}, {25, 28}, {25, 29}, {26, 27}, {26, 28}, {26, 29}, {27, 28}, {27, 29}, {28, 29},
                         {30, 31}, {30, 32}, {30, 33}, {30, 34}, {30, 35}, {31, 32}, {31, 33}, {31, 34}, {31, 35}, {32, 33}, {32, 34}, {32, 35}, {33, 34}, {33, 35}, {34, 35}};
    int numtoswap = rand() % 24; // The number of swaps
    for (int i = 0; i < numtoswap; i++) {
        if(rand() % 2 == 0){
            // Swap rows
            for (int j = 0; j < BoardSize; j++) {
                int temp = board[choice[i][0]][j];
                board[choice[i][0]][j] = board[choice[i][1]][j];
                board[choice[i][1]][j] = temp;
            }
        }
        else{
            // Swap columns
            for (int j = 0; j < BoardSize; j++) {
                int temp = board[j][choice[i][0]];
                board[j][choice[i][0]] = board[j][choice[i][1]];
                board[j][choice[i][1]] = temp;
            }
        }
    }
}

void RemoveNumbers(int board[BoardSize][BoardSize], int cellsToRemove) {
    while (cellsToRemove > 0) {
        int x = rand() % BoardSize;
        int y = rand() % BoardSize;
        if (board[x][y] != 0) {
            board[x][y] = 0;
            cellsToRemove--;
        }
    }
    // Create a copy of the board
    int testboard[BoardSize][BoardSize];
    for (int i = 0; i < BoardSize; i++) {
        for (int j = 0; j < BoardSize; j++) {
            testboard[i][j] = board[i][j];
        }
    }
    // Check if the board has a unique solution
    if (SolveSudoku(testboard, 0, 0) == 0) {
        // Add some numbers back to the board
        int cellsToAdd = rand() % 5 + 1;
        while (cellsToAdd > 0) {
            int x = rand() % BoardSize;
            int y = rand() % BoardSize;
            if (board[x][y] == 0) {
                int num = rand() % BoardSize + 1;
                if (ValidateBoard(board, x, y, num)) {
                    board[x][y] = num;
                    cellsToAdd--;
                }
            }
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
            fprintf(file, "%2d ", board[i][j]);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "\n"); // Separate each Sudoku board with an empty line

    fclose(file);
}

int main(int argc, char** argv) {

    if(argc != 2) {printf("Usage: %s cellsToRemove\n", argv[0]); return -1; }
    int cellsToRemove = atoi(argv[1]);

    srand(1234); // Set a seed for random number generation
    
    // Start time
    clock_t start = clock();
    int board[BoardSize][BoardSize] = {
            {36, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35},
            {6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 1, 2, 3, 4, 5},
            {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
            {18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17},
            {24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23},
            {30, 31, 32, 33, 34, 35, 36, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29},
            {1, 2, 3, 4, 5, 36, 7, 8, 9, 10, 11, 6, 13, 14, 15, 16, 17, 12, 19, 20, 21, 22, 23, 18, 25, 26, 27, 28, 29, 24, 31, 32, 33, 34, 35, 30},
            {7, 8, 9, 10, 11, 6, 13, 14, 15, 16, 17, 12, 19, 20, 21, 22, 23, 18, 25, 26, 27, 28, 29, 24, 31, 32, 33, 34, 35, 30, 1, 2, 3, 4, 5, 36},
            {13, 14, 15, 16, 17, 12, 19, 20, 21, 22, 23, 18, 25, 26, 27, 28, 29, 24, 31, 32, 33, 34, 35, 30, 1, 2, 3, 4, 5, 36, 7, 8, 9, 10, 11, 6},
            {19, 20, 21, 22, 23, 18, 25, 26, 27, 28, 29, 24, 31, 32, 33, 34, 35, 30, 1, 2, 3, 4, 5, 36, 7, 8, 9, 10, 11, 6, 13, 14, 15, 16, 17, 12},
            {25, 26, 27, 28, 29, 24, 31, 32, 33, 34, 35, 30, 1, 2, 3, 4, 5, 36, 7, 8, 9, 10, 11, 6, 13, 14, 15, 16, 17, 12, 19, 20, 21, 22, 23, 18},
            {31, 32, 33, 34, 35, 30, 1, 2, 3, 4, 5, 36, 7, 8, 9, 10, 11, 6, 13, 14, 15, 16, 17, 12, 19, 20, 21, 22, 23, 18, 25, 26, 27, 28, 29, 24},
            {2, 3, 4, 5, 36, 1, 8, 9, 10, 11, 6, 7, 14, 15, 16, 17, 12, 13, 20, 21, 22, 23, 18, 19, 26, 27, 28, 29, 24, 25, 32, 33, 34, 35, 30, 31},
            {8, 9, 10, 11, 6, 7, 14, 15, 16, 17, 12, 13, 20, 21, 22, 23, 18, 19, 26, 27, 28, 29, 24, 25, 32, 33, 34, 35, 30, 31, 2, 3, 4, 5, 36, 1},
            {14, 15, 16, 17, 12, 13, 20, 21, 22, 23, 18, 19, 26, 27, 28, 29, 24, 25, 32, 33, 34, 35, 30, 31, 2, 3, 4, 5, 36, 1, 8, 9, 10, 11, 6, 7},
            {20, 21, 22, 23, 18, 19, 26, 27, 28, 29, 24, 25, 32, 33, 34, 35, 30, 31, 2, 3, 4, 5, 36, 1, 8, 9, 10, 11, 6, 7, 14, 15, 16, 17, 12, 13},
            {26, 27, 28, 29, 24, 25, 32, 33, 34, 35, 30, 31, 2, 3, 4, 5, 36, 1, 8, 9, 10, 11, 6, 7, 14, 15, 16, 17, 12, 13, 20, 21, 22, 23, 18, 19},
            {32, 33, 34, 35, 30, 31, 2, 3, 4, 5, 36, 1, 8, 9, 10, 11, 6, 7, 14, 15, 16, 17, 12, 13, 20, 21, 22, 23, 18, 19, 26, 27, 28, 29, 24, 25},
            {3, 4, 5, 36, 1, 2, 9, 10, 11, 6, 7, 8, 15, 16, 17, 12, 13, 14, 21, 22, 23, 18, 19, 20, 27, 28, 29, 24, 25, 26, 33, 34, 35, 30, 31, 32},
            {9, 10, 11, 6, 7, 8, 15, 16, 17, 12, 13, 14, 21, 22, 23, 18, 19, 20, 27, 28, 29, 24, 25, 26, 33, 34, 35, 30, 31, 32, 3, 4, 5, 36, 1, 2},
            {15, 16, 17, 12, 13, 14, 21, 22, 23, 18, 19, 20, 27, 28, 29, 24, 25, 26, 33, 34, 35, 30, 31, 32, 3, 4, 5, 36, 1, 2, 9, 10, 11, 6, 7, 8},
            {21, 22, 23, 18, 19, 20, 27, 28, 29, 24, 25, 26, 33, 34, 35, 30, 31, 32, 3, 4, 5, 36, 1, 2, 9, 10, 11, 6, 7, 8, 15, 16, 17, 12, 13, 14},
            {27, 28, 29, 24, 25, 26, 33, 34, 35, 30, 31, 32, 3, 4, 5, 36, 1, 2, 9, 10, 11, 6, 7, 8, 15, 16, 17, 12, 13, 14, 21, 22, 23, 18, 19, 20},
            {33, 34, 35, 30, 31, 32, 3, 4, 5, 36, 1, 2, 9, 10, 11, 6, 7, 8, 15, 16, 17, 12, 13, 14, 21, 22, 23, 18, 19, 20, 27, 28, 29, 24, 25, 26},
            {4, 5, 36, 1, 2, 3, 10, 11, 6, 7, 8, 9, 16, 17, 12, 13, 14, 15, 22, 23, 18, 19, 20, 21, 28, 29, 24, 25, 26, 27, 34, 35, 30, 31, 32, 33},
            {10, 11, 6, 7, 8, 9, 16, 17, 12, 13, 14, 15, 22, 23, 18, 19, 20, 21, 28, 29, 24, 25, 26, 27, 34, 35, 30, 31, 32, 33, 4, 5, 36, 1, 2, 3},
            {16, 17, 12, 13, 14, 15, 22, 23, 18, 19, 20, 21, 28, 29, 24, 25, 26, 27, 34, 35, 30, 31, 32, 33, 4, 5, 36, 1, 2, 3, 10, 11, 6, 7, 8, 9},
            {22, 23, 18, 19, 20, 21, 28, 29, 24, 25, 26, 27, 34, 35, 30, 31, 32, 33, 4, 5, 36, 1, 2, 3, 10, 11, 6, 7, 8, 9, 16, 17, 12, 13, 14, 15},
            {28, 29, 24, 25, 26, 27, 34, 35, 30, 31, 32, 33, 4, 5, 36, 1, 2, 3, 10, 11, 6, 7, 8, 9, 16, 17, 12, 13, 14, 15, 22, 23, 18, 19, 20, 21},
            {34, 35, 30, 31, 32, 33, 4, 5, 36, 1, 2, 3, 10, 11, 6, 7, 8, 9, 16, 17, 12, 13, 14, 15, 22, 23, 18, 19, 20, 21, 28, 29, 24, 25, 26, 27},
            {5, 36, 1, 2, 3, 4, 11, 6, 7, 8, 9, 10, 17, 12, 13, 14, 15, 16, 23, 18, 19, 20, 21, 22, 29, 24, 25, 26, 27, 28, 35, 30, 31, 32, 33, 34},
            {11, 6, 7, 8, 9, 10, 17, 12, 13, 14, 15, 16, 23, 18, 19, 20, 21, 22, 29, 24, 25, 26, 27, 28, 35, 30, 31, 32, 33, 34, 5, 36, 1, 2, 3, 4},
            {17, 12, 13, 14, 15, 16, 23, 18, 19, 20, 21, 22, 29, 24, 25, 26, 27, 28, 35, 30, 31, 32, 33, 34, 5, 36, 1, 2, 3, 4, 11, 6, 7, 8, 9, 10},
            {23, 18, 19, 20, 21, 22, 29, 24, 25, 26, 27, 28, 35, 30, 31, 32, 33, 34, 5, 36, 1, 2, 3, 4, 11, 6, 7, 8, 9, 10, 17, 12, 13, 14, 15, 16},
            {29, 24, 25, 26, 27, 28, 35, 30, 31, 32, 33, 34, 5, 36, 1, 2, 3, 4, 11, 6, 7, 8, 9, 10, 17, 12, 13, 14, 15, 16, 23, 18, 19, 20, 21, 22},
            {35, 30, 31, 32, 33, 34, 5, 36, 1, 2, 3, 4, 11, 6, 7, 8, 9, 10, 17, 12, 13, 14, 15, 16, 23, 18, 19, 20, 21, 22, 29, 24, 25, 26, 27, 28}
    };
    SwapBoard(board);
    RemoveNumbers(board, cellsToRemove);

    char path[100] = "36_";
    if (cellsToRemove <= 100) {
        strcat(path, "easy.txt");
    }
    else if (cellsToRemove <= 300) {
        strcat(path, "medium.txt");
    }
    else {
        strcat(path, "hard.txt");
    }

    FILE *output_file = fopen(path, "w");
    if (output_file == NULL) {
        printf("Error opening output file.\n");
        return 1;
    }

    WriteBoardToFile(board, path);

    // End time
    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_taken);

    fclose(output_file);
    printf("Sudoku boards generated and saved to %s\n", path);

    return 0;
}
