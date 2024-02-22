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

    // While the user doesn't want to exit, we keep asking for options
    while (willExit(options)) {
        printBoard(&board);
        showMenu();
        enterOptions(options);
        int i = 0;
        // While the character is not null and we haven't reached the end of the string
        while(options[i] != '\0' && i < INPUT) {
            switch (tolower(options[i])) {
                case 'w': // Up
                    printf("Mover arriba\n");
                    cursor_position[1] -=1;
                    break;
                case 'a': // Left
                    printf("Mover izquierda\n");
                    cursor_position[0] -=1;
                    break;
                case 's': // Down
                    printf("Mover abajo\n");
                    cursor_position[1] +=1;
                    break;
                case 'd': // Right
                    printf("Mover derecha\n");
                    cursor_position[0] +=1;
                    break;
                case 'x': // Select piece
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
                case 'q': // Exit
                    printf("Salir\n");
                    break;
                default: // Invalid option
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
    
    
    return 0;
}