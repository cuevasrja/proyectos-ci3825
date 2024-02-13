#ifndef PIECE
#define PIECE

typedef struct piece{
    int type;
    int points;
    int x, y;
    int inMovement;
} Piece;

char get_value(Piece* piece);

Piece* newPiece(int type, int points);

void freePiece(Piece* piece);

int getType(Piece* piece);

int getPoints(Piece* piece);

int getX(Piece* piece);

int getY(Piece* piece);

void up(Piece* piece);

void down(Piece* piece);

void right(Piece* piece);

void left(Piece* piece);

void setPosition(Piece* piece, int x, int y);

#endif