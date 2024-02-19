#ifndef BOARD
#define BOARD

#include "cell.h"
#include "piece.h"

typedef struct board {
    /* Arreglo que contiene las piezas del juego*/
    Piece pieces[16];
    /* Matriz de celdas */
    Cell cells[8][8];
    /* Matriz de chars usada para imprimir */
    char char_cells[33][33];
    /* id de los reyes para acceder facilmente*/
    int black_king_id;
    int white_king_id;
} Board;

Board newBoard();

PieceType get_piece_type(Board * board, Cell * cell, int i, int j);

PieceColor get_piece_color(Board * board, Cell * cell, int i, int j);

char get_piece_char_ij(Board * board, Cell * cell, int i, int j);

void printBoard(Board* board);

#endif