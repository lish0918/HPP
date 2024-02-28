#include <stdio.h>
#include <stdbool.h>

#define BoardSize 9

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

    // Check each 3x3 block
    for (int blockRow = 0; blockRow < BoardSize; blockRow += 3) {
        for (int blockCol = 0; blockCol < BoardSize; blockCol += 3) {
            bool blockCheck[BoardSize + 1] = {false};

            for (int i = blockRow; i < blockRow + 3; i++) {
                for (int j = blockCol; j < blockCol + 3; j++) {
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
    }
    if (flag == 0) {
        printf("All Sudoku solutions are valid.\n");
    } else {
        printf("One or more Sudoku solutions are invalid.\n");
    }

    fclose(file);
}
