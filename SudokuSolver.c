#include "sudoku.h"
/*
netID's: mkhan331, salkha21, jalenc3
This program solves the Sudoku puzzle using recursive backtracking.
We created 4 other functions to help create the solve_sudoku function.
The first function is_val_in_row tests to see if the val being checked 
already existed in the ith row of array sudoku.
THe second function is_val_in_col tests to if the the val being checked
already existed in jth column of array sudoku.
The third function is_val_in_3x3_zone tests if the val being checked
has already been filled in the 3x3 zone corresponding to the inputted cell. 
The fourth function is_val_valid tests for the other 3 functions
to checks the legality of filling a cell (i, j) with a given number val. 
*/

int is_val_in_row(const int val, const int i, const int sudoku[9][9]) {

  assert(i>=0 && i<9);
  int row;
    //Loops through each row and checks for val
  for (row = 0; row < 9; row++)
    {
      if (sudoku[i][row] == val)
        return 1; 
    } 

  return 0;
  // END TODO
}

// Function: is_val_in_col
// Return true if "val" already existed in jth column of array sudoku.
int is_val_in_col(const int val, const int j, const int sudoku[9][9]) {

  assert(j>=0 && j<9);
  int col;
   // Loops through each row and checks for val
    for (col = 0; col < 9; col++)
    {
      if (sudoku[col][j] == val)
        return 1; 
    } 

  return 0;
  // END TODO
}

// Function: is_val_in_3x3_zone
// Return true if val already existed in the 3x3 zone corresponding to (i, j)
int is_val_in_3x3_zone(const int val, const int i, const int j, const int sudoku[9][9]) {
  int zrow = (i/3)*3; // Sets starting row of corresponding 3x3
  int zcol = (j/3)*3; //Sets starting column of corresponding 3x3
  int startrow, startcol; //Used as counters
  assert(i>=0 && i<9);
  for (startrow = zrow; startrow < zrow + 3; startrow++)
  {
    for (startcol = zcol; startcol < zcol + 3; startcol++)
      if(sudoku[startrow][startcol] == val)
      {
        return 1;
      }
  }

  return 0;

}

// Function: is_val_valid
// Return true if the val is can be filled in the given entry.
int is_val_valid(const int val, const int i, const int j, const int sudoku[9][9]) {

  assert(i>=0 && i<9 && j>=0 && j<9);
 
  // BEG TODO
  //Checks if val is in row
  if (is_val_in_row(val, i, sudoku) == 1) 
    return 0;
  //Checks if val is in column
  else if(is_val_in_col(val, j, sudoku) == 1)
    return 0;
  //Checks if val is in 3x3
  else if (is_val_in_3x3_zone(val, i, j, sudoku) == 1)
    return 0;
  //Returns 1 otherwise;
  else
    return 1;
  // END TODO
}

// Procedure: solve_sudoku
// Solve the given sudoku instance.
int solve_sudoku(int sudoku[9][9]) {

  int i, j,num;
  for (i = 0; i < 9; i++) //Loops thorugh each row
  {
    for(j = 0; j < 9; j++) //Loops through each column
    {
      if (sudoku[i][j] == 0) //Checks if element is 0
      {
        //Implements given algorithm
        for (num = 1; num <= 9; num++) //Checks possibile values
        {
          if (is_val_valid(num, i, j, sudoku) == 1)
          {
            sudoku[i][j] = num;
            if (solve_sudoku(sudoku))
            {
              return 1;
            }
            else
            {
              sudoku[i][j] = 0;
            }
          }
        }
        return 0;
      }
    }
  }
 
}

// Procedure: print_sudoku
void print_sudoku(int sudoku[9][9])
{
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      printf("%2d", sudoku[i][j]);
    }
    printf("\n");
  }
}

// Procedure: parse_sudoku
void parse_sudoku(const char fpath[], int sudoku[9][9]) {
  FILE *reader = fopen(fpath, "r");
  assert(reader != NULL);
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      fscanf(reader, "%d", &sudoku[i][j]);
    }
  }
  fclose(reader);
}
