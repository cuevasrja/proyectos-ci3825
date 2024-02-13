# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include "piece.h"

char chessPieces[] = {
    'N',
    'K',
    'z',
    'x'
};

char get_value(Piece* piece){
    if(piece == NULL || piece->type < 0){
        return '-';
    }
    return chessPieces[piece->type];
}

// Constructor
Piece* newPiece(int type, int points) {
    Piece* piece = (Piece*)malloc(sizeof(Piece));
    piece->type = type;
    piece->points = points;
    piece->x = 2;
    piece->y = 2;
    piece->inMovement = 0;
    return piece;
}

// Destructor
void freePiece(Piece* piece) {
    free(piece);
}

// Getters
int getType(Piece* piece) {
    return piece->type;
}

int getPoints(Piece* piece) {
    return piece->points;
}

int getX(Piece* piece) {
    return piece->x;
}

int getY(Piece* piece) {
    return piece->y;
}

// TODO: Buscar una forma de retornar un arreglo de enteros
// int* getPosicion(piece* piece) {
//     int* posicion = (int*)malloc(2 * sizeof(int));
//     posicion[0] = piece->x;
//     posicion[1] = piece->y;
//     return posicion;
// }

// Setters
void up(Piece* piece) {
    piece->y++;
}

void down(Piece* piece) {
    piece->y--;
}

void right(Piece* piece) {
    piece->x++;
}

void left(Piece* piece) {
    piece->x--;
}

void setPosition(Piece* piece, int x, int y) {
    piece->x = x;
    piece->y = y;
}