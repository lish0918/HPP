if (Solve(board, unAssignInd, N_unAssign)) {
        // Print solved board
        printf("-------------------------\n");
        for (int x = 0; x < BoardSize; x++) {
            printf("| ");
            for (int y = 0; y < BoardSize; y++) {
                printf("%d ", board[x][y]);
                if ((y + 1) % 3 == 0) {
                    printf("| ");
                }
            }
            printf("\n");
            if ((x + 1) % 3 == 0) {
                printf("-------------------------\n");
            }
        }
    } else {
        printf("No solution found.\n");
    }