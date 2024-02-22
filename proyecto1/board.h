#ifndef BOARD
#define BOARD

#include "cell.h"
#include "piece.h"

typedef enum turn{
    USER = 0,
    IA = 1
} Turn;
typedef struct cursor
{
    /* Coordenadas de la celda de la tabla donde esta el cursor */
    int board_row;
    int board_col;

    /* Coordenadas de la matriz de la celda donde esta el cursor */
    int cell_row;
    int cell_col;

    /* indica si la casilla donde esta el cursor es valida para seleccionar*/
    int is_cell_valid;

}Cursor;

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
    Turn turn;
    int winner;
    Cursor cursor;
} Board;

Board newBoard();

void move_cursor(Board * board, int cursor_position_input[2]);

PieceType get_piece_type(Board * board, Cell * cell, int i, int j);

PieceColor get_piece_color(Board * board, Cell * cell, int i, int j);

char get_piece_char_ij(Board * board, Cell * cell, int i, int j);

void print_char_cells(char new_char_cells[33][33]);

void move_cursor(Board * board, int cursor_position_input[2]);

void is_selection_valid(Board * board);

int isValidMove(Board * board, Piece * piece, int x, int y);

int is_play_valid(Board * board, int valid_piece_cell[2]);

void printBoard(Board* board);

int* getRandomMove(Board * board, Piece * piece);

int manhattanDistance(int x1, int y1, int x2, int y2);

Piece* getClosestEnemy(Board * board, Piece * piece);

void updatePatience(Board * board, Piece * piece);

int isWinner(Board * board);

void checkEat(Board * board, Piece * piece, int x, int y);

#endif