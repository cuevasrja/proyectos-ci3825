#include <stdlib.h>
# include "piece.h"

// Constructor
Piece* newPiece(char* team, char* type, int value) {
    Piece* piece = (Piece*)malloc(sizeof(Piece));
    piece->team = team;
    piece->type = type;
    piece->value = value;
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
char* getType(Piece* piece) {
    return piece->type;
}

char* getTeam(Piece* piece) {
    return piece->team;
}

int getValue(Piece* piece) {
    return piece->value;
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