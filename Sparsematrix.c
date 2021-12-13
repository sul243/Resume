#include "sparsemat.h"

#include <stdio.h>
#include <stdlib.h>

/*
    The goal of this MP is to create data structure called a list of tuples that
    stores sparse matrices more efficiently than a 2-D array. We 
    made multiple additional helper functions. 
   
    From top to bottom of this file, the first function is called get_index.
    This function returns the row major order of an item given its row and col.
    This is used to sort the linked list and make sure it is order.
    The insert function inserts a node into a linked list. It takes
    in as input, the width of the matrix, the row, and col, and a value
    It first creates a node and then finds where this new
    node should go based on its row major index, and then goes thorough the 2 cases
    of adding a node. It accesses the value member and sets the appropriate value.
    The delete function in the same parameters as insert without a value. This finds the node
    and "deletes" the nmode from the list.
    The find function find a node at a certain row and col and returns a pointer to that
    node.
    Load tuples reads necessary data from the input file (M rows x N cols)
    For each row it scans it calls the set tuples function to set the values for each node.
    The gv_tuples function calls the find function and returns a value.
    The set tuple function either inserts, replaces, or deletes a node.
    The save_tuples function writes to an output file the sparse matrix in the required format.
    The add tuples function adds 2 matrices.
    
The Pieces
*/

int get_index(int n, int row, int col)
{
    return row * n + col;
}

void insert(sp_tuples_node **head_ptr, int n, int row, int col, double value)
{
    // Pointer to new node
    sp_tuples_node *new_ptr;
    // Pointer to previous node in the list
    sp_tuples_node *prev_ptr;
    // Pointer to current node in the list
    sp_tuples_node *curr_ptr;
    int new_index = get_index(n, row, col);

    // Allocate memory for new node
    new_ptr = malloc(sizeof(sp_tuples_node));
    if (!new_ptr)
    {
        printf("Error allocating sp_tuples_node.\n");
        exit(1);
    }

    // Set members
    new_ptr->value = value;
    new_ptr->row = row;
    new_ptr->col = col;
    // Initialize next to NULL (will get overwritten as needed below)
    new_ptr->next = NULL;

    // Traverse the linked list starting from the head
    prev_ptr = NULL;
    curr_ptr = *head_ptr;

    // Find the correct location in the list for the new node
    while (curr_ptr != NULL && 
         new_index > get_index(n, curr_ptr->row, curr_ptr->col))
    {
        // Advance to the next node in the list
        prev_ptr = curr_ptr;
        curr_ptr = curr_ptr->next;
    }

    // Insert new node at beginning of list
    if (prev_ptr == NULL)
    {
        new_ptr->next = *head_ptr;
        *head_ptr = new_ptr;
    }
    // Insert new node between prev_ptr and curr_ptr
    else
    {
        // Link the previous node to the new node...
        prev_ptr->next = new_ptr;
        // Link the new node to the current node
        new_ptr->next = curr_ptr;
    }
}

void delete(sp_tuples_node **head_ptr, int n, int row, int col)
{
    // Pointer to previous node in the list
    sp_tuples_node *prev_ptr;
    // Pointer to current node in the list
    sp_tuples_node *curr_ptr;
    // Temporary node pointer
    sp_tuples_node *temp_ptr;
    int index = get_index(n, row, col);

    // If node to be deleted is first node in list
    if (index == get_index(n, (*head_ptr)->row, (*head_ptr)->col))
    {
        // Hold onto node being removed
        temp_ptr = *head_ptr;
        // Unlink node
        *head_ptr = (*head_ptr)->next;
        // Free unlinked node
        free(temp_ptr);
    }
    // Node to be deleted is not first node in list
    else
    {
        // Traverse the linked list starting from the head
        prev_ptr = *head_ptr;
        curr_ptr = (*head_ptr)->next;

        // Find the correct location in the list for the new node
        while (curr_ptr != NULL && 
            index != get_index(n, curr_ptr->row, curr_ptr->col))
        {
            // Advance to the next node in the list
            prev_ptr = curr_ptr;
            curr_ptr = curr_ptr->next;
        }

        // Delete node at curr_ptr
        if (curr_ptr != NULL)
        {
            // Hold onto node being removed
            temp_ptr = curr_ptr;
            // Unlink node
            prev_ptr->next = curr_ptr->next;
            // Free unlinked node
            free(temp_ptr);
        }
    }
}

sp_tuples_node * find(sp_tuples_node *head_ptr, int row, int col)
{
    // Traverse the linked list starting from the head
    sp_tuples_node *curr_ptr = head_ptr;
    while (curr_ptr != NULL)
    {
        if (curr_ptr->row == row && curr_ptr->col == col)
            return curr_ptr;
        // Advance to the next node in the list
        curr_ptr = curr_ptr->next;
    }
    
    return NULL;
}

sp_tuples * load_tuples(char* input_file)
{
    // Open input file
    FILE *fp = fopen(input_file, "r");
    if (!fp)
    {
        printf("Error opening input file.\n");
        return NULL;
    }

    // Allocate memory for sp_tuples
    sp_tuples *tuples_ptr = malloc(sizeof(sp_tuples));
    if (!tuples_ptr)
    {
        printf("Error allocating memory for sp_tuples.\n");
        return NULL;
    }
    
    // Initialize members
    tuples_ptr->m = 0;
    tuples_ptr->n = 0;
    tuples_ptr->nz = 0;
    tuples_ptr->tuples_head = NULL;

    // Read dimensions of matrix
    fscanf(fp, "%d%d", &tuples_ptr->m, &tuples_ptr->n);
    //printf("m = %d n = %d\n", sp_t_ptr->m, sp_t_ptr->n);

    // Read nonzero elements
    int row;
    int col;
    double value;
    fscanf(fp, "%d%d%lf", &row, &col, &value);
    while (!feof(fp))
    {
        // If row of data was read successfully, allocate memory for a node
        //printf("row = %d col = %d value = %f\n", row, col, value);
        set_tuples(tuples_ptr, row, col, value);

        // Read next row of data
        fscanf(fp, "%d%d%lf", &row, &col, &value);
    }
    // Close file
    fclose(fp);

    // Return tuples_ptr
    return tuples_ptr;
}


double gv_tuples(sp_tuples * mat_t, int row, int col)

{
    sp_tuples_node *node_ptr = find(mat_t->tuples_head, row, col);
    if (node_ptr != NULL)
        return node_ptr->value;
    return 0;
}

void set_tuples(sp_tuples * mat_t, int row, int col, double value)
{
    sp_tuples_node *node_ptr = find(mat_t->tuples_head, row, col);
    // If node is in list
    if (node_ptr != NULL)
    {
        if (value != 0)
            node_ptr->value = value;
        else
        {
            delete(&mat_t->tuples_head, mat_t->n, row, col);
            mat_t->nz--;
        }
    }
    else
    {
        if (value != 0)
        {
            insert(&mat_t->tuples_head, mat_t->n, row, col, value);
            mat_t->nz++;
        }
    }
    return;
}



void save_tuples(char * file_name, sp_tuples * mat_t)
{
    // Open output file
    FILE *fp = fopen(file_name, "w");
    if (!fp)
    {
        printf("Error opening input file.\n");
        exit(1);
    }
    
    // Write matrix dimensions
    fprintf(fp, "%d %d\n", mat_t->m, mat_t->n);

    // Traverse the linked list starting from the head
    sp_tuples_node *curr_ptr = mat_t->tuples_head;
    while (curr_ptr != NULL)
    {
        // Write nonzero elements
        fprintf(fp, "%d %d %f\n", curr_ptr->row, curr_ptr->col, curr_ptr->value);
        // Advance to the next node in the list
        curr_ptr = curr_ptr->next;
    }

    // Close file
    fclose(fp);

	return;
}



sp_tuples * add_tuples(sp_tuples * matA, sp_tuples * matB)
{
    if (matA->m == matB->m && matA->n == matB->n)
    {
        // Allocate memory for sp_tuples
        sp_tuples *matC = malloc(sizeof(sp_tuples));
        if (!matC)
        {
            printf("Error allocating memory for sp_tuples.\n");
            return NULL;
        }
        
        // Initialize members
        matC->m = matA->m;
        matC->n = matA->n;
        matC->nz = 0;
        matC->tuples_head = NULL;

        // Initialize matC to matA
        // Traverse the linked list starting from the head
        sp_tuples_node *curr_ptr = matA->tuples_head;
        while (curr_ptr != NULL)
        {
            int row = curr_ptr->row;
            int col = curr_ptr->col;
            double valueA = curr_ptr->value;
            set_tuples(matC, row, col, valueA);
            // Advance to the next node in the list
            curr_ptr = curr_ptr->next;
        }
        // Add matB to matC
        // Traverse the linked list starting from the head
        curr_ptr = matB->tuples_head;
        while (curr_ptr != NULL)
        {
            int row = curr_ptr->row;
            int col = curr_ptr->col;
            double valueB = curr_ptr->value;
            double valueC = gv_tuples(matC, row, col);
            set_tuples(matC, row, col, valueC + valueB);
            // Advance to the next node in the list
            curr_ptr = curr_ptr->next;
        }
        return matC;
    }
    return NULL;
}



sp_tuples * mult_tuples(sp_tuples * matA, sp_tuples * matB){ 
    return NULL;

}


	
void destroy_tuples(sp_tuples * mat_t)
{
    // Temporary node pointer
    sp_tuples_node *temp_ptr;

    // Traverse the linked list starting from the head
    sp_tuples_node *curr_ptr = mat_t->tuples_head;
    while (curr_ptr != NULL)
    {
        // Hold onto node being removed
        temp_ptr = curr_ptr;
        // Advance to the next node in the list
        curr_ptr = curr_ptr->next;
        // Free unlinked node        
        free(temp_ptr);
    }

    // Free sp_tuples
    free(mat_t);
	
    return;
} 
