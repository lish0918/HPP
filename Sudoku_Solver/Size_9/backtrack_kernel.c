#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <stdlib.h>
#include <string.h>

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

int lineCount(FILE *file) {
    int count = 0;
    char c;
    for (c = getc(file); c != EOF; c = getc(file)) {
        if (c == '\n') {
            count++;
        }
    }
    rewind(file);
    return count;
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Usage: %s <number_of_threads>\n", argv[0]);
        return 1;
    }
    int num_threads = atoi(argv[1]);

    FILE *input_file = fopen("sudoku_boards.txt", "r");
    FILE *output_file = fopen("sudoku_solutions.txt", "w");

    if (input_file == NULL || output_file == NULL) {
        printf("Failed to open file.\n");
        return 1;
    }

    int num_boards = lineCount(input_file) / (BoardSize + 1); // Number of boards in the input file
    int boards[num_boards][BoardSize][BoardSize];
    int unAssignInd[num_boards][BoardSize * BoardSize];
    int N_unAssign[num_boards];

    // Read all boards into memory
    for (int i = 0; i < num_boards; i++) {
        N_unAssign[i] = 0;
        for (int j = 0; j < BoardSize; j++) {
            for (int k = 0; k < BoardSize; k++) {
                fscanf(input_file, "%d", &boards[i][j][k]);
                if (boards[i][j][k] == 0) {
                    unAssignInd[i][N_unAssign[i]++] = j * BoardSize + k; // Increment the count of unassigned cells
                }
            }
        }
    }

    // Start time
    clock_t start = clock();

    // Parallelize the loop with OpenMP
    #pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < num_boards; i++)
        Solve(boards[i], unAssignInd[i], N_unAssign[i]);   

    for (int i = 0; i < num_boards; i++) {
        for (int j = 0; j < BoardSize; j++) {
            for (int k = 0; k < BoardSize; k++) {
                fprintf(output_file, "%d ", boards[i][j][k]);
            }
            fprintf(output_file, "\n");
        }
        fprintf(output_file, "\n"); // Separate each Sudoku solution with an empty line
    }
            
    // End time
    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_taken);

    fclose(input_file);
    fclose(output_file);

    return 0;
}
