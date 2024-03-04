#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define BoardSize 9
#define BoxSize 3

int DuplicateNumbersinRow(int board[], int x, int num) {
    for (int y = 0; y < BoardSize; y++) {
        if (board[x * BoardSize + y] == num) {
            return 1;
        }
    }
    return 0;
}

int DuplicateNumbersinCol(int board[], int y, int num) {
    for (int x = 0; x < BoardSize; x++) {
        if (board[x * BoardSize + y] == num) {
            return 1;
        }
    }
    return 0;
}

int DuplicateNumbersinBox(int board[], int startRow, int startCol, int num) {
    for (int x = 0; x < BoxSize; x++) {
        for (int y = 0; y < BoxSize; y++) {
            if (board[(startRow + x) * BoardSize + (startCol + y)] == num) {
                return 1;
            }
        }
    }
    return 0;
}

int ValidateBoard(int board[], int x, int y, int num) {
    if (DuplicateNumbersinRow(board, x, num) || DuplicateNumbersinCol(board, y, num) ||
        DuplicateNumbersinBox(board, x - x % BoxSize, y - y % BoxSize, num)) {
        return 0;
    }
    return 1;
}

int Solve(int board[], int unAssignInd[], int N_unAssign) {
    if (N_unAssign == 0) {
        return 1;
    }
    int index = unAssignInd[N_unAssign - 1];
    int x = index / BoardSize;
    int y = index % BoardSize;
    for (int val = 1; val <= BoardSize; val++) {
        if (ValidateBoard(board, x, y, val)) {
            board[x * BoardSize + y] = val; 
            if (Solve(board, unAssignInd, N_unAssign - 1)) {
                return 1;
            }
            board[x * BoardSize + y] = 0; // Reset the value for backtracking
        }
    }
    return 0;
}

void ReadBoardFromFile(int board[], int unAssignInd[], int *N_unAssign, FILE *file) {
    *N_unAssign = 0; // Initialize the number of unassigned cells

    for (int x = 0; x < BoardSize; x++) {
        for (int y = 0; y < BoardSize; y++) {
            if (fscanf(file, "%d", &board[x * BoardSize + y]) != 1) {
                return;
            }
            if (board[x * BoardSize + y] == 0) {
                unAssignInd[(*N_unAssign)++] = x * BoardSize + y; // Increment the count of unassigned cells
            }
        }
    }
}

void WriteBoardToFile(int board[], FILE *file) {
    for (int i = 0; i < BoardSize; i++) {
        for (int j = 0; j < BoardSize; j++) {
            fprintf(file, "%2d ", board[i * BoardSize + j]);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "\n"); // Separate each Sudoku solution with an empty line
}

int main(int argc, char** argv) {

    if(argc != 2) {printf("Usage: %s num_sudokus\n", argv[0]); return -1; }
    int num_sudokus = atoi(argv[1]);

    FILE *input_file = fopen("sudoku_boards.txt", "r");
    FILE *output_file = fopen("sudoku_solutions.txt", "w");

    if (input_file == NULL || output_file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    int board[BoardSize * BoardSize];
    int unAssignInd[BoardSize * BoardSize];
    int N_unAssign = 0;

    // Start time
    clock_t start = clock();
    for (int i = 0; i < num_sudokus; i++) {
        N_unAssign = 0;
        ReadBoardFromFile(board, unAssignInd, &N_unAssign, input_file);

        if (Solve(board, unAssignInd, N_unAssign)) {
            WriteBoardToFile(board, output_file);
        } else {
            printf("No solution found for one of the boards.\n");
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
