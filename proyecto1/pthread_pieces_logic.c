#include "pthread_pieces_logic.h"

void pthread_piece_handler(int signal){
    printf("Ha ocurrido una interrupcion de un hilo \n");
}

void * pthread_piece(void * struct_state){

    pPthreadState * state = (pPthreadState *)struct_state;
    signal(SIGUSR1, pthread_piece_handler);

    printf("HILO: Se creo el hilo de id %d \n", state->id);
    fflush(stdout);

    while (state->is_alive == 1)
    {
        if ((state -> in_movement) == 1)
        {
            int i;
            int num_steps = state -> steps;

            while (num_steps > 0 && state->in_movement && state->is_alive)
            {
                /* Avisar al padre de este hilo que la pieza se debe mover */
                printf("HILO: se esta moviendo la pieza \n");
                fflush(stdout);
                sleep(1);

                RequestPiece * new_request = calloc(1, sizeof(RequestPiece));
                /* Verificamos que calloc se haya ejecutado correctamente */
                if (new_request == NULL) {
                    printf("Error al reservar memoria para la solicitud. ");
                    printf("pthread_piece(), pthread_pieces_logic.c \n");
                    exit(1);
                }

                /* La accion es movimiento */
                new_request->action = 0;
                new_request->id_piece = state -> id;

                /* Activamos nuestro semaforo para evitar que otro hilo o el 
                   proceso padre lo modifique al mismo tiempo*/
                pthread_mutex_lock(state -> sem);
                /* Agregamos al final de la cola de solicitudes nuestra solicitud de mov*/
                enqueue(state -> request_queue, new_request);
                pthread_mutex_unlock(state -> sem);
                num_steps--;
            }

            /* Actualizar el estado para dejar de moverse */
            state -> in_movement = 0;
            
        }
        else
        {
            sleep(state -> patience);

            /* Si el hilo llego a esta linea, sigue vivo y no tiene solicitud de moverse
               significa que el hilo perdio su paciencia
            */
            if (state -> is_alive == 1 && state -> in_movement == 0)
            {
                /* Ponemos la paciencia en 0 para que el hilo 
                   no se vaya a dormir hasta que la muevan
                */
                state -> patience = 0;

                /* Creamos una solicitud*/
                RequestPiece * new_request = calloc(1, sizeof(RequestPiece));
                /* Verificamos que calloc se haya ejecutado correctamente */
                if (new_request == NULL) {
                    printf("Error al reservar memoria para la solicitud. ");
                    printf("pthread_piece(), pthread_pieces_logic.c \n");
                    exit(1);
                }

                /* La accion es pedir ser usada */
                new_request->action = 1;
                new_request->id_piece = state -> id;

                /* Activamos nuestro semaforo para evitar que otro hilo o el 
                   proceso padre lo modifique al mismo tiempo*/
                pthread_mutex_lock(state -> sem);
                /* Agregamos ala solicitud */
                enqueue(state -> request_queue, new_request);
                pthread_mutex_unlock(state -> sem);

                /* Esperamos dos segundos a para volver a pedir ser usada*/
                sleep(2);

                /* Si hubo una interrupcion revisamos si ya nos movemos*/
                if (state -> in_movement != 0)
                    break;
                
                /*  Si no nos movemos, entonces pedimos de nuevo 
                    Tenemos que crear una nueva solicitud porque la anterior la libero 
                    el proceso principal
                */
                new_request = calloc(1, sizeof(RequestPiece));
                /* Verificamos que calloc se haya ejecutado correctamente */
                if (new_request == NULL) {
                    printf("Error al reservar memoria para la solicitud. ");
                    printf("pthread_piece(), pthread_pieces_logic.c \n");
                    exit(1);
                }
                new_request->action = 1;
                new_request->id_piece = state -> id;

                pthread_mutex_lock(state -> sem);
                enqueue(state -> request_queue, new_request);
                pthread_mutex_unlock(state -> sem);

                /* Volvemos a dormir para dar tiempo */
                sleep(2);

                /* Si hubo una interrupcion revisamos si ya nos movemos*/
                if (state -> in_movement != 0)
                    break;

                /* Hacemos nuestra ultima solicitud, 
                   Como no se movio la pieza, se procede a pedirle al proceso principal 
                   que el se encargue de moverla 
                */
                new_request = calloc(1, sizeof(RequestPiece));
                /* Verificamos que calloc se haya ejecutado correctamente */
                if (new_request == NULL) {
                    printf("Error al reservar memoria para la solicitud. ");
                    printf("pthread_piece(), pthread_pieces_logic.c \n");
                    exit(1);
                }
                /* Accion: pedir que el proceso principal del juego mueva la pieza*/
                new_request->action = 2;
                new_request->id_piece = state -> id;
                pthread_mutex_lock(state -> sem);
                enqueue(state -> request_queue, new_request);
                pthread_mutex_unlock(state -> sem);

                    


            }
        }
    }

    printf("HILO %d: he muerto \n", state->id);
}