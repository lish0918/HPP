#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BoardSize 9
#define BoxSize 3

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

int DuplicateNumbersinBox(int board[BoardSize][BoardSize], int startX, int startY, int num) {
	int startRow = startX - startX % BoxSize;
	int startCol = startY - startY % BoxSize;
	for (int x = 0; x < BoxSize; x++) {
		for (int y = 0; y < BoxSize; y++) {
			if (board[startRow + x][startCol + y] == num) {
				return 1;
			}
		}
	}
	return 0;
}

int ValidateBoard(int board[BoardSize][BoardSize], int x, int y, int num) {
	if (DuplicateNumbersinRow(board, x, num)) {
		return 0;
	}
	if (DuplicateNumbersinCol(board, y, num)) {
		return 0;
	}
	if (DuplicateNumbersinBox(board, x, y, num)) {
		return 0;
	}
	return 1;
}

void PrintBoard(int board[BoardSize][BoardSize]) {
	for (int x = 0; x < BoardSize; x++) {
		if (x % BoxSize == 0) {
			printf("-------------------------\n");
		}
		for (int y = 0; y < BoardSize; y++) {
			if (y % BoxSize == 0) {
				printf("| ");
			}
			printf("%d ", board[x][y]);
		}
		printf("|\n");
	}
	printf("-------------------------\n");
}

void GenerateBoard(int board[BoardSize][BoardSize]) {
	srand(time(NULL));
	int filledCells = rand() % 10 + 20; // Randomly fill 20-30 cells
	for (int i = 0; i < filledCells; i++) {
		int x = rand() % BoardSize;
		int y = rand() % BoardSize;
		int num = rand() % BoardSize + 1;
		if (ValidateBoard(board, x, y, num)) {
			board[x][y] = num;
		}
	}
}

int main() {
	printf("Welcome to Sudoku!\n");
	printf("Generating the Sudoku board...\n");

	int board[BoardSize][BoardSize] = {0};
	GenerateBoard(board);
	PrintBoard(board);

	return 0;
}
