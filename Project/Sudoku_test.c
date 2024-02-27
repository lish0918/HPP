#include <stdio.h>
#include <stdbool.h>

#define BoardSize 9

bool isValidSolution(int board[BoardSize][BoardSize]) {
    bool rowCheck[BoardSize][BoardSize] = {false};
    bool colCheck[BoardSize][BoardSize] = {false};
    bool boxCheck[BoardSize][BoardSize] = {false};

    for (int row = 0; row < BoardSize; row++) {
        for (int col = 0; col < BoardSize; col++) {
            int num = board[row][col] - 1; // Adjust to zero-based indexing

            if (num < 0 || num >= BoardSize)
                return false;

            if (rowCheck[row][num] || colCheck[col][num] || boxCheck[row - row % 3 + col / 3][num])
                return false;

            rowCheck[row][num] = true;
            colCheck[col][num] = true;
            boxCheck[row - row % 3 + col / 3][num] = true;
        }
    }

    return true;
}

void read_solutions(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file.\n");
        return;
    }

    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Read a solution
        int board[BoardSize][BoardSize] = {0};
        for (int row = 0; row < BoardSize; row++) {
            fgets(buffer, sizeof(buffer), file);
            for (int col = 0; col < BoardSize; col++) {
                board[row][col] = buffer[col * 2] - '0'; // Convert character to integer
            }
        }
        print(board);

        // Check if the solution is valid
        if (isValidSolution(board)) {
            printf("The Sudoku solution is valid.\n");
        } else {
            printf("The Sudoku solution is invalid.\n");
        }

        // Skip empty line between solutions
        fgets(buffer, sizeof(buffer), file);
    }

    fclose(file);
}

int main() {
    const char *filename = "sudoku_solutions.txt";
    read_solutions(filename);

    return 0;
}
