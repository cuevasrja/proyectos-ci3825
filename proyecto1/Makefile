C = gcc
SOURCE = piece.h piece.c cell.h cell.c board.h board.c queue.c pthread_pieces_logic.c child_process_logic.c main.c
TESTS = piece.h piece.c cell.h cell.c board.h board.c queue.c pthread_pieces_logic.c child_process_logic.c main_test_com_pipes.c
TARGET = main.out
FLAGS = -pthread

# Cada vez que se ejecute make, se compilará el código fuente
all: $(SOURCE)
	$(C) $(SOURCE) -o $(TARGET) $(FLAGS)

# Cada vez que se ejecute make test, se compilará el código fuente de los tests
test: $(TESTS)
	$(C) $(TESTS) -o test.out $(FLAGS)

# Cada vez que se ejecute make clean, se eliminará el archivo ejecutable
clean:
	rm -f *.out