#ifndef CELL
#define CELL

#include "piece.h"
typedef struct cell{
    int owner;
    int matrix[5][5];
} Cell;

void init_cell(Cell * cell, int piece_id);

#endif