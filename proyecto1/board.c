# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "board.h"

# define N 8
# define M 5

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

/* Caracteres para el cursor*/
char cursor_chars[2] = {'+', '*'};

/* Constructo del tablero */
Board newBoard() {
    Board board;

    /* Inicializacion del arreglo de piezas */

    /* Inicializacion de los reyes de cada jugador */
    board.white_king_id = 0;
    board.black_king_id = 8;

    initPiece(&board.pieces[board.white_king_id], board.white_king_id, KING, WHITE, 10, getInitPatience(KING), 7, 4);
    initPiece(&board.pieces[board.black_king_id], board.black_king_id, KING, BLACK, 10, getInitPatience(KING), 0, 4);

    /* Inicializacion de los caballeros de cada jugador */

    int i;
    /* Los caballeros blancos comienzan en la posicion 1 y llegan hasta la pos 7*/
    int r = 1;
    for (i = 1; i < 8; i++)
    {
        if (i == 5)
            r--;
        
        initPiece(&board.pieces[i], i, KNIGHT, WHITE, 3, getInitPatience(KNIGHT), 7, i - r);
    }

    /* Los caballeros blancos comienzan en la posicion 9 y llegan hasta la pos 15*/
    r = 9;
    for (i = 9; i < 16; i++)
    {
        initPiece(&board.pieces[i], i, KNIGHT, BLACK, 3, getInitPatience(KNIGHT), 0 , i - r);
    }

    /* Inicializacion de las celdas del tablero */

    /* Inicializacion de las celdas de los reyes */
    init_cell(&board.cells[0][4], board.black_king_id);
    init_cell(&board.cells[7][4], board.white_king_id);

    /* Inicializacion de las celdas de los caballeros */
    int k = 1;
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
    for (i = 1; i < 32; i++)
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

    /* Inicializacion del cursor */

    board.cursor.board_row = 3;
    board.cursor.board_col = 3;
    board.cursor.cell_row = 2;
    board.cursor.cell_col = 2;
    board.cursor.is_cell_valid = 0;

    /* El juego comienza con el turno del usuario */
    board.turn = USER;

    /* No hay ganador al principio */
    board.winner = -1;
    /* Inicializacion del arreglo de movimientos y  del arreglo de destinos*/

    for (i = 0; i < 16; i++)
    {
        board.pieces_in_mov[i] = -1;
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

    /* Dudoso */
    /* Posicionamos el cursor */
    int cur_board_row = board->cursor.board_row;
    int cur_board_col = board->cursor.board_col;
    int cur_cell_row  = board->cursor.cell_row;
    int cur_cell_col  = board->cursor.cell_col;
    int c = board->cursor.is_cell_valid;
    char cur_char = cursor_chars[c];
    new_char_cells[cur_board_row*4 + cur_cell_row][cur_board_col*4 + cur_cell_col] = cur_char;

    print_char_cells(new_char_cells);
}

/*
Funcion que mueve el cursor
@param board tablero de juego
@param cursor_position_input arreglo de dos enteros que representan el movimiento del cursor
*/
void move_cursor(Board * board, int cursor_position_input[2]){

    int board_row = board->cursor.board_row;
    int board_col = board->cursor.board_col; 
    int cell_row = board->cursor.cell_row;
    int cell_col = board->cursor.cell_col;
    int des_x = cursor_position_input[0];
    int des_y = cursor_position_input[1];

    int new_board_row;
    int new_board_col;
    int new_cell_row;
    int new_cell_col;

    int abs_row = board_row*M + cell_row + des_y;
    int abs_col = board_col*M + cell_col + des_x;

    if (abs_row >= N*M)
    {
        new_board_row = N - 1;
        new_cell_row = M - 1;
    }
    else if (abs_row < 0)
    {
        new_board_row = 0;
        new_cell_row = 0;
    }
    else
    {
        /* Si no se excede de ninguna forma los bordes de la tabla*/
        new_board_row = abs_row/M;

        if (des_y >= 0)
        {
            new_cell_row = (cell_row + des_y)%M;
        } 
        else
        {
            new_cell_row = (-1*des_y)%M;
            new_cell_row = M - new_cell_row;
            new_cell_row = (cell_row + new_cell_row)%M;
        }
        
    }


    if (abs_col >= N*M)
    {
        new_board_col = N - 1;
        new_cell_col = M - 1;
    }
    else if (abs_col < 0)
    {
        new_board_col = 0;
        new_cell_col = 0;
    }
    else
    {
        /* Si no se excede de ninguna forma los bordes de la tabla*/
        new_board_col = abs_col/M;

        if (des_x >= 0)
        {
            new_cell_col = (cell_col + des_x)%M;
        } 
        else
        {
            new_cell_col = (-1*des_x)%M;
            new_cell_col = M - new_cell_col;
            new_cell_col = (cell_col + new_cell_col)%M;
        }
        
    }

    /* Actualizamos la posicion del cursor */
    board->cursor.board_col = new_board_col;
    board->cursor.board_row = new_board_row;
    board->cursor.cell_col  = new_cell_col;
    board->cursor.cell_row  = new_cell_row;

}

/*
Funcion que avisar si la seleccion del cursor es valida
@param board tablero de juego
*/
void is_selection_valid(Board * board){

    if (board->turn != USER)
    {
        printf("No es turno del usuario\n");
        return;
    }

    /* Accedemos a la posicion del cursor */
    int board_row = board->cursor.board_row;
    int board_col = board->cursor.board_col; 
    int cell_row = board->cursor.cell_row;
    int cell_col = board->cursor.cell_col;

    /* Accedemos al dueño de la casilla */
    int cell_owner = board->cells[board_row][board_col].owner;

    if (cell_owner == -1)
    {
        printf("No hay ninguna pieza en esa casilla\n");
        board->cursor.is_cell_valid = 0;
        return;
    }

    /* Si la casilla tiene dueño, accedemos al arreglo de piezas para saber de que team es*/
    PieceColor color_piece = board->pieces[cell_owner].color;

    if (color_piece == WHITE)
    {
        printf("te pertence la pieza!");
        board->cursor.is_cell_valid = 1;
        return;
    }


    printf("La pieza no te pertenece \n");
    board->cursor.is_cell_valid = 0;
    
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
Funcion devuelve si es valido el movimiento de una pieza
@param board tablero de juego
@param valid_piece_cell arreglo de dos enteros que representan la celda de la pieza que se quiere mover
*/
/* Devuelve -1 si salio mal. 1 si se hizo bien*/
int compute_path(Board * board, int id_piece, int cell_target[2], int move_type[2]){

    printf("Dentro de la funcion que calcula el camino\n");
    if (id_piece < 0 || id_piece > 15)
    {
        perror("error en compute_path. id de pieza invalido\n");
        return -1;
    }
    
    Piece * piece = &board->pieces[id_piece];
    int num_steps = 0;


    if (piece->type == KING)
    {
        printf("Calculando el camino del rey\n");
        /* Centramos la pieza en su celda si la pieza no lo estaba.*/
        if (piece->x != 2)
        {
            /* cuantos pasos hay que tomar para posicionarnos en el centro de la casilla*/
            int c = 2 - piece->x;

            int i, n;
            int k = 1;
            if (c < 0){
                n = -1*c;
                k = -1;
            }         
            /* creamos un movimiento para cada paso*/
            for (i = 0; i < n; i++)
            {
                int * move = calloc(2, sizeof(int));
                if (move == NULL) {
                    printf("Error al reservar memoria para el movimiento.\n ");
                    return -1;
                }
                move[0] = k;
                move[1] = 0;
                enqueue(&piece->moves_queue, move);
            }  
        }
        if (piece->y != 2)
        {
            int c = 2 - piece->y;

            int i, n;
            int k = 1;
            if (c < 0){
                n = -1*c;
                k = -1;
            }         
            for (i = 0; i < n; i++)
            {
                int * move = calloc(2, sizeof(int));
                if (move == NULL) {
                    printf("Error al reservar memoria para el movimiento.\n ");
                    exit(1);
                }
                move[0] = 0;
                move[1] = k;
                enqueue(&piece->moves_queue, move);
            }  
        }
        
        printf("cell_target[0]: %d\n", cell_target[0]);
        printf("cell_target[1]: %d\n", cell_target[1]);
        printf("piece.cell_col: %d\n", piece->cell_col);
        printf("piece.cell_row: %d\n", piece->cell_row);
        
        int dx = cell_target[1]*M  - piece->cell_col*M;
        int dy = cell_target[0]*M  - piece->cell_row*M;

        if (dy < 0)
            dy = -1*dy;
        if (dx < 0)
            dx = -1*dx;
        
        printf("dx: %d dy: %d \n", dx, dy);


        while(dx > 0 || dy > 0){
            int * move = calloc(2, sizeof(int));
            if (move == NULL) {
                printf("Error al reservar memoria para el movimiento.\n ");
                exit(1);
            }
            // Hay que revisar si esto funciona
            move[0] = -1*move_type[0];
            move[1] = -1*move_type[1];
            printf("compute_path. move[0]: %d move[1]: %d \n", move[0], move[1]);
            enqueue(&piece->moves_queue, move);
            printf("Cantidad de elementos en la cola %d \n", piece->moves_queue.length);

            if (dx > 0)
                dx--;

            if (dy > 0)
            dy--;
        }
        
    }
    else if (piece->type == KNIGHT)
    {
        /* Centramos la pieza*/
        if (piece->x != 2)
        {
            int c = 2 - piece->x;
            int i, n;
            int k = 1;
            if (c < 0){
                n = -1*c;
                k = -1;
            }         
            /* creamos un movimiento para cada paso*/
            for (i = 0; i < n; i++)
            {
                int * move = calloc(2, sizeof(int));
                if (move == NULL) {
                    printf("Error al reservar memoria para el movimiento.\n ");
                    return -1;
                }
                move[0] = k;
                move[1] = 0;
                enqueue(&piece->moves_queue, move);
            }  
        }
        if (piece->y != 2)
        {
            int c = 2 - piece->y;

            int i, n;
            int k = 1;
            if (c < 0){
                n = -1*c;
                k = -1;
            }         
            for (i = 0; i < n; i++)
            {
                int * move = calloc(2, sizeof(int));
                if (move == NULL) {
                    printf("Error al reservar memoria para el movimiento.\n ");
                    exit(1);
                }
                move[0] = 0;
                move[1] = k;
                enqueue(&piece->moves_queue, move);
            }  
        }

        int dx = cell_target[1]*M  - piece->cell_col*M;
        int dy = cell_target[0]*M  - piece->cell_row*M;

        if (dy < 0)
        {
            dy  *= -1;
        }

        if (dx < 0)
        {
            dx  *= -1;
        }      

        /* Asumiendo que el primer elemento es en las filas y el segundo en las columnas*/
        if (move_type[0] > move_type[1])
        {

            /* Primero realizamos el movimiento vertical*/
            while(dy > 0){
                int * move = calloc(2, sizeof(int));
                if (move == NULL) {
                    printf("Error al reservar memoria para el movimiento.\n ");
                    return -1;
                }

                move[0] = -1*move_type[0]/2;
                move[1] = 0;
                printf("compute_path. move[0]: %d move[1]: %d \n", move[0], move[1]);
                enqueue(&piece->moves_queue, move);
                printf("Cantidad de elementos en la cola %d \n", piece->moves_queue.length);
                dy--;
            }

            /* Ahora el horizontal */
            while(dx > 0){
                int * move = calloc(2, sizeof(int));
                if (move == NULL) {
                    printf("Error al reservar memoria para el movimiento.\n ");
                    return -1;
                }
                // Hay que revisar si esto funciona
                move[0] = 0;
                move[1] = -1*move_type[1];
                printf("compute_path. move[0]: %d move[1]: %d \n", move[0], move[1]);
                enqueue(&piece->moves_queue, move);
                printf("Cantidad de elementos en la cola %d \n", piece->moves_queue.length);
                dx--;
            }
        } 
        else if (move_type[0] < move_type[1])
        {

            /* Primero realizamos el movimiento hor*/
            while(dx > 0){
                int * move = calloc(2, sizeof(int));
                if (move == NULL) {
                    printf("Error al reservar memoria para el movimiento.\n ");
                    return -1;
                }
                // Hay que revisar si esto funciona
                move[0] = 0;
                move[1] = -1*move_type[1]/2;
                printf("compute_path. move[0]: %d move[1]: %d \n", move[0], move[1]);
                enqueue(&piece->moves_queue, move);
                printf("Cantidad de elementos en la cola %d \n", piece->moves_queue.length);
                dx--;
            }

            /* Ahora el ver */
            while(dy > 0){
                int * move = calloc(2, sizeof(int));
                if (move == NULL) {
                    printf("Error al reservar memoria para el movimiento.\n ");
                    return -1;
                }
                // Hay que revisar si esto funciona
                move[0] = -1*move_type[0];
                move[1] = 0;
                printf("compute_path. move[0]: %d move[1]: %d \n", move[0], move[1]);
                enqueue(&piece->moves_queue, move);
                printf("Cantidad de elementos en la cola %d \n", piece->moves_queue.length);
                dy--;
            }
        }
        else
        {
            return -1;
        }

    }
    else
    {
        perror("error en compute_path. Tipo de pieza invalido \n");
        return -1;
    }
    
    return 1;
}

int is_play_valid(Board * board, int valid_piece_cell[2]){

    int is_valid = 1;
    int move_valid = 0;

    if (board->turn != USER)
    {
        is_valid = 0;
        printf("No es tu turno");
        return is_valid;
    }

    /* Accedemos a la posicion del cursor */
    int board_row = board->cursor.board_row;
    int board_col = board->cursor.board_col; 
    int cell_row = board->cursor.cell_row;
    int cell_col = board->cursor.cell_col;

    /* Accedemos al dueño de la casilla */
    int cell_owner = board->cells[board_row][board_col].owner;

    if (cell_owner != -1)
    {
        PieceColor color_piece = board->pieces[cell_owner].color;
        /* Si es una pieza del jugador entonces no es valido */
        if (color_piece == WHITE)
        {
            is_valid = 0;
            printf("Hay una pieza tuya ahi!\n");
            return is_valid;
        }
    }

    /* Revisamos si el tipo de pieza puede moverse a esa casilla*/
    int piece_select = board->cells[valid_piece_cell[0]][valid_piece_cell[1]].owner;
    Piece* piece = &(board->pieces[piece_select]);
    PieceType type_piece = board->pieces[piece_select].type;

    printf("Tipo de pieza escogida: %d \n", type_piece);
        
    int i, j, k;
    /* Si es un rey */
    if (type_piece == 1)
    {              
        for (i = 0; i < 8; i++)
        {

                j = board_row + kingMoves[i][0];
                k = board_col + kingMoves[i][1];
                printf("j: %d, k: %d, rc: %d, cc: %d \n", j, k, valid_piece_cell[0], valid_piece_cell[1]);
                if (j != valid_piece_cell[0] || k != valid_piece_cell[1])
                {
                    continue;
                }
                printf("Se va a computar el camino a tomar para el rey\n");
                move_valid = 1;
                /* Si el movimiento era valido, hay que calcular su trayectoria */
                int target[2] = {board_row, board_col};
                printf("board row del target %d. board col del target: %d \n", board_row, board_col);
                compute_path(board, piece_select, target, kingMoves[i]);
                break;

        }
    } 
    else
    {
        for (i = 0; i < 8; i++)
        {
                j = board_row + horseMoves[i][0];
                k = board_col + horseMoves[i][1];
                if (j != valid_piece_cell[0] || k != valid_piece_cell[1])
                {
                    continue;
                }
                move_valid = 1;
                printf("Se va a computar el camino a toma\nr");
                /* Si el movimiento era valido, hay que calcular su trayectoria */
                int target[2] = {board_row, board_col};
                compute_path(board, piece_select, target, horseMoves[i]);
                break;

        }
    }

    if (board_row == valid_piece_cell[0] && board_col == valid_piece_cell[1])
    {
        move_valid = 1;
    }

    if (move_valid == 0)
    {
        printf("No se puede hacer ese movimiento\n");
        is_valid = 0;
    }

    return is_valid;
    
}

/* Devuelve 1 si se movio de forma correcta la pieza, 0 si hay que hacer que la pieza espere
un segundo para moverse*/
int move_char_piece(Board * board, int id_piece){

    printf("move_char_piece id: %d \n", id_piece);
    if (id_piece < 0 || id_piece > 15)
    {
        perror("error move_char_piece. id de la pieza invalido\n");
        exit(1);
    }
 
    Piece * piece = &board->pieces[id_piece];
    int * move_ref;
    int move[2];
    if (piece->moves_queue.length > 0)
    {
        move_ref = peek(&piece->moves_queue);
        move[0] = move_ref[0];
        move[1] = move_ref[1];
    } else
    {
        perror("error move_char_piece. Cola de movimientos vacia\n");
        return -1;
    }

    int move_success;

    move_success = move_piece(board, id_piece, move[1], move[0]);

    if (move_success != 0)
    {
        dequeue(&piece->moves_queue);
        free(move_ref);
    }

    /* Si se realizo el ultimo movimiento, actualizamos el dueño*/
    if (move_success != 0 && piece->moves_queue.length == 0 )
    {
        board->cells[piece->cell_row][piece->cell_col].owner = piece->id;
    }
    
    
    return move_success;

}

/* -1 si dio error. 0 si la pieza necesita descansar otro segundo. 1 si el movimiento se hizo*/
int move_piece(Board * board, int id_piece, int des_x, int des_y){

    printf("move_piece: Recibi un movimiento \n");
    fflush(stdout);

    if (id_piece < 0 || id_piece > 15)
    {
        perror("id invalido. move_piece");
        return -1;
    }
    
    Piece * piece = &board->pieces[id_piece];
    int new_board_row;
    int new_board_col;
    int new_cell_row;
    int new_cell_col;

    /* Asumiendo que y son las filas y x son las columnas*/
    int abs_row = piece->cell_row*M + piece->y + des_y;
    int abs_col = piece->cell_col*M + piece->x + des_x;

    if (abs_row < 0 || abs_col < 0 || abs_row >= N*M || abs_col >= N*M)
    {
        return -1;
    }
    
    new_board_row = abs_row/M;

    if (des_y >= 0)
    {
        new_cell_row = (piece->y + des_y)%M;
    } 
    else
    {
        new_cell_row = (-1*des_y)%M;
        new_cell_row = M - new_cell_row;
        new_cell_row = (piece->y + new_cell_row)%M;
    }

    new_board_col = abs_col/M;

    if (des_x >= 0)
    {
        new_cell_col = (piece->x + des_x)%M;
    } 
    else
    {
        new_cell_col = (-1*des_x)%M;
        new_cell_col = M - new_cell_col;
        new_cell_col = (piece->x + new_cell_col)%M;
    }

    /*Revisamos si esa posicion no tiene una pieza */
    if (board->cells[new_board_row][new_board_col].matrix[new_cell_row][new_cell_col] != -1)
    {
        /* La pieza necesita descansar un segundo mas */
        return 0;
    }
     
    /* Actualizamos los datos de la pieza y de las celdas */
    board->cells[piece->cell_row][piece->cell_col].matrix[piece->y][piece->x] = -1;

    piece->x = new_cell_col;
    piece->y = new_cell_row;
    piece->cell_row = new_board_row;
    piece->cell_col = new_board_col;

    board->cells[piece->cell_row][piece->cell_col].matrix[piece->y][piece->x] = piece->id;
    
    return 1;
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

/*
Funcion que verifica si hay un ganador
@param board tablero de juego
@return 1 si hay un ganador, 0 en caso contrario
*/
int isWinner(Board * board){
    return board -> winner != -1;
}

/*
Funcion que revisa si al un pieza llegar a una casilla, se come a otra pieza
@param board tablero de juego
@param piece pieza que se quiere mover
@param x coordenada x a la que se quiere mover la pieza
@param y coordenada y a la que se quiere mover la pieza
*/
void checkEat(Board * board, Piece * piece, int x, int y){
    /* Chequeamos que la celda no este vacia */
    if (board -> cells[x][y].owner == -1) return;

    Piece* destination = &(board -> pieces[board -> cells[x][y].owner]);
    /* Chequeamos que la pieza no sea del mismo color */
    if (destination -> color == piece -> color) return;
    /* Chequeamos que la pieza no sea un rey, si lo es, el juego termina */
    if (destination -> type == KING){
        board -> winner = piece -> color;
    }
    board -> cells[x][y].owner = piece -> id;
    piece -> x = x;
    piece -> y = y;
    destination -> x = -1;
    destination -> y = -1;
    destination -> id = -1;
}