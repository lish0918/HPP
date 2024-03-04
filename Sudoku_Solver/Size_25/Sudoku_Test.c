#include <stdio.h>
#include <stdbool.h>

#define BoardSize 25
#define BoxSize 5

bool isSudoku(int matrix[][BoardSize]) {
    // Check each row and column
    for (int i = 0; i < BoardSize; i++) {
        bool rowCheck[BoardSize + 1] = {false};
        bool colCheck[BoardSize + 1] = {false};

        for (int j = 0; j < BoardSize; j++) {
            // Check each row
            if (matrix[i][j] != 0 && rowCheck[matrix[i][j]]) {
                return false;
            }
            rowCheck[matrix[i][j]] = true;

            // Check each column
            if (matrix[j][i] != 0 && colCheck[matrix[j][i]]) {
                return false;
            }
            colCheck[matrix[j][i]] = true;
        }
    }

    // Check each 5x5 block
    for (int blockRow = 0; blockRow < BoardSize; blockRow += BoxSize) {
        for (int blockCol = 0; blockCol < BoardSize; blockCol += BoxSize) {
            bool blockCheck[BoardSize + 1] = {false};

            for (int i = blockRow; i < blockRow + BoxSize; i++) {
                for (int j = blockCol; j < blockCol + BoxSize; j++) {
                    if (matrix[i][j] != 0 && blockCheck[matrix[i][j]]) {
                        return false;
                    }
                    blockCheck[matrix[i][j]] = true;
                }
            }
        }
    }
    return true;
}

void ReadBoardFromFile(int board[BoardSize][BoardSize], int unAssignInd[], int *N_unAssign, FILE *file) {
    *N_unAssign = 0; // Initialize the number of unassigned cells

    for (int x = 0; x < BoardSize; x++) {
        for (int y = 0; y < BoardSize; y++) {
            if (fscanf(file, "%d", &board[x][y]) != 1) {
                return;
            }
            if (board[x][y] == 0) {
                unAssignInd[(*N_unAssign)++] = x * BoardSize + y; // Increment the count of unassigned cells
            }
        }
    }
}

void main() { 
    FILE *file = fopen("sudoku_solutions.txt", "r");
    if (file == NULL) {
        printf("Failed to open file.\n");
        return;
    }

    int board[BoardSize][BoardSize];
    int unAssignInd[BoardSize * BoardSize];
    int N_unAssign = 0;
    int flag = 0;
    int num = 0;

    while (!feof(file)) {
        // Read a solution
        ReadBoardFromFile(board, unAssignInd, &N_unAssign, file);

        // Check if the solution is valid
        if (isSudoku(board)) {
            //printf("The Sudoku solution is valid.\n");
        } else {
            //printf("The Sudoku solution is invalid.\n");
            flag = 1;
        }
        num++;
    }
    if (flag == 0) {
        printf("All %d Sudoku solutions are valid.\n", num - 1);
    } else {
        printf("One or more Sudoku solutions are invalid.\n");
    }

    fclose(file);
}
