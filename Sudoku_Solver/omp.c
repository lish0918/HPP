#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <stdlib.h>

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

int GetunAssignInd(int board[BoardSize][BoardSize]){
    for (int x = 0; x < BoardSize; x++) {
        for (int y = 0; y < BoardSize; y++) {
            if (board[x][y] == 0) {
                return x * 9 + y;
            }
        }
    }
    return -1;
}

int Solve(int board[BoardSize][BoardSize]) {
    int index = GetunAssignInd(board);

    if (index == -1) {
        return 1;
    }

    int x = index / 9;
    int y = index % 9;

    for (int num = 1; num <= BoardSize; num++) {
        if (ValidateBoard(board, x, y, num)) {
            board[x][y] = num;
            if (Solve(board)) {
                return 1;
            }
            board[x][y] = 0;
        }
    }

    return 0; 
}

void ReadBoardFromFile(int board[BoardSize][BoardSize], FILE *file) {
    for (int x = 0; x < BoardSize; x++) {
        for (int y = 0; y < BoardSize; y++) {
            if (fscanf(file, "%d", &board[x][y]) != 1) {
                return;
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

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <number_of_threads> <number_of_Sudokus>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    int num_sudokus = atoi(argv[2]);

    FILE *input_file = fopen("sudoku_boards.txt", "r");
    FILE *output_file = fopen("sudoku_solutions.txt", "w");

    if (input_file == NULL || output_file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    int board[BoardSize][BoardSize];
    clock_t start = clock();

    #pragma omp parallel num_threads(num_threads)
    {
        #pragma omp for
        for (int i = 0; i < num_sudokus; i++) {
            ReadBoardFromFile(board, input_file);
            if (Solve(board)) {
                #pragma omp critical
                {
                    WriteBoardToFile(board, output_file);
                }
            } else {
                printf("No solution found for the board_%d.\n", i);
            }
        }
    }

    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_taken);

    fclose(input_file);
    fclose(output_file);

    return 0;
}
