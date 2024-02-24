# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include "piece.h"

/*
Caracteres que representan las piezas
- N: Caballo blanco
- K: Rey blanco
- z: Caballo Negro
- x: Rey Negro
*/
char chessPieces[2][2] = {{'N', 'K'}, {'z', 'x'}};

/*Paciencia inicial de las piezas
- 0: Caballos
- 1: Reyes
*/
int initPatience[2] = {233, 70};

int getInitPatience(PieceType type){
    return initPatience[type];
}

char get_piece_char(PieceType type, PieceColor color){
    return chessPieces[color][type];
}

void initPiece(Piece * piece, int id, PieceType type, PieceColor color, int points, int patience, int cell_row, int cell_col) {

    piece -> id = id;
    piece -> type = type;
    piece -> color = color;
    piece -> points = points;
    piece -> x = 2;
    piece -> y = 2;
    piece -> cell_row = cell_row;
    piece -> cell_col = cell_col;
    piece -> inMovement = 0;
    piece -> moves_queue = new_queue();
    piece -> patience = patience;
}

/* Getters */

int getPoints(Piece * piece) {
    return piece->points;
}

int getX(Piece * piece) {
    return piece->x;
}

int getY(Piece * piece) {
    return piece->y;
}

int getPatience(Piece * piece) {
    return piece->patience;
}

/* Setters */
void up(Piece * piece) {
    piece->y++;
}

void down(Piece * piece) {
    piece->y--;
}

void right(Piece *piece) {
    piece->x++;
}

void left(Piece * piece) {
    piece->x--;
}

void setPosition(Piece * piece, int x, int y) {
    piece->x = x;
    piece->y = y;
}

void setPatience(Piece * piece, int patience) {
    piece->patience = patience;
}


