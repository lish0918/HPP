#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <time.h>
#include <unistd.h>

#define BoardSize 25
#define BoxSize 5

double start_time;
clock_t start;

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

int IfSolve(int board[BoardSize * BoardSize], int *x, int *y) {
    for (*x = 0; *x < BoardSize; (*x)++) {
        for (*y = 0; *y < BoardSize; (*y)++) {
            if (board[(*x) * BoardSize + *y] == 0) {
                return 0;
            }
        }
    }
    return 1;
}

void CopyBoard(int dest[BoardSize * BoardSize], int src[BoardSize * BoardSize]) {
    for (int i = 0; i < BoardSize * BoardSize; i++) {
        dest[i] = src[i];
    }
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

void ReadBoardFromFile(int board[BoardSize * BoardSize], FILE *file) {
    for (int x = 0; x < BoardSize; x++) {
        for (int y = 0; y < BoardSize; y++) {
            if (fscanf(file, "%d", &board[x * BoardSize + y]) != 1) {
                return;
            }
        }
    }
}

int Solve(int board[BoardSize * BoardSize], int level) {

    int x = 0;
    int y = 0;
    if (IfSolve(board, &x, &y)) return 1; 
    
    #pragma omp taskgroup
    {
        for (int num = 1; num <= BoardSize; num++ ) {        
            if (ValidateBoard(board, x, y, num)) {            		
                #pragma omp task default(none) firstprivate(board, x, y, num, level) shared(start_time, start, solution) final(level > 1)
                {			
                    int copy_board[BoardSize * BoardSize];			
                    //memcpy(copy_board,board,BoardSize*BoardSize*sizeof(int));				                
                    CopyBoard(copy_board, board);
                    copy_board[x * BoardSize + y] = num;          
                    if(Solve(copy_board, level + 1)) {
                        //#pragma omp critical
                        //{
                            //FILE *output_file = fopen("sudoku_solutions.txt", "w");
                            
                            //WriteBoardToFile(copy_board, output_file);
                            //double end_time = omp_get_wtime(); 
                            //double time_wtime = end_time - start_time;
                            //printf("Time wtime: %f seconds\n", time_wtime);

                            //clock_t end = clock();
                            //double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
                            //printf("Time taken: %f seconds\n", time_taken);
                            //exit(0);
                            CopyBoard(solution, copy_board);
                            #pragma omp cancel taskgroup
                        //}
                    }
                }                               
            }
        }
        #pragma omp taskwait
    }
    return 0;
}

int main(int argc, char** argv) {
    if(argc != 2) {printf("Usage: %s n_threads\n", argv[0]); return -1; }
    int n_threads = atoi(argv[1]);
    //int n_threads = 4;
    FILE *input_file = fopen("sudoku_boards.txt", "r");
    FILE *output_file = fopen("sudoku_solutions.txt", "w");

    if (input_file == NULL) {
        printf("Error opening input file.\n");
        return 1;
    }

    if (output_file == NULL) {
        printf("Error opening output file.\n");
        return 1;
    }
    
    int sudoku[BoardSize * BoardSize];
    ReadBoardFromFile(sudoku, input_file);	 
    fclose(input_file);
        
    //printf("Solving Sudoku: \n");
    //PrintBoard(sudoku);
    //printf("\n");
    //printf("Solved Sudoku: \n");   	   

    start_time = omp_get_wtime(); 
    start = clock(); 
    #pragma omp parallel default(none) shared(sudoku) num_threads(n_threads)
	#pragma omp single nowait
	{
	   Solve(sudoku,1);   
	}
    WriteBoardToFile(solution, output_file);
    double end_time = omp_get_wtime(); 
    double time_wtime = end_time - start_time;
    printf("Time wtime: %f seconds\n", time_wtime);

    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_taken);

    return 0;
}