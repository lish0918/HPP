#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BoardSize 16
#define BoxSize 4

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
    // There are 16 ways to swap the rows and columns
    int choice[16][2] = {{0, 1}, {0, 2}, {0, 3}, {1, 2}, {1, 3}, {2, 3}, {4, 5}, {4, 6}, {4, 7}, {5, 6}, {5, 7}, {6, 7}, {8, 9}, {8, 10}, {9, 10}, {11, 12}};
    int numtoswap = rand() % 16; // The number of swaps
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
            fprintf(file, "%d ", board[i][j]);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "\n"); // Separate each Sudoku board with an empty line

    fclose(file);
}

int main() {
    int num_boards = 1;

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
            {11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 16, 15, 14, 13, 12},
            {9, 16, 8, 6, 5, 1, 2, 12, 11, 15, 14, 3, 4, 13, 7, 10},
            {1, 4, 3, 13, 6, 7, 15, 9, 8, 16, 12, 5, 11, 10, 14, 2},
            {7, 6, 2, 12, 16, 4, 1, 11, 15, 3, 13, 14, 10, 9, 5, 8},
            {2, 15, 6, 1, 14, 12, 3, 13, 10, 11, 9, 16, 8, 5, 4, 7},
            {12, 13, 14, 10, 2, 5, 16, 3, 4, 7, 8, 1, 6, 15, 9, 11},
            {4, 8, 16, 5, 15, 14, 12, 2, 9, 6, 10, 7, 13, 11, 1, 3},
            {3, 9, 7, 11, 1, 16, 6, 15, 13, 14, 2, 8, 12, 4, 10, 5},
            {16, 1, 10, 2, 4, 15, 14, 8, 7, 13, 5, 11, 9, 3, 6, 12},
            {14, 7, 5, 3, 8, 2, 9, 16, 12, 10, 11, 6, 1, 8, 13, 15},
            {6, 12, 15, 7, 13, 11, 8, 1, 5, 9, 4, 10, 3, 16, 2, 14},
            {15, 13, 1, 9, 3, 8, 7, 14, 2, 5, 16, 12, 4, 6, 11, 10},
            {10, 14, 4, 16, 12, 3, 13, 6, 1, 8, 7, 9, 15, 2, 11, 5},
            {5, 3, 2, 15, 9, 10, 16, 7, 6, 12, 1, 13, 14, 8, 11, 4},
            {13, 5, 12, 14, 11, 9, 4, 10, 16, 1, 15, 2, 7, 11, 8, 6},
            {8, 11, 16, 13, 10, 15, 11, 5, 14, 4, 6, 7, 2, 1, 3, 9}
        };
        SwapBoard(board);
        // Remove 50-54 cells from the board
        int cellsToRemove = rand() % 5 + 50;
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
