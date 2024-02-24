#ifndef MAIN
#define MAIN

# define INPUT 10

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

#endif