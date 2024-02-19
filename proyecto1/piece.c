# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include "piece.h"

char chessPieces[2][2] = {{'N', 'K'}, {'z', 'x'}};

char get_piece_char(PieceType type, PieceColor color){
    return chessPieces[color][type];
}

// Constructor
void initPiece(Piece * piece, int id, PieceType type, PieceColor color, int points) {

    piece -> id = id;
    piece -> type = type;
    piece -> color = color;
    piece -> points = points;
    piece -> x = 2;
    piece -> y = 2;
    piece -> inMovement = 0;
}

// Getters

int getPoints(Piece * piece) {
    return piece->points;
}

int getX(Piece * piece) {
    return piece->x;
}

int getY(Piece * piece) {
    return piece->y;
}

// Setters
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