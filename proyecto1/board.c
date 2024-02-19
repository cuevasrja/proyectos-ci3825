# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "board.h"

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
Board newBoard() {
    Board board;

    /* Inicializacion del arreglo de piezas */

    /* Inicializacion de los reyes de cada jugador */
    board.white_king_id = 0;
    board.black_king_id = 1;

    initPiece(&board.pieces[board.white_king_id], board.white_king_id, KING, WHITE, 10);
    initPiece(&board.pieces[board.black_king_id], board.black_king_id, KING, BLACK, 10);

    /* Inicializacion de los caballeros de cada jugador */

    int i;
    /* Los caballeros blancos comienzan en la posicion 2 y llegan hasta la pos 8*/
    for (i = 2; i < 9; i++)
    {
        initPiece(&board.pieces[i], i, KNIGHT, WHITE, 3);
    }

    /* Los caballeros blancos comienzan en la posicion 9 y llegan hasta la pos 15*/
    for (i = 9; i < 16; i++)
    {
        initPiece(&board.pieces[i], i, KNIGHT, BLACK, 3);
    }

    /* Inicializacion de las celdas del tablero */

    /* Inicializacion de las celdas de los reyes */
    init_cell(&board.cells[0][4], board.black_king_id);
    init_cell(&board.cells[7][4], board.white_king_id);

    /* Inicializacion de las celdas de los caballeros */
    int k = 2;
    int l = 9;
    for (i = 0; i < N; i++) {

        /* Nos saltamos la columna donde estan los reyes */
        if (i == 4)
        {
            k--;
            l--;
            continue;
        }
            


        /* Inicializacion de los caballeros negros */
        init_cell(&board.cells[0][i], i + l);

        /* Inicializacion de los caballeros blancos */
        init_cell(&board.cells[7][i], i + k);

    }

    /* Inicializacion de las celdas sin piezas */
    int j;
    for (i = 1; i < N - 1; i++){
        for (j = 0; j < N; j++){
            init_cell(&board.cells[i][j], -1);
        }
    }

    /* Inicializacion de la tabla de chars para imprimir */

    /* Inicializacion del primer borde horizontal y ultimo borde horizonta;*/
    for (i = 0; i < 33; i++)
    {
        if (i % 4 == 0)
        {
            board.char_cells[0][i] = ' ';
            board.char_cells[32][i] = '|';
        }
        else
        {
            board.char_cells[0][i] = '_';
            board.char_cells[32][i] = '_';
        }
    }

    /* Inicializacion del resto de bordes*/
    for ( i = 1; i < 32; i++)
    {
        for (j = 0; j < 33; j++)
        {
            if (j % 4 == 0)
            {
                board.char_cells[i][j] = '|';
            }
            else if (i % 4 == 0)
            {
                board.char_cells[i][j] = '_';
            }
            else
            {
                board.char_cells[i][j] = ' ';
            }
        }
    }
    
    return board;
}

PieceType get_piece_type(Board * board, Cell * cell, int i, int j){
    int index = cell -> matrix[i][j];
    return  board -> pieces[index].type;
}

PieceColor get_piece_color(Board * board, Cell * cell, int i, int j){
    int index = cell -> matrix[i][j];
    return  board -> pieces[index].color;
}

char get_piece_char_ij(Board * board, Cell * cell, int i, int j){
    PieceType type = get_piece_type(board, cell, i, j);
    PieceColor color = get_piece_color(board, cell, i, j);

    return get_piece_char(type, color);
}

void print_char_cells(char new_char_cells[33][33]){

    int i, j;
    for (i = 0; i < 33; i++)
    {
        for ( j = 0; j < 33; j++)
        {
            printf("%c", new_char_cells[i][j]);
        }
        printf("\n");
    }

}

void printBoard(Board * board){

    char new_char_cells[33][33];
    int i, j, k, l;

    /* Limpiamos la tabla de la iteracion pasada */
    for (i = 0; i < 33; i++)
    {
        for ( j = 0; j < 33; j++)
        {
            new_char_cells[i][j] = board->char_cells[i][j];
        }
        printf("\n");
    }
        

    /* Poscicionamos las piezas donde van */
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            Cell * act_cell = &(board->cells[i][j]);
            for (k = 0; k < 5; k++)
            {
                for (l = 0; l < 5; l++)
                {
                    if ((act_cell -> matrix[k][l]) == -1)
                         continue;

                    char char_piece = get_piece_char_ij(board, act_cell, k, l);
                    new_char_cells[i*4 + k][j*4 + l] = char_piece;
                }
            } 
        }
    }

    print_char_cells(new_char_cells);
}