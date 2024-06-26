#include <stdio.h>
#include <time.h>
#include <stdlib.h>

/*when try to switch board sizes, you should modify lines 6,7,92 accrodingly*/
#define BoardSize 49
#define BoxSize 7

int DuplicateNumbersinRow(char board[], int x, int num) {
    for (int y = 0; y < BoardSize; y++) {
        if (board[x * BoardSize + y] == num) {
            return 1;
        }
    }
    return 0;
}

int DuplicateNumbersinCol(char board[], int y, int num) {
    for (int x = 0; x < BoardSize; x++) {
        if (board[x * BoardSize + y] == num) {
            return 1;
        }
    }
    return 0;
}

int DuplicateNumbersinBox(char board[], int startRow, int startCol, int num) {
    for (int x = 0; x < BoxSize; x++) {
        for (int y = 0; y < BoxSize; y++) {
            if (board[(startRow + x) * BoardSize + (startCol + y)] == num) {
                return 1;
            }
        }
    }
    return 0;
}

int ValidateBoard(char board[], int x, int y, int num) {
    if (DuplicateNumbersinRow(board, x, num) || DuplicateNumbersinCol(board, y, num) ||
        DuplicateNumbersinBox(board, x - x % BoxSize, y - y % BoxSize, num)) {
        return 0;
    }
    return 1;
}

int Solve(char board[], int unAssignInd[], int N_unAssign) {
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

void ReadBoardFromFile(char board[], int unAssignInd[], int *N_unAssign, FILE *file) {
    *N_unAssign = 0; // Initialize the number of unassigned cells

    for (int x = 0; x < BoardSize; x++) {
        for (int y = 0; y < BoardSize; y++) {
            if (fscanf(file, "%hhd", &board[x * BoardSize + y]) != 1) {
                return;
            }
            if (board[x * BoardSize + y] == 0) {
                unAssignInd[(*N_unAssign)++] = x * BoardSize + y; // Increment the count of unassigned cells
            }
        }
    }
}

void WriteBoardToFile(char board[], FILE *file) {
    for (int i = 0; i < BoardSize; i++) {
        for (int j = 0; j < BoardSize; j++) {
            fprintf(file, "%2d ", board[i * BoardSize + j]);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "\n"); // Separate each Sudoku solution with an empty line
}

int main() {

    FILE *input_file = fopen("file/49_medium.txt", "r");
    FILE *output_file = fopen("sudoku_solutions.txt", "w");

    if (input_file == NULL || output_file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    char board[BoardSize * BoardSize];
    int unAssignInd[BoardSize * BoardSize];
    int N_unAssign = 0;

    // Start time
    clock_t start = clock();
    N_unAssign = 0;
    ReadBoardFromFile(board, unAssignInd, &N_unAssign, input_file);

    if (Solve(board, unAssignInd, N_unAssign)) {
        WriteBoardToFile(board, output_file);
    } else {
        printf("No solution found for one of the boards.\n");
    }

    // End time
    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_taken);

    fclose(input_file);
    fclose(output_file);

    return 0;
}
