#ifndef CELL
#define CELL

#include "piece.h"
// ? Y si hay varias piezas ?
typedef struct cell{
    Piece* piece;
} Cell;

Cell* newCell(Piece* piece);

void freeCell(Cell* cell);

void setPiece(Cell* cell, Piece* piece);

#endif