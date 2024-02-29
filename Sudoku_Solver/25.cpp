#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <omp.h>

using std::cout;
using std::endl;

#define DIM 16
#define BLANK 0
#define SPACE " "
#define LINE "|"
#define NEW_ROW "-----------------------------------------------------------------------------------"
#define GRID_FULL std::make_pair(16, 16)

// Prints the Soduko grid
void print_grid(int grid[DIM][DIM])
{
    for (int i = 0; i < DIM; i++)
    {
        cout << SPACE << SPACE << SPACE << SPACE << endl;
        cout << NEW_ROW << endl;
        for (int j = 0; j < DIM; j++)
        {
            cout << SPACE;
            if (BLANK == grid[i][j])
            {
                cout << SPACE;
            }
            else
            {
                cout << grid[i][j];
            }
            cout << SPACE;
            cout << LINE;
        }
    }
    cout << endl << NEW_ROW << endl << endl;;
}

// Returns a boolean which indicates whether any assigned entry
// in the specified row matches the given number. 
bool used_in_row(int grid[DIM][DIM], int row, int num)
{
    for (int col = 0; col < DIM; col++)
        if (grid[row][col] == num)
        {
            return true;
        }
    return false;
}

// Returns a boolean which indicates whether any assigned entry
// in the specified column matches the given number. 
bool used_in_col(int grid[DIM][DIM], int col, int num)
{
    for (int row = 0; row < DIM; row++)
        if (grid[row][col] == num)
        {
            return true;
        }
    return false;
}

// Returns a boolean which indicates whether any assigned entry
// within the specified 3x3 box matches the given number. 
bool used_in_box(int grid[DIM][DIM], int box_start_rpw, int box_start_col, int num)
{
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            if (grid[row + box_start_rpw][col + box_start_col] == num) 
            {
                return true;
            }
    return false;
}

// Returns a boolean which indicates whether it will be legal to assign
// num to the given row,col location.
bool is_safe(int grid[DIM][DIM], int row, int col, int num)
{
    // Check if 'num' is not already placed in current row,
    // current column and current 3x3 box 
    return !used_in_row(grid, row, num) &&
        !used_in_col(grid, col, num) &&
        !used_in_box(grid, row - row % 4, col - col % 4, num);
}

// Searches the grid to find an entry that is still unassigned. If
// found, the reference parameters row, col will be set the location
// that is unassigned, and true is returned. If no unassigned entries
// remain, false is returned. 
std::pair<int, int> get_unassigned_location(int grid[DIM][DIM])
{
    for (int row = 0; row < DIM; row++)
        for (int col = 0; col < DIM; col++)
            if (grid[row][col] == BLANK)
            {
                return std::make_pair(row, col);
            }
    return GRID_FULL;
}

bool solve_soduko3(int grid[DIM][DIM])
{
    // If the Soduko grid has been filled, we are done
    if (GRID_FULL == get_unassigned_location(grid))
    {
   //omp cancel 
        return true; 
    }

    // Get an unassigned Soduko grid location
    std::pair<int, int> row_and_col = get_unassigned_location(grid);
    int row = row_and_col.first;
    int col = row_and_col.second;

    // Consider digits 1 to 16 
    for (int num = 1; num <= 16; num++)
    {
        if (is_safe(grid, row, col, num))
        {
            grid[row][col] = num;

            if (solve_soduko3(grid))
            { 
        //int iam = omp_get_thread_num();
        //printf("Solution from thread %d\n",iam);
        print_grid(grid);
                exit(true); //omp cancel
                return true;
            }

            // different number for this particular unassigned location
            grid[row][col] = BLANK;
        }
    }
    return false; 
}

bool solve_soduko2(int grid[DIM][DIM])
{
    // If the Soduko grid has been filled, we are done
    if (GRID_FULL == get_unassigned_location(grid))
    {
   //omp cancel 
        return true; 
    }

    // Get an unassigned Soduko grid location
    std::pair<int, int> row_and_col = get_unassigned_location(grid);
    int row = row_and_col.first;
    int col = row_and_col.second;

    // Consider digits 1 to 16
  #pragma omp taskloop firstprivate(grid) 
    for (int num = 1; num <= 16; num++)
    {
        if (is_safe(grid, row, col, num))
        {
            grid[row][col] = num;

            
            if (solve_soduko3(grid))
            { 
        }

            grid[row][col] = BLANK;
        }
    }

    return false; 
}

// Takes a partially filled-in grid and attempts to assign values to
// all unassigned locations in such a way to meet the requirements
// for Sudoku solution (non-duplication across rows, columns, and boxes) 
bool solve_soduko(int grid[DIM][DIM])
{
    // If the Soduko grid has been filled, we are done
    if (GRID_FULL == get_unassigned_location(grid))
    {
   //omp cancel 
        return true; 
    }

    // Get an unassigned Soduko grid location
    std::pair<int, int> row_and_col = get_unassigned_location(grid);
    int row = row_and_col.first;
    int col = row_and_col.second;

    // Consider digits 1 to 16
  #pragma omp taskloop firstprivate(grid) 
    for (int num = 1; num <= 16; num++)
    {
        if (is_safe(grid, row, col, num))
        {
            grid[row][col] = num;
            if (solve_soduko2(grid))
            { 
            }
            grid[row][col] = BLANK;
        }
    }

    return false; 
}




int main(int argc, char const * argv[])
{
  int n;
  if (argc > 1) {
    n = atoi(argv[1]);
  } else {
    n = 10;
  }
    cout << "********************************\n\n\tSudoku Solver\n\n********************************" << endl << endl;

  int grid[DIM][DIM] = 
               { 
               {0, 0, 0, 1, 6, 0, 0, 0, 0, 7, 0, 3, 0, 0, 0, 0 },
               {0, 6, 0, 0, 0, 5, 0, 4, 14, 0, 1, 0, 2, 11, 0, 13 }, 
               {0, 0, 0, 7, 0, 3, 0, 13, 8, 0, 16, 4, 0, 0, 1, 0 },
               {0, 0, 0, 0, 1, 0, 0, 0, 6, 10, 0, 11, 3, 0, 8, 14 },
               {0, 2, 0, 0, 0, 1, 11, 0, 3, 0, 10, 0, 0, 8, 0, 12 },
               {0, 0, 12, 0, 0, 0, 6, 9, 0, 14, 0, 0, 10, 0, 5, 0 },
               {0, 10, 0, 13, 0, 0, 3, 15, 0, 9, 0, 2, 0, 6, 0, 16 },
               {1, 4, 7, 0, 13, 0, 0, 5, 0, 6, 0, 0, 9, 0, 0, 11 },
               {0, 7, 0, 5, 9, 6, 1, 0, 2, 8, 3, 10, 0, 14, 0, 4 },
               {0, 0, 8, 0, 0, 0, 0, 3, 5, 0, 0, 15, 0, 13, 0, 10 },
               {6, 3, 0, 4, 0, 15, 0, 8, 7, 0, 0, 1, 0, 12, 0, 2 },
               {0, 1, 0, 0, 4, 11, 0, 2, 0, 16, 0, 0, 8, 3, 6, 7 },
               {0, 0, 0, 3, 0, 0, 2, 10, 0, 13, 0, 6, 0, 5, 0, 0 },
               {5, 0, 0, 2, 0, 8, 0, 6, 10, 1, 0, 7, 0, 0, 12, 9 },
               {7, 9, 1, 6, 0, 14, 0, 11, 0, 3, 0, 5, 0, 0, 10, 8 },
               {0, 0, 0, 0, 0, 0, 9, 1, 4, 0, 0, 8, 0, 7, 2, 3 }
               };

    print_grid(grid);
  
  #pragma omp parallel
  if (true == solve_soduko(grid))
  {
    print_grid(grid);
  }
  else
  {
    
  }
  
    return 0;
}