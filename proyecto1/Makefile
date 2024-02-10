# TODO: Buscar flag para compilar con hilos
C = gcc
SOURCE = piece.h piece.c cell.h cell.c board.h board.c main.c
TARGET = main.out
FLAGS = -pthread

# Cada vez que se ejecute make, se compilará el código fuente
all: $(SOURCE)
	$(C) $(SOURCE) -o $(TARGET) $(FLAGS)

# Cada vez que se ejecute make clean, se eliminará el archivo ejecutable
clean:
	rm -f $(TARGET)