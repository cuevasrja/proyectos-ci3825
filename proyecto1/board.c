# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "board.h"

// Constructor
Board* newBoard() {
    Board* board = (Board*)malloc(sizeof(Board));
    int i;
    for (i = 0; i < 8; i++) {
        if (i == 4){
            board->cells[0][i] = newCell(newPiece("Black", "King", 10));
            board->cells[7][i] = newCell(newPiece("White", "King", 10));
        }
        else {
            board->cells[0][i] = newCell(newPiece("Black", "Horse", 3));
            board->cells[7][i] = newCell(newPiece("White", "Horse", 3));
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

char* get_key(Board* board, int x, int y) {
    if (board->cells[x][y] != NULL) {
        // Unimos el equipo y el tipo de pieza en formato "team_type"
        char* key = (char*)malloc(20 * sizeof(char));
        strcpy(key, getTeam(board->cells[x][y]->piece));
        strcat(key, "_");
        strcat(key, getType(board->cells[x][y]->piece));
        return key;
    }
    return NULL;
}

void printBoard(Board* board) {
    int i;
    for (i = 0; i < 8; i++) {
        int j;
        for (j = 0; j < 8; j++) {
            if (board->cells[i][j] != NULL) {
                printf("%c ", get_value(get_key(board, i, j)));
            }
            else {
                printf("- ");
            }
        }
        printf("\n");
    }
    

}