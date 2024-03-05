#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

/*when try to switch board sizes, you should modify lines 7,8 accrodingly*/
#define BoardSize 25
#define BoxSize 5

typedef struct Board {
    int data[BoardSize * BoardSize];
    struct Board* next;
}Board;

typedef struct {
    struct Board* list;
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

void FreeBoardQueue(BoardQueue* boards) {
    Board* current = boards->list;
    while (current) {
        Board* next = current->next;
        free(current);
        current = next;
    }
    free(boards);
}

void PushBack(BoardQueue* boards, const Board* board) {
    Board* newBoard = (Board*)malloc(sizeof(Board));
    // Copy the data from the board to the new board
    for (int i = 0; i < BoardSize * BoardSize; i++) {
        newBoard->data[i] = board->data[i];
    }
    newBoard->next = NULL;

    // Add the new board to the end of the list
    Board* last = boards->list;
    if (last) {
        while (last->next) {
            last = last->next;
        }
        last->next = newBoard;
    }
    else {
        boards->list = newBoard;
    }
    boards->size++;
} // Pushes a board to the back of the queue

void PopFront(BoardQueue* boards) {
    if(boards->list == NULL) {
        return;
    }
    Board* temp = boards->list;
    boards->list = boards->list->next;
    free(temp);
    boards->size--;
} // Pops the front board from the queue

int QueueSize(const BoardQueue* boards) {
    return boards->size;
}

void PartionSolve(BoardQueue* boards) {
    if (boards->list == NULL) {
        return;
    }
    Board* fboard = boards->list;
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
    if (boards->size <= 1) {
        return;
    }

    Board** boardArray = (Board**)malloc(boards->size * sizeof(Board*));
    Board* current = boards->list;
    int index = 0;
    while (current != NULL) {
        boardArray[index++] = current;
        current = current->next;
    }

    // Fisher-Yates
    srand(time(NULL)); // Seed the random number generator
    for (int i = boards->size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        // Swap boardArray[i] and boardArray[j]
        Board* temp = boardArray[i];
        boardArray[i] = boardArray[j];
        boardArray[j] = temp;
    }

    // Reconstruct the queue
    boards->list = boardArray[0];
    current = boards->list;
    for (int i = 1; i < boards->size; i++) {
        current->next = boardArray[i];
        current = current->next;
    }
    current->next = NULL;

    free(boardArray);
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

        struct Board *head = (struct Board*)malloc(sizeof(struct Board));
        ReadBoardFromFile(head, input_file);
        problems[i]->list = head;
        head->next = NULL;
    }

    BoardQueue* solutions = (BoardQueue*)malloc(sizeof(BoardQueue));
    solutions->size = 0;

    for (int i = 0; i < num_boards; i++) {
        PartionSolve(problems[i]);
    }

    int num_partions = 1;
    omp_set_num_threads(4);

    // Start time
    double start_time = omp_get_wtime();  
    clock_t start = clock();  

    while(solutions->size != num_boards){
        #pragma omp parallel for schedule(dynamic) shared(solutions)
        for (int i = 0; i < num_boards; i++) {
            if(problems[i]->solved == 0){
                if (SolveSudoku(problems[i]->list, 0, 0)) {
                    solutions->list = (Board*)malloc(sizeof(Board));
                    PushBack(solutions, problems[i]->list);
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
        WriteBoardToFile(solutions->list, output_file);
    }

    for (int i = 0; i < num_boards; i++) {       
        FreeBoardQueue(problems[i]);
    }
    free(problems);
    FreeBoardQueue(solutions);

    fclose(output_file);
    fclose(input_file);

    return 0;
}
