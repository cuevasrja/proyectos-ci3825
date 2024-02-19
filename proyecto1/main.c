# include <stdio.h>
# include <stdlib.h>
# include <ctype.h>
# include "main.h"
# include "board.h"

int n = sizeof(menu) / sizeof(menu[0]);

/*
This function prints the menu
*/
void showMenu() {
    for (int i = 0; i < n; i++) {
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

int main() {
    Board board = newBoard();
    char options[INPUT];
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
                    break;
                case 'a': // Left
                    printf("Mover izquierda\n");
                    break;
                case 's': // Down
                    printf("Mover abajo\n");
                    break;
                case 'd': // Right
                    printf("Mover derecha\n");
                    break;
                case 'x': // Select piece
                    printf("Seleccionar pieza\n");
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
    }
    
    
    return 0;
}