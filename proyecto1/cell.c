#include <stdlib.h>
#include "cell.h"

// Constructor
Cell* newCell() {
    Cell* cell = (Cell*)malloc(sizeof(Cell));
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