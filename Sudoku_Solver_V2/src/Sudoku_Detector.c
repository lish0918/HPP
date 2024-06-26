#include <stdio.h>
#include <stdbool.h>

/*when try to switch board sizes, you should modify lines 5,6 accrodingly*/
#define BoardSize 49
#define BoxSize 7

bool isSudoku(int matrix[][BoardSize]) {
    for (int i = 0; i < BoardSize; i++) {
        bool rowCheck[BoardSize + 1] = {false};
        bool colCheck[BoardSize + 1] = {false};

        for (int j = 0; j < BoardSize; j++) {
            if (matrix[i][j] != 0 && rowCheck[matrix[i][j]]) {
                return false;
            }
            rowCheck[matrix[i][j]] = true;

            if (matrix[j][i] != 0 && colCheck[matrix[j][i]]) {
                return false;
            }
            colCheck[matrix[j][i]] = true;
        }
    }

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
    *N_unAssign = 0; 

    for (int x = 0; x < BoardSize; x++) {
        for (int y = 0; y < BoardSize; y++) {
            if (fscanf(file, "%d", &board[x][y]) != 1) {
                return;
            }
            if (board[x][y] == 0) {
                unAssignInd[(*N_unAssign)++] = x * BoardSize + y; 
            }
        }
    }
}

void main() { 
    FILE *file = fopen("sudoku_solutions.txt", "r"); //
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
        ReadBoardFromFile(board, unAssignInd, &N_unAssign, file);

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
        printf("The Sudoku solution is invalid.\n");
    }

    fclose(file);
}
