#include <stdlib.h>
# include <stdio.h>
#include "cell.h"


// Constructor. Como parametro recibe un puntero a una pieza que por defecto es NULL
Cell* newCell(Piece* piece) {
    Cell* cell = (Cell*)malloc(sizeof(Cell));
    cell->piece = piece;
    return cell;
}

// Destructor
void freeCell(Cell* cell) {
    free(cell);
}

// Setter
void setPiece(Cell* cell, Piece* piece) {
    cell->piece = piece;
}

// Getter
char getValue(Cell* cell) {
    if (cell == NULL) {
        return '-';
    }
    return get_value(cell->piece);
}