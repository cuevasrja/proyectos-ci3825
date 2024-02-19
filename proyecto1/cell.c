#include <stdlib.h>
# include <stdio.h>
#include "cell.h"


// Constructor. Como parametro recibe un puntero a una pieza que por defecto es NULL
void init_cell(Cell * cell, int piece_id) {

    cell -> owner = piece_id;

    int i,j;
    for (i = 0; i < 5; i++)
    {
        for (j = 0 ; j < 5; j++)
        {
            cell -> matrix[i][j] = -1;
        }
    } 
    cell -> matrix[2][2] = piece_id;

}

