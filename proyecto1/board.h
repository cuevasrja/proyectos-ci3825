#ifndef BOARD
#define BOARD

# include "cell.h"

typedef struct board {
    Cell* cells[8][8];
} Board;

Board* newBoard();

void freeBoard(Board* board);

char* get_key(Board* board, int x, int y);

void printBoard(Board* board);

#endif