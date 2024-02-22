# include <stdio.h>
# include <stdlib.h>
# include <ctype.h>
# include "board.h"
# include "menu.h"

int main() {

    /* Se crea el tablero */
    Board board = newBoard();

    /* options es un arreglo de caracteres que guarda el input del usuario */
    char options[INPUT];
    int j;
    for (j = 0; j < INPUT; j++) {
        options[j] = '\0';
    }

    /* cursor_position es un arreglo de enteros que indica en que posicion 
       debe estar el cursor.
       La primera posicion corresponde al eje horizontal, si es un entero negativo 
       significa que el cursor debe ir a la izq desde donde esta y si es positivo a la derecha
       La segunda posicion es el eje vertical. Si es un int positivo, el cursor baja, de lo
       contrario sube.
    */
   int cursor_position[2] = {0, 0};
   /* Nos indica si se selecciono la casilla de forma que revisamos si es valida la opcion */
   int check_selection = 0;
   /* si la seleccion es valida ponemos en un arreglo la fila y columna de la celda*/
   int valid_piece_cell[2] = {-1, -1};
   int check_play = 0;

    /* Mientras que no haya un ganador */
    while (!isWinner(&board)) {
        printBoard(&board);
        showMenu();
        enterOptions(options);
        if (willExit(options)){ /* Si el usuario quiere salir, salimos */
            printf("Saliendo...\n");
            break;
        }
        int i = 0;
        /* Mientras que el caracter no sea nulo y no se haya llegado al final del arreglo */
        while(options[i] != '\0' && i < INPUT) {
            switch (tolower(options[i])) {
                case 'w': /* Arriba */
                    printf("Mover arriba\n");
                    cursor_position[1] -=1;
                    break;
                case 'a': /* Izquierda */
                    printf("Mover izquierda\n");
                    cursor_position[0] -=1;
                    break;
                case 's': /* Abajo */
                    printf("Mover abajo\n");
                    cursor_position[1] +=1;
                    break;
                case 'd': /* Derecha */
                    printf("Mover derecha\n");
                    cursor_position[0] +=1;
                    break;
                case 'x': /* Seleccionar pieza */
                    printf("Seleccionar pieza\n");
                    /* Si llegamos hasta aca, significa que ya tenemos 
                    la futura casilla del cursor en cursor position.
                    volvemos i == INPUT para salir del while */
                    
                    /*Si el cursor habia seleccionado una casilla valida, 
                     entonces toca revisar si la nueva accion es una jugada valida*/
                    if (board.cursor.is_cell_valid == 1){
                        check_play = 1;
                    } 
                    else
                    {
                        /* Si el cursor estaba en +, procedemos a ver si es una seleccion
                        correcta */
                        check_selection = 1;
                    }
                    
                    i = INPUT;

                    break;
                default: /* Opción inválida */
                    printf("Opción inválida\n");
                    break;
            }
            i++;
        }
        /* Vamos a mover el cursor */
        move_cursor(&board, cursor_position);
        /* Limpiamos el arreglo */
        cursor_position[0] = 0;
        cursor_position[1] = 0;
        /* Revisamos si hay que chequear una seleccion*/
        if (check_selection == 1)
        {
            is_selection_valid(&board);
            /**/
            valid_piece_cell[0] = board.cursor.board_row;
            valid_piece_cell[1] = board.cursor.board_col;
            check_selection = 0;
        }
        /* Si el cursor esta en un seleccion valida, revisamos si hay que chequear una jugada*/
        if (check_play == 1)
        {
            int is_valid = is_play_valid(&board, valid_piece_cell);
            if (is_valid == 1)
            {
                printf("Se selecciono una jugada valida!\n");
                printf("Se enviara al proceso!\n");
            }
            else
            {
                printf("Jugada invalida!!\n");
                printf("NO se enviara al proceso!\n");
            }
            
            
            board.cursor.is_cell_valid = 0;
            check_play = 0;
        }
        
        


        
    }
    
    /* Si hay un ganador, decimos quien es */
    if (isWinner(&board)) {
        printf("El ganador es: %s\n", board.winner == 0 ? "Usuario" : "IA");
    }
    return 0;
}