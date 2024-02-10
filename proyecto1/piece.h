#ifndef PIECE
#define PIECE

typedef struct piece{
    char* team;
    char* type;
    int value;
    int x, y;
    int inMovement;
} Piece;

// TODO: Declarar metodos
Piece* newPiece(char* team, char* type, int value);

#endif