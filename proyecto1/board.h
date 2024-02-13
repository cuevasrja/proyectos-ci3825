#ifndef BOARD
#define BOARD

# include "cell.h"

typedef struct board {
    Cell* cells[8][8];
} Board;

Board* newBoard();

void freeBoard(Board* board);

void printBorder(int x, int y, int i);

void printRow(Cell* row[], int h);

void printBoard(Board* board);

#endif