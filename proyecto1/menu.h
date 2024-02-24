#ifndef MAIN
#define MAIN

# include <pthread.h>
# include <unistd.h>
# include "queue.h"
# include "board.h"

# define INPUT 10

/* Struct para manejar el hilo que maneja el input del usuario para que no se bloquee el proceso principal*/
typedef struct info_menu
{
    /* Con game_running el hilo se pregunta si debe volver a preguntar por input*/
    int game_running;
    int ask_for_input;
    /* Nos indica si se selecciono la casilla de forma que revisamos si es valida la opcion */
    int check_selection;
    /* si la seleccion es valida ponemos en un arreglo la fila y columna de la celda*/
    int check_play; 
    int cursor_in_valid_cell;
    pthread_mutex_t * sem;
    Queue * moves_cursor_queue;
    Board * board;
    
    int exit;
}InfoMenu;

/*
Esta función muestra el menú
*/
void showMenu();

/*
Esta función verifica si el usuario quiere salir
@param options: las opciones que el usuario ingresó
@param id_process_child: el id del proceso hijo
@return 1 si el usuario quiere salir, 0 si no
*/
int willExit(char options[INPUT], int id_process_child);

/*
Esta función verifica si las opciones ingresadas por el usuario son válidas
@param options: las opciones que el usuario ingresó
@return 1 si las opciones son válidas, 0 si no
*/
int isValidOption(char options[INPUT]);

/*
Esta función pide al usuario que ingrese las opciones
@param options: las opciones que el usuario ingresó
*/
void enterOptions(char options[INPUT]);

/*
Esta función es la que usa el hilo que controla el input del usuario
@param struct_info: struct con informacion para poder acceder y actualizar datos del input
*/
void * pthread_input_menu_control(void * struct_info);

#endif