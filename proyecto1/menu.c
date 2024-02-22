# include <stdio.h>
# include <stdlib.h>
# include <ctype.h>
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

/*
This function prints the menu
*/
void showMenu() {
    for (int i = 0; i < menuSize; i++) {
        printf("%s\n", menu[i]);
    }
}

/*
This function checks if the user wants to exit
@param options: the options the user entered
@return 0 if the user wants to exit, 1 otherwise
*/
int willExit(char options[INPUT]) {
    int i = 0;
    for (i = 0; i < INPUT; i++) {
        if (tolower(options[i]) == 'q'){
            return 0;
        }
    }
    return 1;
}

/*
This function checks if the options entered by the user are valid
@param options: the options the user entered
@return 1 if the options are valid, 0 otherwise
*/
int isValidOption(char options[INPUT]) {
    int i = 0;
    for (i = 0; i < INPUT; i++) {
        // If the character is null, we break the loop because we reached the end of the string
        if (options[i] == '\0') {
            break;
        }
        // If the character is not a valid option, we return 0
        if (tolower(options[i]) != 'w' && tolower(options[i]) != 'a' && tolower(options[i]) != 's' && tolower(options[i]) != 'd' && tolower(options[i]) != 'x' && tolower(options[i]) != 'q') {
            return 0;
        }
    }
    return 1;
}

/*
This function asks the user to enter the options
@param options: The array where the options will be stored
*/
void enterOptions(char options[INPUT]) {
    // First we clean the options
    int i = 0;
    for (i = 0; i < INPUT; i++) {
        options[i] = '\0';
    }
    // We ask the user to enter the options
    printf("Ingrese una opción (Solo puede ingresar un máximo de %d acciones): \033[1;92m", INPUT);
    scanf("%s", options);
    printf("\033[0m");

    // We check if all the options are valid
    while (!isValidOption(options)) {
        printf("Opción inválida, por favor ingrese una opción válida (Solo puede ingresar un máximo de %d acciones): \033[1;92m", INPUT);
        scanf("%s", options);
        printf("\033[0m");
    }
    
}