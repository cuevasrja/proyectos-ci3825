#ifndef PIECE
#define PIECE

# include "queue.h"

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
    int cell_row, cell_col;
    int inMovement;
    Queue moves_queue;
    int patience;
} Piece;

void initPiece(Piece * piece, int id, PieceType type, PieceColor color, int points, int patience, int cell_row, int cell_col);

char get_piece_char(PieceType type, PieceColor color);

int getInitPatience(PieceType type);

int get_abs_row(Piece * piece);

int get_abs_col(Piece * piece);

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