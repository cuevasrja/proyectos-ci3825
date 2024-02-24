#ifndef BOARD
#define BOARD

#include "cell.h"
#include "piece.h"

typedef enum turn{
    USER = 0,
    IA = 1
} Turn;
typedef struct cursor
{
    /* Coordenadas de la celda de la tabla donde esta el cursor */
    int board_row;
    int board_col;

    /* Coordenadas de la matriz de la celda donde esta el cursor */
    int cell_row;
    int cell_col;

    /* indica si la casilla donde esta el cursor es valida para seleccionar*/
    int is_cell_valid;

}Cursor;

typedef struct board {
    /* Arreglo que contiene las piezas del juego*/
    Piece pieces[16];
    /* Matriz de celdas */
    Cell cells[8][8];
    /* Matriz de chars usada para imprimir */
    char char_cells[33][33];
    /* Arreglo que muestra las piezas en movimiento.
    -1 si la pieza no se mueve
    0  si la pieza se mueve por una orden del jugador
    1  si la pieza se mueve porque perdio la paciencia*/
    int pieces_in_mov[16];
    /* id de los reyes para acceder facilmente*/
    int black_king_id;
    int white_king_id;
    /* Atributo que nos indica de quien es el turno en el juego */
    Turn turn;
    int winner;
    /*  */
    Cursor cursor;
} Board;

/* Constructo del tablero */
Board newBoard();

/*
Funcion que mueve el cursor
@param board tablero de juego
@param cursor_position_input arreglo de dos enteros que representan el movimiento del cursor
*/
void move_cursor(Board * board, int cursor_position_input[2]);

/*
Funcion que retorna el tipo de pieza en una celda
@param board tablero de juego
@param cell celda de la cual se quiere obtener el tipo de pieza
@param i coordenada i de la celda
@param j coordenada j de la celda
@return tipo de pieza en la celda
*/
PieceType get_piece_type(Board * board, Cell * cell, int i, int j);

/*
Funcion que retorna el color de la pieza en una celda
@param board tablero de juego
@param cell celda de la cual se quiere obtener el color de la pieza
@param i coordenada i de la celda
@param j coordenada j de la celda
@return color de la pieza en la celda
*/
PieceColor get_piece_color(Board * board, Cell * cell, int i, int j);

/*
Funcion que retorna el caracter que representa una pieza
@param Board tablero de juego
@param Cell celda de la cual se quiere obtener el caracter de la pieza
@param i coordenada i de la celda
@param j coordenada j de la celda
@return caracter que representa la pieza
*/
char get_piece_char_ij(Board * board, Cell * cell, int i, int j);

/*
Funcion que imprime la tabla de caracteres
@param new_char_cells tabla de caracteres que se quiere imprimir
*/
void print_char_cells(char new_char_cells[33][33]);

/*
Funcion que imprime el tablero de juego
@param board tablero de juego
*/
void printBoard(Board* board);

/*
Funcion que avisar si la seleccion del cursor es valida
@param board tablero de juego
*/
void is_selection_valid(Board * board);

/*
Funcion que revisa si un movimiento es valido
@param board tablero de juego
@param piece pieza que se quiere mover
@param x coordenada x a la que se quiere mover la pieza
@param y coordenada y a la que se quiere mover la pieza
@return 1 si el movimiento es valido, 0 en caso contrario
*/
int isValidMove(Board * board, Piece * piece, int x, int y);

/*
Funcion devuelve si es valido el movimiento de una pieza
@param board tablero de juego
@param valid_piece_cell arreglo de dos enteros que representan la celda de la pieza que se quiere mover
@param cell_target arreglo de dos enteros que representan la celda a la que se quiere mover la pieza
@return 1 si el movimiento es valido, -1 en caso contrario
*/
int compute_path(Board * board, int id_piece, int cell_target[2], int move_type[2]);

/*
Funcion que revisa si un movimiento es valido
@param board tablero de juego
@param valid_piece_cell arreglo de dos enteros que representan la celda de la pieza que se quiere mover
*/
int is_play_valid(Board * board, int valid_piece_cell[2]);

/* 
Funcion que mueve el caracter de una pieza en el tablero
@param board tablero de juego
@param id_piece id de la pieza que se quiere mover
@return Devuelve 1 si se movio de forma correcta la pieza, 0 si hay que hacer que la pieza espere
un segundo para moverse
*/
int move_char_piece(Board * board, int id_piece);

/*
Funcion que mueve una pieza a una nueva posicion
@param board tablero de juego
@param id_piece id de la pieza que se quiere mover
@param des_x coordenada x a la que se quiere mover la pieza
@param des_y coordenada y a la que se quiere mover la pieza
@return -1 si dio error. 0 si la pieza necesita descansar otro segundo. 1 si el movimiento se hizo
*/
int move_piece(Board * board, int id_piece, int des_x, int des_y);

/*
Funcion que busca un movimiento aleatorio para una pieza
@param board tablero de juego
@param piece pieza de la cual se quiere encontrar un movimiento aleatorio
@return puntero a un arreglo de dos enteros que representan el movimiento aleatorio
en caso de que no se pueda realizar un movimiento, retorna NULL
*/
int* getRandomMove(Board * board, Piece * piece);

/*
Función que retorna la distancia de manhattan entre dos puntos
@param x1 coordenada x del primer punto
@param y1 coordenada y del primer punto
@param x2 coordenada x del segundo punto
@param y2 coordenada y del segundo punto
@return distancia de manhattan entre los dos puntos
*/
int manhattanDistance(int x1, int y1, int x2, int y2);

/*
Función que retorna la pieza más cercana al caballo
@param board tablero de juego
@param piece pieza de la cual se quiere encontrar la pieza más cercana
@return puntero a la pieza más cercana
*/
Piece* getClosestEnemy(Board * board, Piece * piece);

/*
Actualiza la paciencia de una pieza (en segundos). Para calcular la paciencia, se toma en cuenta
la distancia de manhattan entre la pieza y la pieza más cercana del color contrario, multiplicada
por 100 y dividida entre el valor de la pieza enemiga mas cercana.
@param board tablero de juego
@param piece pieza de la cual se quiere actualizar la paciencia
*/
void updatePatience(Board * board, Piece * piece);

/*
Funcion que verifica si hay un ganador
@param board tablero de juego
@return 1 si hay un ganador, 0 en caso contrario
*/
int isWinner(Board * board);

/*
Funcion que revisa si al un pieza llegar a una casilla, se come a otra pieza
@param board tablero de juego
@param piece pieza que se quiere mover
@param x coordenada x a la que se quiere mover la pieza
@param y coordenada y a la que se quiere mover la pieza
*/
void checkEat(Board * board, Piece * piece, int x, int y);

#endif