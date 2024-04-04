# Proyecto 2 Enero–Marzo 2024 (Pidecola)
# Autores: Sandibel Soares, Carlo Herrera, Juan Cuevas

# Estructura del Proyecto
#                  main.c
#                 /  |   \ 
#                /   |    \ 
# 		        /    |     \ 
#              /     |      \ 
#             /      |       \ 
#            /       |        \ 
#           /        |         \ 
#          /         |          \ 
#         /          |           \ 
# calc_prob.c <--- queue.c --> files_finder.c

# Variables
CC = gcc
CFLAGS = -ansi
LDFLAGS = -lm
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
EXEC = pidecola.out

# Reglas
.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)
	rm -f $(OBJ)

# TODO: Crear el arbol de dependencias, compilar los archivos .c y generar los archivos .o
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f *.o $(EXEC)