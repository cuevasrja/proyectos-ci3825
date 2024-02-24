#ifndef CELL
#define CELL

#include "piece.h"
typedef struct cell{
    int owner;
    int matrix[5][5];
} Cell;

/*
Constructor. Como parametro recibe un puntero a una pieza que por defecto es NULL
@param cell: puntero a la celda que se va a inicializar
@param piece_id: id de la pieza que se va a inicializar
*/
void init_cell(Cell * cell, int piece_id);

#endif