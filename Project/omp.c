#include <stdio.h>
#include <omp.h>

#define BoardSize 9

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

int DuplicateNumbersinBox(int board[BoardSize][BoardSize], int startRow, int startCol, int num) {
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            if (board[startRow + x][startCol + y] == num) {
                return 1;
            }
        }
    }
    return 0;
}

int ValidateBoard(int board[BoardSize][BoardSize], int x, int y, int num) {
    if (DuplicateNumbersinRow(board, x, num) || DuplicateNumbersinCol(board, y, num) ||
        DuplicateNumbersinBox(board, x - x % 3, y - y % 3, num)) {
        return 0;
    }
    return 1;
}

int Solve(int board[BoardSize][BoardSize], int unAssignInd[], int N_unAssign) {
    if (N_unAssign == 0) {
        // No more empty positions, solution found
        return 1;
    }
    int index = unAssignInd[N_unAssign - 1];
    int x = index / BoardSize;
    int y = index % BoardSize;
    int sol_found = 0;

    #pragma omp parallel for shared(sol_found)
    for (int val = 1; val <= BoardSize && !sol_found; val++) {
        if (ValidateBoard(board, x, y, val)) {
            #pragma omp critical
            {
                board[x][y] = val; // Set guess
            }
            if (Solve(board, unAssignInd, N_unAssign - 1)) {
                sol_found = 1;
            } else {
                #pragma omp critical
                {
                    board[x][y] = 0; // Reset the value for backtracking
                }
            }
        }
    }
    return sol_found;
}

void ReadBoardFromFile(int board[BoardSize][BoardSize], int unAssignInd[], int *N_unAssign) {
    // Read sudoku board from file
    FILE *file = fopen("sudoku_board.txt", "r");
    if (file == NULL) {
        printf("Failed to open file.\n");
        return;
    }

    *N_unAssign = 0; // Initialize the number of unassigned cells

    for (int x = 0; x < BoardSize; x++) {
        for (int y = 0; y < BoardSize; y++) {
            if (fscanf(file, "%d", &board[x][y]) != 1) {
                printf("Failed to read board from file.\n");
                fclose(file);
                return;
            }
            if (board[x][y] == 0) {
                unAssignInd[(*N_unAssign)++] = x * BoardSize + y; // Increment the count of unassigned cells
            }
        }
    }

    fclose(file);
}

void PrintBoard(int board[BoardSize][BoardSize]) {
    printf("-------------------------\n");
    for (int x = 0; x < BoardSize; x++) {
        printf("| ");
        for (int y = 0; y < BoardSize; y++) {
            printf("%d ", board[x][y]);
            if ((y + 1) % 3 == 0) {
                printf("| ");
            }
        }
        printf("\n");
        if ((x + 1) % 3 == 0) {
            printf("-------------------------\n");
        }
    }
}

int main() {
    int board[BoardSize][BoardSize];
    int unAssignInd[BoardSize * BoardSize];
    int N_unAssign = 0;

    ReadBoardFromFile(board, unAssignInd, &N_unAssign);

    if (Solve(board, unAssignInd, N_unAssign)) {
        // Print solved board
        PrintBoard(board);
    } else {
        printf("No solution found.\n");
    }

    return 0;
}
