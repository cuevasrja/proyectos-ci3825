#ifndef CHILD_PROCESS_LOGIC
#define CHILD_PROCESS_LOGIC

# include <pthread.h>
# include <poll.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>

# include "queue.h"
# include "pthread_pieces_logic.h"
# include "board.h"
typedef struct request_father
{
    int action;
    int id;
    int patience_act[8];

} RequestFather;

typedef struct input_utils {
    int read_fd;
    pthread_mutex_t * sem;
    Queue * to_do;
} InputUtils;

/*
Funcion que se encarga de manejar las solicitudes del hijo
@param struct_info: estructura que contiene la informacion necesaria para el hilo
*/
void * pthread_input_control_child(void * struct_info);

/*
Funcion que se encarga de manejar las solicitudes del padre
@param struct_info: estructura que contiene la informacion necesaria para el hilo
*/
void * pthread_input_control_father(void * struct_info);

/*
Funcion que inicializa el codigo del hijo
@param child_request: arreglo de dos elementos que contiene los file descriptors para la comunicacion con el padre
@param father_request: arreglo de dos elementos que contiene los file descriptors para la comunicacion con el hijo
*/
void usr_child_code(int child_request[2], int father_request[2]);

/*
Funcion que manejara las peticiones del hijo
@param board: tablero del juego
@param sem_to_do_father: semaforo para manejar la cola de solicitudes del padre
@param to_do_queue_father: cola de solicitudes del padre
*/
int handleChildRequests(Board * board, pthread_mutex_t * sem_to_do_father, Queue * to_do_queue_father);

/*
Función que envía una petición al hijo
@param board: tablero del juego
@param father_request_fd: file descriptor para la comunicación con el hijo
@param action: acción a realizar
@param piece_id: id de la pieza
*/
void send_request_to_child(Board * board, int father_request_fd, int action, int piece_id);


#endif