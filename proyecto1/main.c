# include <stdio.h>
# include <stdlib.h>
# include <ctype.h>
# include "board.h"
# include "menu.h"

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