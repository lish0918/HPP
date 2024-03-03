#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BoardSize 25
#define BoxSize 5

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
    // There are 25 ways to swap the rows and columns
    int choice[25][2] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4}, {5, 6}, {5, 7}, {5, 8}, {5, 9}, {6, 7}, {6, 8}, {6, 9}, {7, 8}, {7, 9}, {8, 9}, {10, 11}, {10, 12}, {10, 13}, {10, 14}, {11, 12}};
    int numtoswap = rand() % 25; // The number of swaps
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

int main() {

    int num_boards;
    printf("Enter the number of Sudoku boards to generate: ");
    scanf("%d", &num_boards);

    printf("Generating %d Sudoku boards...\n", num_boards);

    FILE *output_file = fopen("sudoku_boards.txt", "w");
    if (output_file == NULL) {
        printf("Error opening output file.\n");
        return 1;
    }

    srand(1234); // Set a seed for random number generation
    
    // Start time
    clock_t start = clock();
    for (int i = 0; i < num_boards; i++) {
        int board[BoardSize][BoardSize] = {
            {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25},
            {11, 12, 13, 14, 15, 1, 2, 3, 4, 5, 16, 17, 18, 19, 20, 22, 23, 21, 24, 25, 6, 7, 8, 9, 10},
            {6, 7, 8, 9, 10, 24, 25, 22, 21, 23, 1, 2, 3, 4, 5, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20},
            {23, 22, 21, 25, 24, 16, 17, 18, 19, 20, 6, 7, 8, 9, 10, 1, 2, 3, 4, 5, 11, 12, 13, 14, 15},
            {16, 17, 18, 19, 20, 11, 12, 13, 14, 15, 24, 21, 23, 22, 25, 6, 7, 8, 9, 10, 1, 2, 3, 4, 5},
            {3, 1, 5, 2, 14, 19, 16, 20, 7, 11, 18, 15, 21, 24, 23, 8, 10, 25, 22, 12, 4, 9, 17, 13, 6},
            {4, 8, 10, 6, 18, 3, 1, 23, 24, 2, 13, 22, 25, 12, 14, 9, 16, 19, 11, 17, 5, 15, 20, 21, 7},
            {24, 15, 25, 7, 12, 4, 5, 9, 13, 6, 3, 1, 16, 17, 2, 18, 20, 14, 21, 23, 8, 10, 22, 11, 19},
            {9, 19, 11, 20, 21, 8, 22, 25, 17, 14, 4, 5, 10, 7, 6, 3, 1, 15, 13, 2, 18, 24, 16, 12, 23},
            {13, 23, 17, 22, 16, 10, 15, 21, 18, 12, 8, 9, 20, 11, 19, 4, 5, 24, 7, 6, 3, 1, 14, 25, 2},
            {2, 5, 1, 15, 3, 14, 13, 17, 25, 8, 23, 16, 19, 10, 18, 12, 24, 11, 20, 9, 7, 4, 21, 6, 22},
            {7, 16, 6, 11, 4, 2, 18, 1, 5, 3, 12, 24, 17, 8, 22, 14, 21, 23, 10, 13, 20, 25, 19, 15, 9},
            {12, 25, 23, 21, 8, 7, 4, 6, 11, 9, 2, 20, 1, 5, 3, 17, 15, 22, 18, 19, 14, 13, 24, 10, 16},
            {14, 20, 19, 24, 9, 22, 23, 10, 16, 21, 7, 4, 6, 15, 13, 2, 25, 1, 5, 3, 12, 8, 11, 17, 18},
            {17, 10, 22, 18, 13, 12, 24, 15, 20, 19, 14, 25, 11, 21, 9, 7, 4, 6, 8, 16, 2, 23, 1, 5, 3},
            {19, 6, 4, 1, 2, 25, 21, 11, 12, 24, 20, 18, 9, 23, 8, 15, 14, 16, 17, 22, 10, 5, 7, 3, 13},
            {5, 13, 9, 3, 7, 17, 8, 2, 1, 4, 22, 19, 24, 25, 11, 10, 18, 20, 23, 21, 15, 6, 12, 16, 14},
            {15, 18, 20, 23, 17, 5, 6, 7, 3, 16, 10, 14, 2, 1, 4, 19, 13, 9, 12, 24, 22, 21, 25, 8, 11},
            {22, 14, 12, 16, 11, 18, 10, 19, 15, 13, 5, 6, 7, 3, 21, 25, 8, 2, 1, 4, 17, 20, 9, 23, 24},
            {10, 21, 24, 8, 25, 20, 9, 14, 23, 22, 15, 13, 12, 16, 17, 5, 6, 7, 3, 11, 19, 18, 2, 1, 4},
            {18, 3, 2, 5, 1, 15, 20, 4, 8, 25, 17, 10, 22, 13, 24, 23, 11, 12, 16, 14, 9, 19, 6, 7, 21},
            {25, 4, 7, 13, 6, 9, 3, 16, 2, 1, 19, 23, 14, 20, 12, 21, 22, 10, 15, 8, 24, 11, 5, 18, 17},
            {20, 11, 15, 12, 19, 23, 14, 5, 6, 7, 21, 3, 4, 2, 1, 24, 9, 17, 25, 18, 13, 16, 10, 22, 8},
            {8, 9, 16, 17, 22, 21, 19, 24, 10, 18, 25, 11, 5, 6, 7, 13, 3, 4, 2, 1, 23, 14, 15, 20, 12},
            {21, 24, 14, 10, 23, 13, 11, 12, 22, 17, 9, 8, 15, 18, 16, 20, 19, 5, 6, 7, 25, 3, 4, 2, 1}
        };
        SwapBoard(board);
        // Remove 74-78 cells to get a Sudoku puzzle
        int cellsToRemove = rand() % 5 + 74;
        RemoveNumbers(board, cellsToRemove);
        WriteBoardToFile(board, "sudoku_boards.txt");
    }
    // End time
    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_taken);

    fclose(output_file);
    printf("Sudoku boards generated and saved to %s\n", "sudoku_boards.txt");

    return 0;
}
