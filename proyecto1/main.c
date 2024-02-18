# include <stdio.h>
# include <stdlib.h>
# include <ctype.h>
# include "main.h"
# include "board.h"

void showMenu() {
    int n = sizeof(menu) / sizeof(menu[0]);
    for (int i = 0; i < n; i++) {
        printf("%s\n", menu[i]);
    }
}

int willExit(char options[INPUT]) {
    int i = 0;
    for (i = 0; i < INPUT; i++) {
        if (tolower(options[i]) == 'q'){
            return 0;
        }
    }
    return 1;
}

int isValidOption(char options[INPUT]) {
    int i = 0;
    for (i = 0; i < INPUT; i++) {
        if (options[i] == '\0') {
            break;
        }
        if (tolower(options[i]) != 'w' && tolower(options[i]) != 'a' && tolower(options[i]) != 's' && tolower(options[i]) != 'd' && tolower(options[i]) != 'x' && tolower(options[i]) != 'q') {
            return 0;
        }
    }
    return 1;
}

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
    Board* board = newBoard();
    char options[INPUT];
    while (willExit(options)) {
        printBoard(board);
        showMenu();
        enterOptions(options);
        int i = 0;
        while(options[i] != '\0' && i < INPUT) {
            switch (tolower(options[i])) {
                case 'w':
                    printf("Mover arriba\n");
                    break;
                case 'a':
                    printf("Mover izquierda\n");
                    break;
                case 's':
                    printf("Mover abajo\n");
                    break;
                case 'd':
                    printf("Mover derecha\n");
                    break;
                case 'x':
                    printf("Seleccionar pieza\n");
                    break;
                case 'q':
                    printf("Salir\n");
                    break;
                default:
                    printf("Opción inválida\n");
                    break;
            }
            i++;
        }
    }
    
    
    return 0;
}