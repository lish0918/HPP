#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>

/*when try to switch board sizes, you should modify lines 8,9,113 accrodingly*/
#define BoardSize 25
#define BoxSize 5
#define Threshold 100

//int solution_found = 0;
char solution[BoardSize * BoardSize];

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

int Solve(char board[], int unAssignInd[], int N_unAssign, int level) {
    if (N_unAssign == 0) {
        return 1;
    }
    int index = unAssignInd[N_unAssign - 1];
    int x = index / BoardSize;
    int y = index % BoardSize;

    for (int val = 1; val <= BoardSize; val++) {
        if (ValidateBoard(board, x, y, val)) {
            #pragma omp task firstprivate(board, x, y, val, level) shared(solution) if(level <= Threshold)
            {
                char *copy_board;
                copy_board = (char *)malloc(BoardSize * BoardSize * sizeof(char));
                memcpy(copy_board, board, BoardSize * BoardSize * sizeof(char));
                copy_board[x * BoardSize + y] = val;
                if (Solve(copy_board, unAssignInd, N_unAssign - 1, level + 1)) {                 
                    memcpy(solution, copy_board, BoardSize * BoardSize * sizeof(char));                        
                    #pragma omp cancel taskgroup
                }
                else{
                    free(copy_board);
                }
            }
        }
    }
    #pragma omp taskwait

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

int main(int argc, char** argv) {
//int main() {
    if(argc != 2) {printf("Usage: %s n_threads\n", argv[0]); return -1; }
    int n_threads = atoi(argv[1]);

    FILE *input_file = fopen("file/25_hard_3.txt", "r");
    FILE *output_file = fopen("sudoku_solutions.txt", "w");

    if (input_file == NULL || output_file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    char board[BoardSize * BoardSize];
    int unAssignInd[BoardSize * BoardSize];
    int N_unAssign = 0;

    #ifdef _OPENMP
    omp_set_num_threads(n_threads);
    #endif

    // Start time
    #ifdef _OPENMP
    double start_time = omp_get_wtime(); 
    #endif

    clock_t start = clock();

    N_unAssign = 0;
    ReadBoardFromFile(board, unAssignInd, &N_unAssign, input_file);

    #pragma omp parallel
	#pragma omp single
    {
        #pragma omp taskgroup
        {
            Solve(board, unAssignInd, N_unAssign, 1);
        }
    }
    WriteBoardToFile(solution, output_file);

    // End time
    #ifdef _OPENMP
    double end_time = omp_get_wtime(); 
    double time_wtime = end_time - start_time;
    printf("Time wtime: %f seconds\n", time_wtime);
    #endif

    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_taken);

    fclose(input_file);
    fclose(output_file);

    return 0;
}
