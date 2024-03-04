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
    // There are 9 ways to swap the rows and columns for a 9x9 Sudoku
    int choice[9][2] = {{0, 1}, {0, 2}, {1, 2}, {3, 4}, {3, 5}, {4, 5}, {6, 7}, {6, 8}, {7, 8}};
    int numtoswap = rand() % 9; // The number of swaps
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
            int temp = board[x][y];
            board[x][y] = 0;
            int testboard[BoardSize][BoardSize];
            for (int i = 0; i < BoardSize; i++) {
                for (int j = 0; j < BoardSize; j++) {
                    testboard[i][j] = board[i][j];
                }
            }
            if (SolveSudoku(testboard, 0, 0) != 1) {
                board[x][y] = temp;
            } else {
            cellsToRemove--;
            }
        }
    }
}

void WriteBoardToFile(int board[BoardSize][BoardSize], const char *filename) {
    FILE *file = fopen(filename, "a");

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

    if(argc != 2) {printf("Usage: %s num_boards\n", argv[0]); return -1; }
    int num_boards = atoi(argv[1]);

    printf("Generating %d Sudoku boards...\n", num_boards);

    FILE *output_file = fopen("sudoku_boards.txt", "w");

    srand(1234); // Set a seed for random number generation
    
    // Start time
    clock_t start = clock();
    for (int i = 0; i < num_boards; i++) {
    int board[BoardSize][BoardSize] = {
            {5, 3, 4, 6, 7, 8, 9, 1, 2},
            {6, 7, 2, 1, 9, 5, 3, 4, 8},
            {1, 9, 8, 3, 4, 2, 5, 6, 7},
            {8, 5, 9, 7, 6, 1, 4, 2, 3},
            {4, 2, 6, 8, 5, 3, 7, 9, 1},
            {7, 1, 3, 9, 2, 4, 8, 5, 6},
            {9, 6, 1, 5, 3, 7, 2, 8, 4},
            {2, 8, 7, 4, 1, 9, 6, 3, 5},
            {3, 4, 5, 2, 8, 6, 1, 7, 9}
        };
        SwapBoard(board);
        // Remove 24-28 cells from the board
        int cellsToRemove = rand() % 5 + 24;
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
