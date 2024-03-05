#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

/*when try to switch board sizes, you should modify lines 7,8 accrodingly*/
#define BoardSize 25
#define BoxSize 5
#define Max_Sudoku 10000 //Maximum number of Sudoku problems that can be entered

typedef struct {
    int data[BoardSize * BoardSize];
} Board;

typedef struct {
    Board* data[Max_Sudoku];
    int size;
    int solved;
} BoardQueue;

int ValidateBoard(Board* board, int x, int y, int num) {
    for (int i = 0; i < BoardSize; i++) {
        if (board->data[x * BoardSize + i] == num) {
            return 0;
        }
        if (board->data[i * BoardSize + y] == num) {
            return 0;
        }
    }

    for (int i = 0; i < BoardSize; i++) {
        if (board->data[(x - x % BoxSize + i / BoxSize) * BoardSize + (y - y % BoxSize + i % BoxSize)] == num) {
            return 0;
        }
    } // Check the box

    return 1;
}

void FreeBoardQueue(BoardQueue* boards) {
    for (int i = 0; i < boards->size; i++) {
        free(boards->data[i]);
    }
    free(boards);
}

void PushBack(BoardQueue* boards, const Board* board) {
    Board* newBoard = (Board*)malloc(sizeof(Board));
    for (int i = 0; i < BoardSize * BoardSize; i++) {
        newBoard->data[i] = board->data[i];
    }
    boards->data[boards->size++] = newBoard;
} // Pushes a board to the back of the queue

void PopFront(BoardQueue* boards) {
    free(boards->data[0]);
    for (int i = 0; i < boards->size - 1; i++) {
        boards->data[i] = boards->data[i + 1];
    }
    boards->size--;
} // Pops the front board from the queue

int QueueSize(const BoardQueue* boards) {
    return boards->size;
}

void PartionSolve(BoardQueue* boards) {
    if (QueueSize(boards) <= 0) {
        return;
    }
    Board* fboard = boards->data[0];
    int index = -1;
    for (int i = 0; i < BoardSize * BoardSize; i++) {
        if (fboard->data[i] == 0) {
            index = i;
            break;
        }
    }
    if (index < 0) {
        PushBack(boards, fboard);
        PopFront(boards);
        return;
    }
    int x = index / BoardSize;
    int y = index % BoardSize;
    for (int num = 1; num <= BoardSize; num++) {
        if (ValidateBoard(fboard, x, y, num)) {
            PushBack(boards, fboard);
        }
    }
    PopFront(boards);
}

void Shuffle(BoardQueue* boards) {
    BoardQueue* tboards = (BoardQueue*)malloc(sizeof(BoardQueue));
    tboards->size = boards -> size;

    for (int i = 0; i < boards->size; ++i) {
        tboards->data[i] = (Board*)malloc(sizeof(Board));
        tboards->data[i] = boards->data[i];
        boards->data[i] = NULL;
    }

    srand(1234); // Set random seed
    for (int i = 0; i < boards->size - 1; ++i) {
        int j = i + rand() / (RAND_MAX / (boards->size - i) + 1);
        Board* temp = tboards->data[j];
        tboards->data[j] = tboards->data[i];
        tboards->data[i] = temp;
    }

    for (int i = 0; i < boards->size; ++i) {
        boards->data[i] = tboards->data[i];
    }

    FreeBoardQueue(tboards);
}


int SolveSudoku(Board* board, int x, int y) {
    if (x == BoardSize - 1 && y == BoardSize) {
        return 1;
    }
    if (y == BoardSize) {
        x++;
        y = 0;
    }
    if (board->data[x * BoardSize + y] != 0) {
        return SolveSudoku(board, x, y + 1);
    }
    for (int num = 1; num <= BoardSize; num++) {
        if (ValidateBoard(board, x, y, num)) {
            board->data[x * BoardSize + y] = num;
            if (SolveSudoku(board, x, y + 1)) {
                return 1;
            }
            board->data[x * BoardSize + y] = 0;
        }
    }
    return 0;
}

void WriteBoardToFile(Board* board, FILE *file) {
    for (int x = 0; x < BoardSize; x++) {
        for (int y = 0; y < BoardSize; y++) {
            fprintf(file, "%2d ", board->data[x * BoardSize + y]);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "\n"); 
}

void ReadBoardFromFile(Board* board, FILE *file) {
    for (int x = 0; x < BoardSize; x++) {
        for (int y = 0; y < BoardSize; y++) {
            if (fscanf(file, "%d", &board->data[x * BoardSize + y]) != 1) {
                return;
            }
        }
    }
}

int main(int argc, char** argv) {

    if(argc != 2) {printf("Usage: %s num_boards\n", argv[0]); return -1; }
    int num_boards = atoi(argv[1]);

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
    
    BoardQueue** problems = (BoardQueue**)malloc(num_boards * sizeof(BoardQueue*));
    for (int i = 0; i < num_boards; i++) {
        problems[i] = (BoardQueue*)malloc(sizeof(BoardQueue));
        problems[i]->size = 1;
        problems[i]->solved = 0;
        problems[i]->data[0] = (Board*)malloc(sizeof(Board));
        ReadBoardFromFile(problems[i]->data[0], input_file);
    }

    BoardQueue* solutions = (BoardQueue*)malloc(sizeof(BoardQueue));
    solutions->size = 0;

    for (int i = 0; i < num_boards; i++) {
        PartionSolve(problems[i]);
    }

    /*Checking one guess is an independent parallel task but two tasks can not work 
    on the same board at the same time. When the solution is found, no more tasks should be 
    issued and previous tasks cancelled. Note, tasks can be created inside tasks to create enough 
    work for the threads in the task queue but too small tasks should be avoided (due to 
    memory and task overhead).*/

    int num_partions = 1;
    omp_set_num_threads(4);

    // Start time
    double start_time = omp_get_wtime();  
    clock_t start = clock();  

    while(solutions->size != num_boards){
        #pragma omp parallel for schedule(dynamic)// shared(solutions)
        for (int i = 0; i < num_boards; i++) {
            if(problems[i]->solved == 0){
                if (SolveSudoku(problems[i]->data[0], 0, 0)) {
                    solutions->data[i] = (Board*)malloc(sizeof(Board));
                    PushBack(solutions, problems[i]->data[0]);
                    problems[i]->solved = 1;
                }
                else if (num_partions < num_boards % 10){
                    PopFront(problems[i]);
                    PartionSolve(problems[i]);
                    Shuffle(problems[i]);
                }
            }
        }
        num_partions++;
    }

    // End time
    double end_time = omp_get_wtime();  // End time using omp_get_wtime()
    double time_wtime = end_time - start_time;
    printf("Time wtime: %f seconds\n", time_wtime);

    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_taken);
    
    printf("Solved %d Sudoku boards.\n", QueueSize(solutions));

    for (int i = 0; i < num_boards; i++) {
        WriteBoardToFile(solutions->data[i], output_file);
    }

    for (int i = 0; i < num_boards; i++) {       
        FreeBoardQueue(problems[i]);
    }
    FreeBoardQueue(solutions);

    fclose(output_file);
    fclose(input_file);

    return 0;
}
