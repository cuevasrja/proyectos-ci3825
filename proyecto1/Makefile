CC=gcc
CFLAGS=-I. -pthread
DEPS = piece.h cell.h board.h queue.h pthread_pieces_logic.h child_process_logic.h menu.h
OBJ = piece.o cell.o board.o queue.o pthread_pieces_logic.o child_process_logic.o menu.o main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main.out: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
	rm -f *.o

.PHONY: clean

clean:
	rm -f *.o *.out
