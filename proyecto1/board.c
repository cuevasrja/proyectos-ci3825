# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "board.h"

# define N 8
# define M 5

/*
Paciencia inicial de las piezas
- 0: Caballos
- 1: Reyes
*/
int initPatience[2] = {233, 70};

/* Movimientos posibles del caballo */
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

/* Movimientos posibles del rey */
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

/* Constructo del tablero */
Board newBoard() {
    Board board;

    /* Inicializacion del arreglo de piezas */

    /* Inicializacion de los reyes de cada jugador */
    board.white_king_id = 0;
    board.black_king_id = 1;

    initPiece(&board.pieces[board.white_king_id], board.white_king_id, KING, WHITE, 10, initPatience[KING]);
    initPiece(&board.pieces[board.black_king_id], board.black_king_id, KING, BLACK, 10, initPatience[KING]);

    /* Inicializacion de los caballeros de cada jugador */

    int i;
    /* Los caballeros blancos comienzan en la posicion 2 y llegan hasta la pos 8*/
    for (i = 2; i < 9; i++)
    {
        initPiece(&board.pieces[i], i, KNIGHT, WHITE, 3, initPatience[KNIGHT]);
    }

    /* Los caballeros blancos comienzan en la posicion 9 y llegan hasta la pos 15*/
    for (i = 9; i < 16; i++)
    {
        initPiece(&board.pieces[i], i, KNIGHT, BLACK, 3, initPatience[KNIGHT]);
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

/*
Funcion que retorna el tipo de pieza en una celda
@param board tablero de juego
@param cell celda de la cual se quiere obtener el tipo de pieza
@param i coordenada i de la celda
@param j coordenada j de la celda
@return tipo de pieza en la celda
*/
PieceType get_piece_type(Board * board, Cell * cell, int i, int j){
    int index = cell -> matrix[i][j];
    return  board -> pieces[index].type;
}

/*
Funcion que retorna el color de la pieza en una celda
@param board tablero de juego
@param cell celda de la cual se quiere obtener el color de la pieza
@param i coordenada i de la celda
@param j coordenada j de la celda
@return color de la pieza en la celda
*/
PieceColor get_piece_color(Board * board, Cell * cell, int i, int j){
    int index = cell -> matrix[i][j];
    return  board -> pieces[index].color;
}

/*
Funcion que retorna el caracter que representa una pieza
@param Board tablero de juego
@param Cell celda de la cual se quiere obtener el caracter de la pieza
@param i coordenada i de la celda
@param j coordenada j de la celda
@return caracter que representa la pieza
*/
char get_piece_char_ij(Board * board, Cell * cell, int i, int j){
    PieceType type = get_piece_type(board, cell, i, j);
    PieceColor color = get_piece_color(board, cell, i, j);

    return get_piece_char(type, color);
}

/*
Funcion que imprime la tabla de caracteres
@param new_char_cells tabla de caracteres que se quiere imprimir
*/
void print_char_cells(char new_char_cells[33][33]){
    int n = 33;
    int i, j;
    for (i = 0; i < n; i++)
    {
        for ( j = 0; j < n; j++)
        {
            printf("%c", new_char_cells[i][j]);
        }
        printf("\n");
    }

}

/*
Funcion que imprime el tablero de juego
@param board tablero de juego
*/
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

/*
Funcion que revisa si un movimiento es valido
@param board tablero de juego
@param piece pieza que se quiere mover
@param x coordenada x a la que se quiere mover la pieza
@param y coordenada y a la que se quiere mover la pieza
@return 1 si el movimiento es valido, 0 en caso contrario
*/
int isValidMove(Board * board, Piece * piece, int x, int y){
    int currentX = getX(piece);
    int currentY = getY(piece);
    int i;
    if (piece -> type == KNIGHT){
        for (i = 0; i < 8; i++){
            if (currentX + horseMoves[i][0] == x && currentY + horseMoves[i][1] == y){
                return 1;
            }
        }
    }
    else if (piece -> type == KING){
        for (i = 0; i < 8; i++){
            if (currentX + kingMoves[i][0] == x && currentY + kingMoves[i][1] == y){
                return 1;
            }
        }
    }
    return 0;
}

/*
Funcion que busca un movimiento aleatorio para una pieza
@param board tablero de juego
@param piece pieza de la cual se quiere encontrar un movimiento aleatorio
@return puntero a un arreglo de dos enteros que representan el movimiento aleatorio
en caso de que no se pueda realizar un movimiento, retorna NULL
*/
int* getRandomMove(Board * board, Piece * piece){
    int x = getX(piece);
    int y = getY(piece);

    int* validMove = (int*) malloc(2 * sizeof(int));
    /* Verificamos que malloc funcione correctamente */
    if (validMove == NULL) {
        printf("Error al reservar memoria. ");
        printf("getRandomMove(Board * board, Piece * piece) en board.c\n");
        exit(1);
    }

    if (piece -> type == KNIGHT){
        int n = sizeof(horseMoves) / sizeof(horseMoves[0]);
        int random = rand() % n;
        int inBounds = x + horseMoves[random][0] >= 0 && x + horseMoves[random][0] <= 7 && y + horseMoves[random][1] >= 0 && y + horseMoves[random][1] <= 7;
        if (!inBounds) return NULL;
        /* Chequeamos que la casilla destino no tenga como dueño una pieza del mismo equipo */
        Piece* destination = &(board -> pieces[board ->cells[x + horseMoves[random][0]][y + horseMoves[random][1]].owner]);
        if (destination -> color == piece -> color) return NULL;

        validMove[0] += horseMoves[random][0] + x;
        validMove[1] += horseMoves[random][1] + y;
        return validMove;
    }
    else if (piece -> type == KING){
        int n = sizeof(kingMoves) / sizeof(kingMoves[0]);
        int random = rand() % n;
        int inBounds = x + kingMoves[random][0] >= 0 && x + kingMoves[random][0] <= 7 && y + kingMoves[random][1] >= 0 && y + kingMoves[random][1] <= 7;
        if (!inBounds) return NULL;
        /* Chequeamos que la casilla destino no tenga como dueño una pieza del mismo equipo */
        Piece* destination = &(board -> pieces[board ->cells[x + kingMoves[random][0]][y + kingMoves[random][1]].owner]);
        if (destination -> color == piece -> color) return NULL;
        validMove[0] += kingMoves[random][0] + x;
        validMove[1] += kingMoves[random][1] + y;
        return validMove;
    }
    free(validMove);
    return NULL;
}

/*
Función que retorna la distancia de manhattan entre dos puntos
@param x1 coordenada x del primer punto
@param y1 coordenada y del primer punto
@param x2 coordenada x del segundo punto
@param y2 coordenada y del segundo punto
@return distancia de manhattan entre los dos puntos
*/
int manhattanDistance(int x1, int y1, int x2, int y2){
    return abs(x1 - x2) + abs(y1 - y2);
}

/*
Función que retorna la pieza más cercana al caballo
@param board tablero de juego
@param piece pieza de la cual se quiere encontrar la pieza más cercana
@return puntero a la pieza más cercana
*/
Piece* getClosestEnemy(Board * board, Piece * piece){
    /*
    Buscamos las coordenadas de la pieza (x, y)
    */
    int x = getX(piece);
    int y = getY(piece);
    /*
    Inicializamos la distancia mínima (100) y la pieza más cercana (NULL)
    */
    int minDistance = 100;
    Piece * closestEnemy = NULL;
    int numPieces = sizeof(board -> pieces) / sizeof(board -> pieces[0]);
    int i;
    /*
    Iteramos sobre todas las piezas del tablero
    */
    for (i = 0; i < numPieces; i++){
        /*
        Si la pieza es del mismo color que la pieza que estamos analizando, la ignoramos
        */
        if (board -> pieces[i].color == piece -> color){
            continue;
        }
        /*
        En caso contrario, calculamos la distancia de manhattan entre la pieza que estamos 
        analizando y la pieza actual
        */
        int enemyX = getX(&(board -> pieces[i]));
        int enemyY = getY(&(board -> pieces[i]));
        int distance = manhattanDistance(x, y, enemyX, enemyY);
        /*
        Si la distancia es menor que la distancia mínima, actualizamos la distancia mínima
        */
        if (distance < minDistance){
            minDistance = distance;
            closestEnemy = &(board -> pieces[i]);
        }
    }
    return closestEnemy;
}

/*
Actualiza la paciencia de una pieza (en segundos). Para calcular la paciencia, se toma en cuenta
la distancia de manhattan entre la pieza y la pieza más cercana del color contrario, multiplicada
por 100 y dividida entre el valor de la pieza enemiga mas cercana.
@param board tablero de juego
@param piece pieza de la cual se quiere actualizar la paciencia
*/
void updatePatience(Board * board, Piece * piece){
    Piece * closestEnemy = getClosestEnemy(board, piece);
    int x = getX(piece);
    int y = getY(piece);
    int enemyX = getX(closestEnemy);
    int enemyY = getY(closestEnemy);
    int distance = manhattanDistance(x, y, enemyX, enemyY);
    int patience = distance * 100 / closestEnemy -> points;
    setPatience(piece, patience);
}