#ifndef PIECE
#define PIECE

typedef struct {
    char* key;
    char value;
} dict;

char get_value(char* key);

typedef struct piece{
    char* team;
    char* type;
    int value;
    int x, y;
    int inMovement;
} Piece;

Piece* newPiece(char* team, char* type, int value);

void freePiece(Piece* piece);

char* getType(Piece* piece);

char* getTeam(Piece* piece);

int getValue(Piece* piece);

int getX(Piece* piece);

int getY(Piece* piece);

void up(Piece* piece);

void down(Piece* piece);

void right(Piece* piece);

void left(Piece* piece);

void setPosition(Piece* piece, int x, int y);

#endif