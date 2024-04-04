# Proyecto 2 Enero–Marzo 2024 (Pidecola)
# Autores: Sandibel Soares, Carlo Herrera, Juan Cuevas

# Estructura del Proyecto
#                  main.c
#                 /      \ 
#                /        \ 
# 		        /          \ 
#              /            \ 
#             /              \ 
#            /                \ 
#           /                  \ 
#          /                    \ 
#         /                      \ 
# calc_prob.c <--- queue.c --> files_finder.c

# Variables
CC = gcc
CFLAGS = -ansi
LDFLAGS = -lm
EXEC = pidecola.out
OBJS = main.o calc_prob.o queue.o files_finder.o

# Reglas
.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	rm -f $(OBJS)

main.o: main.c calc_prob.h queue.h files_finder.h
	$(CC) $(CFLAGS) -c $<

calc_prob.o: calc_prob.c calc_prob.h
	$(CC) $(CFLAGS) -c $<

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c $<

files_finder.o: files_finder.c files_finder.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(EXEC)