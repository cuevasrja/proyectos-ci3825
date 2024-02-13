# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "board.h"

# define EMPTY -1
# define WHITE_HORSE 0
# define WHITE_KING 1
# define BLACK_HORSE 2
# define BLACK_KING 3

// Constructor
Board* newBoard() {
    Board* board = (Board*)malloc(sizeof(Board));
    int i;
    for (i = 0; i < 8; i++) {
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
        for (j = 0; j < 8; j++){
            board->cells[i][j] = NULL;
        }
    }

    return board;
}

// Destructor
void freeBoard(Board* board) {
    free(board);
}

void printBoard(Board* board) {
    int i;
    for (i = 0; i < 8; i++) {
        int j;
        for (j = 0; j < 8; j++) {
            printf("%c ", getValue(board->cells[i][j]));
        }
        printf("\n");
    }
    

}