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
    printf("Ingrese una opción (Solo puede ingresar un máximo de %d acciones): \033[1;92m", INPUT);
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