# include <stdio.h>
# include <stdlib.h>
# include <ctype.h>
# include <signal.h>
# include "menu.h"

char* menu[] = {
    "\033[1;92mW.\033[0m Mover arriba",
    "\033[1;92mA.\033[0m Mover izquierda",
    "\033[1;92mS.\033[0m Mover abajo",
    "\033[1;92mD.\033[0m Mover derecha",
    "\033[1;92mX.\033[0m Seleccionar pieza",
    "\033[1;92mQ.\033[0m Salir"
};

int menuSize = sizeof(menu) / sizeof(menu[0]);

void showMenu() {
    for (int i = 0; i < menuSize; i++) {
        printf("%s\n", menu[i]);
    }
}

int willExit(char options[INPUT], int id_process_child) {
    int i = 0;
    for (i = 0; i < INPUT; i++) {
        if (tolower(options[i]) == 'q'){
            kill(id_process_child, SIGKILL); 
            exit(0);
        }
    }
    return 0;
}

int isValidOption(char options[INPUT]) {
    int i = 0;
    for (i = 0; i < INPUT; i++) {
        /* Si el caracter es nulo, salimos del ciclo */
        if (options[i] == '\0') {
            break;
        }
        /* Si el caracter no es una opción válida, retornamos 0 */
        if (tolower(options[i]) != 'w' && tolower(options[i]) != 'a' && tolower(options[i]) != 's' && tolower(options[i]) != 'd' && tolower(options[i]) != 'x' && tolower(options[i]) != 'q') {
            return 0;
        }
    }
    return 1;
}

void enterOptions(char options[INPUT]) {
    /* Primero volvemos nulas todas las opciones anteriores */
    int i = 0;
    for (i = 0; i < INPUT; i++) {
        options[i] = '\0';
    }
    /* Luego pedimos al usuario que ingrese las opciones */
    /* Limitamos la entrada a 10 caracteres */
    scanf("%10s", options);
    printf("\033[0m\n");

    /* Verificamos que las opciones sean válidas */
    while (!isValidOption(options)) {
        printf("Opción inválida, por favor ingrese una opción válida (Solo puede ingresar un máximo de %d acciones): \033[1;92m", INPUT);
        scanf("%s", options);
        printf("\033[0m");
    }
    
}

void pthread_input_menu_handler(){
    /*No necesitamos que pase nada, solo se necesita que se interrumpa el hilo */
}


void * pthread_input_menu_control(void * struct_info){
    InfoMenu * state = (InfoMenu *)struct_info; 
    signal(SIGUSR1, pthread_input_menu_handler);
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

    while (state->game_running)
    {
        if (state->ask_for_input == 1)
        {
            enterOptions(options);
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
                        /* Si llegamos hasta aca, significa que ya tenemos 
                        la futura casilla del cursor en cursor position.
                        volvemos i == INPUT para salir del while */
                        
                        /*Si el cursor habia seleccionado una casilla valida, 
                        entonces toca revisar si la nueva accion es una jugada valida*/
                        if (state->cursor_in_valid_cell == 1){
                            state->check_play = 1;
                            printf("Seleccionar casilla de destino\n");

                        } 
                        else
                        {
                            /* Si el cursor estaba en +, procedemos a ver si es una seleccion
                            correcta */
                            state->check_selection = 1;
                            printf("Seleccionar pieza\n");
                        }
                        
                        i = INPUT;

                        break;
                    case 'q': // Exit
                        printf("Salir\n");
                        state->ask_for_input = 0;
                        state->exit = 1;
                        break;
                    default: 
                        printf("Opción inválida\n");
                        break;
                }
                i++;
            }
            
            int * new_move_cursor = calloc(2, sizeof(int));
            new_move_cursor[0] = cursor_position[0];
            new_move_cursor[1] = cursor_position[1];

            pthread_mutex_lock(state->sem);
            enqueue(state->moves_cursor_queue, new_move_cursor);
            pthread_mutex_unlock(state->sem);

            /* Limpiamos el arreglo */
            cursor_position[0] = 0;
            cursor_position[1] = 0;
        }
        else
        {
            pause();
        }
         
    } 

}