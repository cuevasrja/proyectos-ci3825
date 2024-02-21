#ifndef PIECE
#define PIECE

typedef enum pieceType{
    /*Los caballos estan en la columna 0 y los reyes en la 1*/
    KNIGHT = 0,
    KING = 1
} PieceType;

typedef enum PieceColor{
    WHITE = 0,
    BLACK = 1
} PieceColor;

typedef struct piece{
    int id;
    PieceType type;
    PieceColor color;
    int points;
    int x, y;
    int inMovement;
    int patience;
} Piece;

/*
Paciencia inicial de las piezas
- 0: Caballos
- 1: Reyes
*/
int initPatience[2] = {233, 70};

void initPiece(Piece * piece, int id, PieceType type, PieceColor color, int points, int patience);

char get_piece_char(PieceType type, PieceColor color);

int getPoints(Piece * piece);

int getX(Piece * piece);

int getY(Piece * piece);

int getPatience(Piece * piece);

void up(Piece * piece);

void down(Piece * piece);

void right(Piece * piece);

void left(Piece * piece);

void setPosition(Piece * piece, int x, int y);

void setPatience(Piece * piece, int patience);

#endif