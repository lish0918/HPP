#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <time.h>
#include <unistd.h>

#define _GNU_SOURCE
#define SIZE 9
#define UNASSIGNED 0

//  compile with 
//                              gcc -fopenmp <filename>.c -o <name>
//Optional set num of threads:  export OMP_NUM_THREADS=4
//                              ./<name>
clock_t start, end;

void print_grid(int grid[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            printf("%2d", grid[row][col]);
        }
        printf("\n");
    }
}

//https://stackoverflow.com/questions/1726302/removing-spaces-from-a-string-in-c
void remove_spaces(char* s) {
    const char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while (*s++ = *d++);
}

int is_exist_row(int grid[SIZE][SIZE], int row, int num){
    for (int col = 0; col < 9; col++) {
        if (grid[row][col] == num) {
            return 1;
        }
    }
    return 0;
}

int is_exist_col(int grid[SIZE][SIZE], int col, int num) {
    for (int row = 0; row < 9; row++) {
        if (grid[row][col] == num) {
            return 1;
        }
    }
    return 0;
}

int is_exist_box(int grid[SIZE][SIZE], int startRow, int startCol, int num) {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (grid[row + startRow][col + startCol] == num) {
                return 1;
            } 
        }
    }
    return 0;
}

int is_safe_num(int grid[SIZE][SIZE], int row, int col, int num) {
    return !is_exist_row(grid, row, num) 
            && !is_exist_col(grid, col, num) 
            && !is_exist_box(grid, row - (row % 3), col - (col %3), num);
}

int find_unassigned(int grid[SIZE][SIZE], int *row, int *col) {
    for (*row = 0; *row < SIZE; (*row)++) {
        for (*col = 0; *col < SIZE; (*col)++) {
            if (grid[*row][*col] == 0) {
                return 1;
            }
        }
    }
    return 0;
}


int solve(int grid[SIZE][SIZE]) {
    
    int row = 0;
    int col = 0;
    
    if (!find_unassigned(grid, &row, &col)){
        return 1;
    }
    
    for (int num = 1; num <= SIZE; num++ ) {        
        if (is_safe_num(grid, row, col, num)) {
            int val = 0;
            #pragma omp task firstprivate(grid, row, col,val,num)
            {
                int copy_grid[SIZE][SIZE];
                for (int row = 0; row < SIZE; row++) {
                    for (int col = 0; col < SIZE; col++) {                      
                        copy_grid[row][col] = grid[row][col];
                    }                   
                }
                
                copy_grid[row][col] = num;              
                val = solve(copy_grid);
                
                if(val) {
                    print_grid(copy_grid);
                    end = clock();
                    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;  
                    printf("\nGelöst in %f s\n",time_spent);                    
                    exit(0);                    
                }
            }                       
            
            if (val) {
                return 1;
            }
            
            grid[row][col] = UNASSIGNED;
            #pragma omp taskwait
        }
    }
    
    return 0;
}

void ReadBoardFromFile(int board[9][9], FILE *file) {
   for (int x = 0; x < 9; x++) {
        for (int y = 0; y < 9; y++) {
            if (fscanf(file, "%d", &board[x][y]) != 1) {
                return;
            }
        }
    }
}

int main(int argc, char** argv) {
    
    int sudoku[SIZE][SIZE];
    
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    
    int i,j;
    i =0;
    file = fopen("Sudoku_boards.txt","r");
    for (int i = 0; i < 3; i++){
        ReadBoardFromFile(sudoku, file);
        
        start = clock();
        printf("Solving Sudoku: \n");
        print_grid(sudoku);
        printf("---------------------\n");
       #pragma omp parallel sections 
       {
           #pragma omp section
           {
               solve(sudoku);   
           }
       }
        //exit(EXIT_SUCCESS);
    } 
    //exit(EXIT_FAILURE);
}