# include <stdio.h>
#include <stdlib.h>
# include "board.h"

// Constructor
Board* newBoard() {
    Board* board = (Board*)malloc(sizeof(Board));
    int i;
    for (i = 0; i < 8; i++) {
        if (i == 4){
            setPiece(board->cells[0][i], newPieza("Black", "King", 10));
            setPiece(board->cells[7][i], newPieza("White", "King", 10));
        }
        else {
            setPiece(board->cells[0][i], newPieza("Black", "Horse", 3));
            setPiece(board->cells[7][i], newPieza("White", "Horse", 3));
        }
        if(board == NULL){
            printf("Error al crear el tablero");
        }
    }

    for (i = 1; i < 7; i++){
        int j;
        for (j = 0; j < 8; j++){
            setPiece(board->cells[i][j], NULL);
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
            printf("a ");
        }
        printf("\n");
    }
    

}