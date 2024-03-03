#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define BoardSize 25

typedef struct {
    int data[BoardSize * BoardSize];
    int size;
} Board;

typedef struct {
    Board* data[BoardSize * BoardSize];
    int size;
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
        if (board->data[(x - x % 5 + i / 5) * BoardSize + (y - y % 5 + i % 5)] == num) {
            return 0;
        }
    } // Check the 5x5 box

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
    for (int i = 0; i < board->size; i++) {
        newBoard->data[i] = board->data[i];
    }
    newBoard->size = board->size;
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
    for (int i = 0; i < fboard->size; i++) {
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
    for (int n = 1; n <= BoardSize; n++) {
        if (ValidateBoard(fboard, x, y, n)) {
            PushBack(boards, fboard);
        }
    }
    PopFront(boards);
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

void ElinimateDuplicate(BoardQueue* solutions) {
    for (int i = 0; i < QueueSize(solutions); i++) {
        for (int j = i + 1; j < QueueSize(solutions); j++) {
            int flag = 1;
            for (int k = 0; k < BoardSize * BoardSize; k++) {
                if (solutions->data[i]->data[k] != solutions->data[j]->data[k]) {
                    flag = 0;
                    break;
                }
            }
            if (flag) {
                free(solutions->data[j]);
                for (int k = j; k < QueueSize(solutions) - 1; k++) {
                    solutions->data[k] = solutions->data[k + 1];
                }
                solutions->size--;
                j--;
            }
        }
    }
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

int main() {
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

    // Read the boards from the input file
    int num_boards;
    printf("Enter the number of Sudoku boards to solve: ");
    scanf("%d", &num_boards);
    
    BoardQueue* problems = (BoardQueue*)malloc(sizeof(BoardQueue));
    problems->size = num_boards;

    for (int i = 0; i < num_boards; i++) {
        problems->data[i] = (Board*)malloc(sizeof(Board));
        ReadBoardFromFile(problems->data[i], input_file);
        problems->data[i]->size = BoardSize * BoardSize;
    }

    // Start time
    clock_t start = clock();

    while (QueueSize(problems) > 0 && QueueSize(problems) < 10) {
        PartionSolve(problems);
    } // Increase the number of boards to solve

    BoardQueue* solutions = (BoardQueue*)malloc(sizeof(BoardQueue));
    solutions->size = 0;

    int N = QueueSize(problems);
    printf("Solving %d Sudoku boards...\n", N);
    
    #pragma omp parallel for schedule(dynamic) shared(N, problems, solutions)
    for (int i = 0; i < N; i++) {
        if(SolveSudoku(problems->data[i], 0, 0)){
            solutions->data[QueueSize(solutions)] = (Board*)malloc(sizeof(Board));
            PushBack(solutions, problems->data[i]);
        }
    }
    
    ElinimateDuplicate(solutions);
    
    printf("Solved %d Sudoku boards.\n", QueueSize(solutions));
    // Solved number less than Solving number, because some problems have the same solution

    for (int i = 0; i < QueueSize(solutions); i++) {
        WriteBoardToFile(solutions->data[i], output_file);
    }

    FreeBoardQueue(solutions);
    FreeBoardQueue(problems);

    // End time
    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_taken);

    fclose(output_file);
    fclose(input_file);

    return 0;
}
