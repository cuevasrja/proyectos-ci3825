# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "board.h"

# define EMPTY -1
# define WHITE_HORSE 0
# define WHITE_KING 1
# define BLACK_HORSE 2
# define BLACK_KING 3

# define N 8
# define M 5

int horseMoves[8][2] = {
    {2, 1},
    {1, 2},
    {-1, 2},
    {-2, 1},
    {-2, -1},
    {-1, -2},
    {1, -2},
    {2, -1}
};

int kingMoves[8][2] = {
    {1, 0},
    {1, 1},
    {0, 1},
    {-1, 1},
    {-1, 0},
    {-1, -1},
    {0, -1},
    {1, -1}
};

// Constructor
Board* newBoard() {
    Board* board = (Board*)malloc(sizeof(Board));
    int i;
    for (i = 0; i < N; i++) {
        if (i == 4){
            board->cells[0][i] = newCell(newPiece(BLACK_KING, 10));
            board->cells[7][i] = newCell(newPiece(WHITE_KING, 10));
        }
        else {
            board->cells[0][i] = newCell(newPiece(BLACK_HORSE, 3));
            board->cells[7][i] = newCell(newPiece(WHITE_HORSE, 3));
        }
        if(board == NULL){
            printf("Error al crear el tablero");
        }
    }

    for (i = 1; i < 7; i++){
        int j;
        for (j = 0; j < N; j++){
            board->cells[i][j] = NULL;
        }
    }

    return board;
}

// Destructor
void freeBoard(Board* board) {
    free(board);
}

void printBorder(int x, int y, int i){
    if (y == 0) printf("-");
    else if(x == 0 || (i == N - 1 && x == M - 1)) printf("|");
    else printf(" ");
}

void printRow(Cell* row[], int h){
    int i;
    for (i = 0; i < N; i++){
        int j;
        for (j = 0; j < M; j++){
            Piece* piece = getPieceIn(row[i], j, h);
            if (piece == NULL) printBorder(j, h, i);
            else printf("%c", get_value(piece));
        }
    }
    printf("\n");
}

void printBoard(Board* board){
    int i;
    for (i = 0; i < N; i++) {
        int j;
        for (j = 0; j < M; j++) {
            printRow(board->cells[i], j);
        }
    }
    for (i = 0; i < N*M; i++){
        printf("-");
    }
    printf("\n");
}