#ifndef PTHREAD_LOGIC
#define PTHREAD_LOGIC

# include <signal.h>
# include <pthread.h>
# include <stdio.h>
# include <pthread.h>
# include <poll.h>
# include <stdlib.h>
# include <unistd.h>

# include "queue.h"
# include "board.h"

/* Struc para representar el estado de cada hilo que representa una pieza*/
typedef struct pPthreadState {
    int id;
    int patience;
    int in_movement;
    int steps;
    int is_alive;
    pthread_mutex_t * sem;
    Queue * request_queue;
} pPthreadState;

typedef struct request_piece
{
    int id_piece;
    int action;

}RequestPiece;

/*
Funcion que maneja las interrupciones de los hilos
*/
void pthread_piece_handler(int signal);

/*
Funcion que maneja la logica de los hilos
@param struct_state: estructura que contiene la informacion del hilo
*/    
void * pthread_piece(void * struct_state);

#endif