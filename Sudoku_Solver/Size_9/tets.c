#include <stdio.h>
#include <time.h>
#include <omp.h>

#define BoardSize 16

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
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (board[startRow + x][startCol + y] == num) {
                return 1;
            }
        }
    }
    return 0;
}

int ValidateBoard(int board[BoardSize][BoardSize], int x, int y, int num) {
    if (DuplicateNumbersinRow(board, x, num) || DuplicateNumbersinCol(board, y, num) ||
        DuplicateNumbersinBox(board, x - x % 4, y - y % 4, num)) {
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
    for (int val = 1; val <= BoardSize; val++) {
        if (ValidateBoard(board, x, y, val)) {
            board[x][y] = val; // Set guess
            // Solve recursively
            if (Solve(board, unAssignInd, N_unAssign - 1)) {
                return 1;
            }
            // If the recursive call didn't return a solution, reset the current cell and try the next value
            board[x][y] = 0; // Reset the value for backtracking
        }
    }
    return 0;
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

void WriteBoardToFile(int board[BoardSize][BoardSize], FILE *file) {
    for (int i = 0; i < BoardSize; i++) {
        for (int j = 0; j < BoardSize; j++) {
            fprintf(file, "%d ", board[i][j]);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "\n"); // Separate each Sudoku solution with an empty line
}

int main() {
    FILE *input_file = fopen("sudoku_boards.txt", "r");
    FILE *output_file = fopen("sudoku_solutions.txt", "w");

    if (input_file == NULL || output_file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    int board[BoardSize][BoardSize];
    int unAssignInd[BoardSize * BoardSize];
    int N_unAssign = 0;
    //int num_sudokus = 1;

    ReadBoardFromFile(board, unAssignInd, &N_unAssign, input_file);

    // Start time
    clock_t start = clock();
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < N_unAssign; i++) {
        int x = unAssignInd[i] / BoardSize;
        int y = unAssignInd[i] % BoardSize;

        for (int num = 1; num <= BoardSize; num++) {
            if (ValidateBoard(board, x, y, num)) {
                #pragma omp critical
                {
                    board[x][y] = num;
                }
                if (Solve(board, unAssignInd, N_unAssign - 1)){
                    #pragma omp critical
                    {
                        WriteBoardToFile(board, output_file);
                        printf("Solution found\n");
                    }
                }
                #pragma omp critical
                {
                    board[x][y] = 0;
                }
            }
        }
    }
    // End time
    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_taken);

    fclose(input_file);
    fclose(output_file);

    return 0;
}
