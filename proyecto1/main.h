#ifndef MAIN
#define MAIN

# define INPUT 10

char* menu[] = {
    "\033[1;92mW.\033[0m Mover arriba",
    "\033[1;92mA.\033[0m Mover izquierda",
    "\033[1;92mS.\033[0m Mover abajo",
    "\033[1;92mD.\033[0m Mover derecha",
    "\033[1;92mX.\033[0m Seleccionar pieza",
    "\033[1;92mQ.\033[0m Salir"
};

void showMenu();
int willExit(char options[INPUT]);
int isValidOption(char options[INPUT]);
void enterOptions(char options[INPUT]);

#endif