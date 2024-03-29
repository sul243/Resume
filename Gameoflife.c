#include "updateBoard.h"
/* 
This progam completes the three functions described in the MP6 page:
countLiveNeighbor, updateBoard, aliveStable.
NOTE; WE CHANGED UPDATEBOARD.H to help us call our function in alive stable.
countLiveNeighbor implements the algorithm provided on the MP6 page.  
It calculates the number of alive cells at the given row and column.
A live cell is represented as 1, a dead cell is represented as 0.
During this caluclation, it makes sure that it stays within bounds for cases
like the elements on the edge of the 2D array. It then returns the number of alive
neighbors.
updateBoard creates another temporary 1D array based on the board row size and board column size.
It iterates through each row and column in the 2D array and then calls the countLiveNeighbor
for each element in the 2D array. Based on the rules of the game, changes are made to the state
of each element and are stored in the temporary array. After iterating through each
element, the values in the temporary array are put into and overwrite the original array.
aliveStable uses much of the same algorithm as updateBoard. However, it checks if every element is dead,
in which case, it returns 1. In addition, if there are no changes, it returns 1. Otherwise
it returns 0.
We added the getIndex function to make indexing the 1D array easier
*/


int countLiveNeighbor(int* board, int boardRowSize, int boardColSize, int row, int col){
    
    int live = 0; 
    int currRow = row;
    int currCol = col;
    int rows;
    int cols;
    // here we loop through the arrray and checks the amount of live neighbors for a specific row and col.
    for (rows = currRow - 1; rows <= currRow + 1; rows++)
    {
        for (cols = currCol - 1; cols <= currCol + 1; cols++)
        {
            if (rows >= 0 && rows < boardRowSize )  // next two rows and cols checks if the specific row and col is within range. 
            {
                if (cols >= 0 && cols < boardColSize)
                {
                    if(!(rows == currRow && cols == currCol))  // we do this to not count current row and coloumn as a live neighbor.
                    {
                        if (board[rows * boardColSize + cols]) 
                        {
                            live++;
                        }
                    }

                }
            }
        }
    }
    return live;
}

void updateBoard(int* board, int boardRowSize, int boardColSize) {
    int live; 
    int rows;
    int cols;
    // int newBoard creates an identical array to the board.
    int newboard[boardRowSize * boardColSize];
    for (rows = 0; rows < boardRowSize; rows++)
    {
        for (cols = 0; cols < boardColSize; cols++)
        {
            live = countLiveNeighbor(board, boardRowSize, boardColSize, rows, cols); // from here the newboard is what keeps track of live neighbors.
            if (board[rows * boardColSize + cols] == 1)  //Checks if the board is a 1 or a 0
            {
                if (live < 2 || live > 3)    // if live neighbors are greater than 3  or less than 2 it is dead changes to 0
                {
                    newboard[rows * boardColSize + cols] = 0; 
                }
                else 
                    newboard[rows * boardColSize + cols] = 1; //otherwise it is still alive
            }
            else 
            {
                if (live == 3)    //if dead cell has 3 live neighbors change it to a live cell.
                {
                    newboard[rows * boardColSize + cols] = 1; 
                }
                else 
                    newboard[rows * boardColSize + cols] = 0;  //else keep it dead.
            }

    
            
            

        }
// Here new board is copied into board
    }
    for (rows = 0; rows < boardRowSize; rows++)
    {
        for (cols = 0; cols < boardColSize; cols++)
        {
            board[rows * boardColSize + cols] = newboard[rows  * boardColSize + cols];
        }
    
    }
    
}

/*
 * aliveStable
 * Checks if the alive cells stay the same for next step
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: return 1 if the alive cells for next step is exactly the same with 
 * current step or there is no alive cells at all.
 * return 0 if the alive cells change for the next step.
 */ 

int aliveStable(int* board, int boardRowSize, int boardColSize)
{
    int rows;
    int columns;
    int live;
    int allDead = 1; //Checks for all dead cells
    int stable = 1; //Checks for changes


    //Checks if every cell is dead
    for (rows = 0; rows < boardRowSize && allDead == 1; rows++)
        for (columns = 0; columns < boardColSize && allDead == 1; columns++)
            if (board[getIndex(rows, boardColSize, columns)] == 1)
                allDead = 0;
    

    if (allDead == 1)
        return allDead;

    //Checks for any change of cells
    for (rows = 0; rows < boardRowSize && stable == 1; rows++)
    {
        for (columns = 0; columns < boardColSize && stable == 1; columns++)
        {
            live = countLiveNeighbor(board, boardRowSize, boardColSize, rows, columns);
            // If cell is alive
            if (board[getIndex(rows, boardColSize, columns)] == 1)
            {
                if (live < 2 || live > 3)
                {
                    stable = 0;
                } 
            }
            // If cell is dead
            else
            {
                if (live == 3)
                {
                    stable = 0;
                }
            }
        }
    }
    //Return stable for no change or change
    return stable;


}
//Peforms calculation from 2D to 1D index
int getIndex(int row, int width, int col)
{
    int index;
    index = row * width + col;
    return index;
}
