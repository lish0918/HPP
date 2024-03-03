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
            fprintf(file, "%d ", board[i][j]);
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
            {1, 11, 4, 20, 25, 15, 10, 21, 8, 18, 3, 12, 9, 6, 2, 19, 7, 13, 16, 5, 14, 17, 22, 23, 24},
            {5, 2, 19, 23, 8, 12, 15, 22, 9, 3, 18, 16, 7, 6, 20, 14, 13, 17, 25, 11, 4, 1, 24, 10, 21},
            {17, 16, 14, 3, 25, 5, 7, 20, 11, 1, 6, 4, 24, 2, 8, 13, 10, 21, 9, 23, 15, 19, 12, 18, 22},
            {16, 7, 21, 8, 4, 2, 11, 13, 25, 10, 22, 24, 5, 3, 18, 19, 1, 6, 15, 9, 23, 17, 20, 14, 12},
            {10, 13, 15, 14, 22, 1, 17, 5, 8, 19, 7, 25, 3, 9, 23, 6, 24, 12, 11, 20, 16, 18, 4, 2, 21},
            {12, 1, 11, 16, 5, 23, 7, 9, 25, 20, 14, 6, 18, 15, 19, 21, 2, 3, 8, 22, 10, 13, 24, 17, 4},
            {8, 19, 20, 15, 13, 25, 9, 3, 18, 17, 1, 11, 22, 24, 14, 4, 5, 23, 2, 7, 21, 16, 6, 10, 12},
            {4, 15, 17, 11, 19, 7, 13, 23, 2, 25, 10, 3, 8, 21, 5, 22, 16, 12, 6, 14, 18, 20, 9, 1, 24},
            {22, 3, 24, 10, 7, 13, 19, 1, 6, 4, 23, 2, 16, 25, 12, 8, 21, 14, 18, 20, 17, 11, 5, 9, 15},
            {21, 6, 25, 2, 14, 24, 8, 18, 16, 5, 9, 7, 1, 12, 15, 10, 4, 22, 19, 3, 13, 23, 11, 20, 17},
            {19, 25, 2, 12, 3, 8, 21, 17, 15, 23, 4, 9, 20, 14, 10, 16, 18, 5, 7, 1, 22, 6, 24, 11, 13},
            {14, 23, 1, 5, 24, 4, 16, 6, 22, 15, 13, 17, 11, 7, 25, 9, 20, 10, 21, 12, 2, 3, 19, 8, 18},
            {18, 24, 3, 22, 11, 21, 5, 8, 19, 12, 2, 1, 6, 10, 7, 15, 23, 16, 14, 17, 25, 9, 13, 4, 20},
            {7, 20, 5, 19, 12, 9, 22, 24, 14, 11, 16, 21, 10, 17, 4, 25, 3, 8, 13, 18, 6, 1, 15, 23, 2},
            {6, 22, 23, 9, 15, 14, 2, 10, 21, 7, 25, 5, 19, 18, 1, 12, 11, 24, 3, 4, 8, 13, 16, 17, 20},
            {25, 21, 9, 18, 1, 10, 4, 16, 7, 24, 8, 14, 23, 20, 3, 11, 15, 17, 22, 13, 12, 5, 2, 6, 19},
            {2, 18, 8, 13, 9, 22, 24, 15, 5, 16, 20, 19, 17, 1, 6, 23, 25, 4, 10, 21, 3, 14, 7, 12, 11},
            {23, 8, 16, 7, 20, 3, 12, 25, 1, 14, 5, 10, 13, 19, 22, 17, 9, 15, 24, 2, 11, 4, 21, 6, 18},
            {24, 9, 22, 6, 17, 11, 14, 2, 4, 21, 19, 8, 25, 23, 16, 3, 12, 1, 5, 15, 20, 7, 18, 13, 10},
            {3, 17, 7, 25, 6, 20, 18, 12, 23, 9, 15, 22, 2, 5, 24, 1, 19, 21, 16, 10, 4, 14, 8, 13, 11},
            {20, 4, 6, 24, 18, 19, 1, 14, 12, 8, 11, 23, 21, 16, 9, 7, 22, 25, 13, 17, 5, 15, 10, 3, 2},
            {9, 10, 13, 17, 2, 18, 25, 4, 3, 6, 12, 15, 14, 22, 21, 5, 8, 19, 1, 24, 7, 11, 23, 20, 16},
            {15, 5, 12, 1, 10, 17, 23, 11, 20, 2, 24, 13, 4, 8, 6, 18, 7, 9, 21, 16, 19, 25, 22, 14, 3},
            {13, 12, 10, 4, 23, 6, 3, 19, 24, 22, 17, 20, 15, 25, 11, 2, 5, 7, 18, 8, 9, 21, 1, 16, 14},
            {11, 14, 18, 21, 16, 25, 6, 7, 13, 1, 5, 9, 12, 4, 17, 20, 24, 2, 23, 19, 22, 10, 3, 15, 8}
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
