# Proyecto 2 Enero–Marzo 2024 (Pidecola)
# Autores: Sandibel Soares, Carlo Herrera, Juan Cuevas

# Variables
CC = gcc
# CFLAGS = -Wall -Wextra -Werror -std=c99 -g
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