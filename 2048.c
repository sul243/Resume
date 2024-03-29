#include "game.h"

/*
The objective is to slide numbered tiles on a grid 
to combine them and create a tile with the number 2048. 
In addition to the functions provided, we made functions 
to slide in all directions and combine in all directions.
The slide functions move the values in the board via a temp
array.
The combine functions check and modify cells that can be
added together.
The move functions combines both of these functions.
See indiviual functions more details.
*/


game * make_game(int rows, int cols)
/*! Create an instance of a game structure with the given number of rows
    and columns, initializing elements to -1 and return a pointer
    to it. (See game.h for the specification for the game data structure) 
    The needed memory should be dynamically allocated with the malloc family
    of functions.
*/
{
    //Dynamically allocate memory for game and cells (DO NOT modify this)
    game * mygame = malloc(sizeof(game));
    mygame->cells = malloc(rows*cols*sizeof(cell));


    //YOUR CODE STARTS HERE:  Initialize all other variables in game struct
    mygame->rows = rows; //Initializes rows in structure
    mygame->cols = cols; //Initializes columns in structures
    for (int i = 0; i < rows * cols; i++)
        mygame->cells[i] = -1; //Initilaizes 1D array to -1
    mygame->score = 0; //Initilaizes score to 0

    return mygame; //Returns pointer to struct
}

void remake_game(game ** _cur_game_ptr,int new_rows,int new_cols)
/*! Given a game structure that is passed by reference, change the
	game structure to have the given number of rows and columns. Initialize
	the score and all elements in the cells to -1. Make sure that any 
	memory previously allocated is not lost in this function.	
*/
{
	/*Frees dynamically allocated memory used by cells in previous game,
	 then dynamically allocates memory for cells in new game.  DO NOT MODIFY.*/
	free((*_cur_game_ptr)->cells);
	(*_cur_game_ptr)->cells = malloc(new_rows*new_cols*sizeof(cell));

	 //YOUR CODE STARTS HERE:  Re-initialize all other variables in game struct
    (*_cur_game_ptr)->rows = new_rows; //Initializes rows in structure with new val
    (*_cur_game_ptr)->cols = new_cols; //Initializes cols in structure with new val
    for (int i = 0; i < new_rows * new_cols; i++)
        (*_cur_game_ptr)->cells[i] = -1; //Initiliazes 1D array to -1
    (*_cur_game_ptr)->score = 0; //Initializes score to 0

	return;	
}

void destroy_game(game * cur_game)
/*! Deallocate any memory acquired with malloc associated with the given game instance.
    This includes any substructures the game data structure contains. Do not modify this function.*/
{
    free(cur_game->cells);
    free(cur_game);
    cur_game = NULL;
    return;
}

cell * get_cell(game * cur_game, int row, int col)
/*! Given a game, a row, and a column, return a pointer to the corresponding
    cell on the game. (See game.h for game data structure specification)
    This function should be handy for accessing game cells. Return NULL
	if the row and col coordinates do not exist.
*/
{
    //YOUR CODE STARTS HERE
    /*Checks if cell is within bounds of 2D array*/
    if (row >= 0 && row < cur_game->rows && col >= 0 && col < cur_game->cols)
    {
        int index = row * cur_game->cols + col; //Converts 2D index to 1D index
        /*Checks if cell is within bounds of 1D array*/
        if (index >= 0 && index < cur_game->rows * cur_game->cols)
            return &cur_game->cells[index]; //Returns address of desired cell
    }
    return NULL; //Returns null if error
}

int slide_up(game * cur_game)
{
    int change = 0; //variable keeps track if there is a change
    cell temp_col[cur_game->rows]; //Creates temp array for sliding
    for (int j = 0; j < cur_game->cols; j++) //Loops through each column
    {
        for (int i = 0; i < cur_game->rows; i++) //Initializes temp array to -1
            temp_col[i] = -1;
        int index = 0;

        for (int i = 0; i < cur_game->rows; i++) //Goes down each column
        {
            cell * pcell = get_cell(cur_game, i, j); //Gets address of current cell
            if (pcell != NULL && *pcell != -1) //Tests if cell is not empty
            {
                temp_col[index] = *pcell; //Places value of cell in temp array
                index++; //Increments index of temp array
            }
        }

        for (int i = 0; i < cur_game->rows; i++) //Loops through temp array
        {
            cell * pcell = get_cell(cur_game, i, j); //Gets address of current cell
            if (*pcell != temp_col[i]) //Compares temp array and cell
            {
                if (change == 0)
                    change = 1; //If they are different, change = 1 (Change has been made)
                *pcell = temp_col[i]; //Changes value of cell from temp array
            }
        }
    }
    return change; //Returns change
}

int slide_down(game * cur_game) //Similiar to slide_up
{
    int change = 0;
    cell temp_col[cur_game->rows];
    for (int j = 0; j < cur_game->cols; j++)
    {
        for (int i = 0; i < cur_game->rows; i++) //Goes "up" the column
            temp_col[i] = -1;
        int index = cur_game->rows - 1; //Sets temp index to "bottom"

        for (int i = cur_game->rows - 1; i >= 0; i--)
        {
            cell * pcell = get_cell(cur_game, i, j);
            if (pcell != NULL && *pcell != -1)
            {
                temp_col[index] = *pcell;
                index--;
            }
        }

        for (int i = 0; i < cur_game->rows; i++)
        {
            cell * pcell = get_cell(cur_game, i, j);
            if (*pcell != temp_col[i])
            {
                if (change == 0)
                    change = 1;
                *pcell = temp_col[i];
            }
        }
    }
    return change;
}

int slide_left(game * cur_game) //Similiar to slide_up
{
    int change = 0;    
    cell temp_row[cur_game->cols]; //Tweaks temp_col to temp_row to adjust for rows
    for (int i = 0; i < cur_game->rows; i++) //Loops through each row
    {
        for (int j = 0; j < cur_game->cols; j++) //Initializes temp array to -1
            temp_row[j] = -1;
        int index = 0;

        for (int j = 0; j < cur_game->cols; j++) //Goes to the "right" of the row
        {
            cell * pcell = get_cell(cur_game, i, j);
            if (pcell != NULL && *pcell != -1)
            {
                temp_row[index] = *pcell;
                index++;
            }
        }

        for (int j = 0; j < cur_game->cols; j++) //Loops through temp array to check change
        {
            cell * pcell = get_cell(cur_game, i, j);
            if (*pcell != temp_row[j])
            {
                if (change == 0)
                    change = 1;
                *pcell = temp_row[j];
            }
        }
    }
    return change; //Return change
}

int slide_right(game * cur_game) //Similiar to slide_ledt
{
    int change = 0;
    cell temp_row[cur_game->cols];
    for (int i = 0; i < cur_game->rows; i++)
    {
        for (int j = 0; j < cur_game->cols; j++)
            temp_row[j] = -1;
        int index = cur_game->cols - 1; //Starts from "right hand" side of temp array 

        for (int j = cur_game->cols - 1; j >= 0; j--)  //Starts from right side
        {
            cell * pcell = get_cell(cur_game, i, j);
            if (pcell != NULL && *pcell != -1)
            {
                temp_row[index] = *pcell;
                index--;
            }
        }

        for (int j = 0; j < cur_game->cols; j++)
        {
            cell * pcell = get_cell(cur_game, i, j);
            if (*pcell != temp_row[j])
            {
                if (change == 0)
                    change = 1;
                *pcell = temp_row[j];
            }
        }
    }
    return change; //Return change
}

int combine_up(game * cur_game, int check_only) //Combines like cells
{
    int change = 0; //Checks for change
    for (int j = 0; j < cur_game->cols; j++) //Starts like slide_up
    {
        for (int i = 0; i < cur_game->rows - 1; i++) //Goes down the column except bottom
        {
            cell * pcur_cell = get_cell(cur_game, i, j); //Gets address of current cell
            cell * padj_cell = get_cell(cur_game, i + 1, j); //Gets address of cell below
            if (pcur_cell != NULL && *pcur_cell != -1) //If current cell is not empty/NULL
            {
                if (padj_cell != NULL && *padj_cell != -1) //If not empty/NULL
                {
                    if (*pcur_cell == *padj_cell) //Checks if dereference val of ptrs =
                    {
                        if (check_only == 0)
                        {
                            *pcur_cell *= 2; //Increases val of current cell
                            *padj_cell = -1; //Makes adj cell empty
                            cur_game->score += *pcur_cell; //Increases 
                        }
                        if (change == 0)
                            change = 1; //Sets change if change has been made
                    }
                }
            }
        }
    }
    return change; //Returns change
}

int combine_down(game * cur_game, int check_only) //Similiar to combine_up
{
    int change = 0;    
    for (int j = 0; j < cur_game->cols; j++)
    {
        for (int i = cur_game->rows -1; i > 0; i--) //Goes up column except top
        {
            cell * pcur_cell = get_cell(cur_game, i, j);
            cell * padj_cell = get_cell(cur_game, i - 1, j); //Checks cell above
            if (pcur_cell != NULL && *pcur_cell != -1)
            {
                if (padj_cell != NULL && *padj_cell != -1)
                {
                    if (*pcur_cell == *padj_cell)
                    {
                        if (check_only == 0)
                        {
                            *pcur_cell *= 2;
                            *padj_cell = -1;
                            cur_game->score += *pcur_cell;
                        }
                        if (change == 0)
                            change = 1;
                    }
                }
            }
        }
    }
    return change; //Returns change
}

int combine_left(game * cur_game, int check_only) //Similiar to combine_up
{
    int change = 0;    
    for (int i = 0; i < cur_game->rows; i++) //Loops thorugh each row
    {
        for (int j = 0; j < cur_game->cols - 1; j++) //Loops through each column except rightmost
        {
            cell * pcur_cell = get_cell(cur_game, i, j);
            cell * padj_cell = get_cell(cur_game, i, j + 1); //Checks for cell to the right
            if (pcur_cell != NULL && *pcur_cell != -1)
            {
                if (padj_cell != NULL && *padj_cell != -1)
                {
                    if (*pcur_cell == *padj_cell)
                    {
                        if (check_only == 0)
                        {
                            *pcur_cell *= 2;
                            *padj_cell = -1;
                            cur_game->score += *pcur_cell;
                        }
                        if (change == 0)
                            change = 1;
                    }
                }
            }
        }
    }
    return change; //Return change
}

int combine_right(game * cur_game, int check_only) //Similair to combine_left
{
    int change = 0;    
    for (int i = 0; i < cur_game->rows; i++)
    {
        for (int j = cur_game->cols - 1; j > 0; j--) //Starts from the right
        {
            cell * pcur_cell = get_cell(cur_game, i, j);
            cell * padj_cell = get_cell(cur_game, i, j - 1); //Checks cell to the left
            if (pcur_cell != NULL && *pcur_cell != -1)
            {
                if (padj_cell != NULL && *padj_cell != -1)
                {
                    if (*pcur_cell == *padj_cell)
                    {
                        if (check_only == 0)
                        {
                            *pcur_cell *= 2;
                            *padj_cell = -1;
                            cur_game->score += *pcur_cell;
                        }
                        if (change == 0)
                            change = 1;
                    }
                }
            }
        }
    }
    return change; //Returns change
}

int move_w(game * cur_game)
/*!Slides all of the tiles in cur_game upwards. If a tile matches with the 
   one above it, the tiles are merged by adding their values together. When
   tiles merge, increase the score by the value of the new tile. A tile can 
   not merge twice in one turn. If sliding the tiles up does not cause any 
   cell to change value, w is an invalid move and return 0. Otherwise, return 1. 
*/
{
    //YOUR CODE STARTS HERE
    int change1 = slide_up(cur_game); //Calls slide up
    int change2 = combine_up(cur_game, 0); //Calls combine up with check_only set to 0
    int change3 = slide_up(cur_game); //Calls slide up again
    //printf("change1 = %d\n", change1);
    //printf("change2 = %d\n", change2);
    //printf("change3 = %d\n", change3);
    return change1 == 0 && change2 == 0 && change3 == 0 ? 0 : 1;
    //If no change in any of the 3 function calls -> return 0
    //If there is one -> return 1
};

int move_s(game * cur_game) //Same as move_w
{
    //YOUR CODE STARTS HERE
    int change1 = slide_down(cur_game);
    int change2 = combine_down(cur_game, 0);
    int change3 = slide_down(cur_game);
    //printf("change1 = %d\n", change1);
    //printf("change2 = %d\n", change2);
    //printf("change3 = %d\n", change3);
    return change1 == 0 && change2 == 0 && change3 == 0 ? 0 : 1;
};

int move_a(game * cur_game) //Same as move_w
{
    //YOUR CODE STARTS HERE
    int change1 = slide_left(cur_game);
    int change2 = combine_left(cur_game, 0);
    int change3 = slide_left(cur_game);
    //printf("change1 = %d\n", change1);
    //printf("change2 = %d\n", change2);
    //printf("change3 = %d\n", change3);
    return change1 == 0 && change2 == 0 && change3 == 0 ? 0 : 1;
};

int move_d(game * cur_game){ //Same as move_w
    //YOUR CODE STARTS HERE
    int change1 = slide_right(cur_game);
    int change2 = combine_right(cur_game, 0);
    int change3 = slide_right(cur_game);
    //printf("change1 = %d\n", change1);
    //printf("change2 = %d\n", change2);
    //printf("change3 = %d\n", change3);
    return change1 == 0 && change2 == 0 && change3 == 0 ? 0 : 1;
};

int legal_move_check(game * cur_game)
/*! Given the current game check if there are any legal moves on the board. There are
    no legal moves if sliding in any direction will not cause the game to change.
	Return 1 if there are possible legal moves, 0 if there are none.
 */
{
    //YOUR CODE STARTS HERE
    int n = cur_game->rows * cur_game->cols; //Checks if 1D array is empty
    for (int i = 0; i < n; i++)
    {
        if (cur_game->cells[i] == -1)
            return 1; //Returns 1 if empty
    }

    int change1 = combine_up(cur_game, 1); //Calls combine functions with check_only set to 1
    int change2 = combine_down(cur_game, 1); //This does the same as combine w/o the combining
    int change3 = combine_left(cur_game, 1); //^^
    int change4 = combine_right(cur_game, 1); //^^
    if (change1 || change2 || change3 || change4) //If any ot these are 1
        return 1; //Legal moves possibile

    return 0; //No possible legal moves
}


/*! code below is provided and should not be changed */

void rand_new_tile(game * cur_game)
/*! insert a new tile into a random empty cell. First call rand()%(rows*cols) to get a random value between 0 and (rows*cols)-1.
*/
{
	
	cell * cell_ptr;
    cell_ptr = 	cur_game->cells;
	
    if (cell_ptr == NULL){ 	
        printf("Bad Cell Pointer.\n");
        exit(0);
    }
	
	
	//check for an empty cell
	int emptycheck = 0;
	int i;
	
	for(i = 0; i < ((cur_game->rows)*(cur_game->cols)); i++){
		if ((*cell_ptr) == -1){
				emptycheck = 1;
				break;
		}		
        cell_ptr += 1;
	}
	if (emptycheck == 0){
		printf("Error: Trying to insert into no a board with no empty cell. The function rand_new_tile() should only be called after tiles have succesfully moved, meaning there should be at least 1 open spot.\n");
		exit(0);
	}
	
    int ind,row,col;
	int num;
    do{
		ind = rand()%((cur_game->rows)*(cur_game->cols));
		col = ind%(cur_game->cols);
		row = ind/cur_game->cols;
    } while ( *get_cell(cur_game, row, col) != -1);
        //*get_cell(cur_game, row, col) = 2;
	num = rand()%20;
	if(num <= 1){
		*get_cell(cur_game, row, col) = 4; // 1/10th chance
	}
	else{
		*get_cell(cur_game, row, col) = 2;// 9/10th chance
	}
}

int print_game(game * cur_game) 
{
    cell * cell_ptr;
    cell_ptr = 	cur_game->cells;

    int rows = cur_game->rows;
    int cols = cur_game->cols;
    int i,j;
	
	printf("\n\n\nscore:%d\n",cur_game->score); 
	
	
	printf("\u2554"); // topleft box char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // top box char
	printf("\u2557\n"); //top right char 
	
	
    for(i = 0; i < rows; i++){
		printf("\u2551"); // side box char
        for(j = 0; j < cols; j++){
            if ((*cell_ptr) == -1 ) { //print asterisks
                printf(" **  "); 
            }
            else {
                switch( *cell_ptr ){ //print colored text
                    case 2:
                        printf("\x1b[1;31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4:
                        printf("\x1b[1;32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8:
                        printf("\x1b[1;33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 16:
                        printf("\x1b[1;34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 32:
                        printf("\x1b[1;35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 64:
                        printf("\x1b[1;36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 128:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 256:
                        printf("\x1b[32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 512:
                        printf("\x1b[33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 1024:
                        printf("\x1b[34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 2048:
                        printf("\x1b[35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4096:
                        printf("\x1b[36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8192:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
					default:
						printf("  X  ");

                }

            }
            cell_ptr++;
        }
	printf("\u2551\n"); //print right wall and newline
    }
	
	printf("\u255A"); // print bottom left char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // bottom char
	printf("\u255D\n"); //bottom right char
	
    return 0;
}

int process_turn(const char input_char, game* cur_game) //returns 1 if legal move is possible after input is processed
{ 
	int rows,cols;
	char buf[200];
	char garbage[2];
    int move_success = 0;
	
    switch ( input_char ) {
    case 'w':
        move_success = move_w(cur_game);
        break;
    case 'a':
        move_success = move_a(cur_game);
        break;
    case 's':
        move_success = move_s(cur_game);
        break;
    case 'd':
        move_success = move_d(cur_game);
        break;
    case 'q':
        destroy_game(cur_game);
        printf("\nQuitting..\n");
        return 0;
        break;
	case 'n':
		//get row and col input for new game
		dim_prompt: printf("NEW GAME: Enter dimensions (rows columns):");
		while (NULL == fgets(buf,200,stdin)) {
			printf("\nProgram Terminated.\n");
			return 0;
		}
		
		if (2 != sscanf(buf,"%d%d%1s",&rows,&cols,garbage) ||
		rows < 0 || cols < 0){
			printf("Invalid dimensions.\n");
			goto dim_prompt;
		} 
		
		remake_game(&cur_game,rows,cols);
		
		move_success = 1;
		
    default: //any other input
        printf("Invalid Input. Valid inputs are: w, a, s, d, q, n.\n");
    }

	
	
	
    if(move_success == 1){ //if movement happened, insert new tile and print the game.
         rand_new_tile(cur_game); 
		 print_game(cur_game);
    } 

    if( legal_move_check(cur_game) == 0){  //check if the newly spawned tile results in game over.
        printf("Game Over!\n");
        return 0;
    }
    return 1;
}
