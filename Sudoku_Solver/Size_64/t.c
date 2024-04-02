#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <time.h>
#include <unistd.h>

#define BoardSize 25
#define BoxSize 5

int solution[BoardSize * BoardSize];

void PrintBoard(int board[BoardSize * BoardSize]) {
    for (int x = 0; x < BoardSize; x++) {
        for (int y = 0; y < BoardSize; y++) {
            printf("%2d ", board[x * BoardSize + y]);
        }
        printf("\n");
    }
}

int ValidateBoard(int board[BoardSize * BoardSize], int x, int y, int num) {
    for (int i = 0; i < BoardSize; i++) {
        if (board[x * BoardSize + i] == num) {
            return 0;
        }
        if (board[i * BoardSize + y] == num) {
            return 0;
        }
    }
    for (int i = 0; i < BoardSize; i++) {
        if (board[(x - x % BoxSize + i / BoxSize) * BoardSize + (y - y % BoxSize + i % BoxSize)] == num) {
            return 0;
        }
    }
    return 1;
}

int find_unassigned(int board[BoardSize * BoardSize], int *x, int *y) {
    for (*x = 0; *x < BoardSize; (*x)++) {
        for (*y = 0; *y < BoardSize; (*y)++) {
            if (board[(*x) * BoardSize + *y] == 0) {
                return 1;
            }
        }
    }
    return 0;
}

void CopyBoard(int dest[BoardSize * BoardSize], int src[BoardSize * BoardSize]) {
    for (int i = 0; i < BoardSize * BoardSize; i++) {
        dest[i] = src[i];
    }
}

int Solve(int board[BoardSize * BoardSize], int unAssignInd[], int N_unAssign, int level) {
    if (N_unAssign == 0) {
        return 1;
    }
    int index = unAssignInd[N_unAssign - 1];
    int x = index / BoardSize;
    int y = index % BoardSize;
    
    for (int val = 1; val <= BoardSize; val++ ) {        
        if (ValidateBoard(board, x, y, val)) {            		
            #pragma omp task default(none) firstprivate(board, x, y, val, level) final(level > 1)
            {			
				int copy_board[BoardSize * BoardSize];			
				//memcpy(copy_board,board,BoardSize*BoardSize*sizeof(int));				                
				CopyBoard(copy_board, board);
                copy_board[x * BoardSize + y] = val;          
                if(Solve(copy_board, unAssignInd, N_unAssign - 1, level + 1)) {
                    #pragma omp critical
                    {
                        CopyBoard(solution, copy_board);
                        //memcpy(solution, copy_board, BoardSize * BoardSize * sizeof(int));
                    }
                }
            }                       
                   
        }
    }
    #pragma omp taskwait
    return 0;
}

void WriteBoardToFile(int board[BoardSize * BoardSize], FILE *file) {
    for (int x = 0; x < BoardSize; x++) {
        for (int y = 0; y < BoardSize; y++) {
            fprintf(file, "%2d ", board[x * BoardSize + y]);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "\n"); 
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

int main(int argc, char** argv) {
    if(argc != 2) {printf("Usage: %s n_threads\n", argv[0]); return -1; }
    int n_threads = atoi(argv[1]);
    //int n_threads = 4;

    FILE *output_file = fopen("sudoku_solutions.txt", "w");
    FILE *input_file = fopen("sudoku_boards.txt", "r");

    if (input_file == NULL) {
        printf("Error opening input file.\n");
        return 1;
    }

    if (output_file == NULL) {
        printf("Error opening output file.\n");
        fclose(input_file);
        return 1;
    }
    
    int problem[BoardSize * BoardSize];
    int unAssignInd[BoardSize * BoardSize];
    int N_unAssign = 0;
    ReadBoardFromFile(problem, unAssignInd, &N_unAssign, input_file);	 
        
    printf("Solving Sudoku: \n");
    PrintBoard(problem);
    printf("\n");
    printf("Solved Sudoku: \n");   	   

    double start_time = omp_get_wtime();  
    clock_t start = clock(); 
    #pragma omp parallel default(none) shared(problem) num_threads(n_threads)
	#pragma omp single nowait
	{
	   Solve(problem, unAssignInd, N_unAssign, 1);   
	}
    PrintBoard(solution);
    WriteBoardToFile(solution, output_file);

    double end_time = omp_get_wtime(); 
    double time_wtime = end_time - start_time;
    printf("Time wtime: %f seconds\n", time_wtime);

    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_taken);

    fclose(input_file);
    fclose(output_file);
    return 0;
}