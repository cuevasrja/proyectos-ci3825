# include "child_process_logic.h"

int can_read_from_pipe(int fd_c){
    struct pollfd fds = { .fd = fd_c, .events = POLLIN };
    int res = poll(&fds, 1, 0);

    //if res < 0 then an error occurred with poll
    //POLLERR is set for some other errors
    //POLLNVAL is set if the pipe is closed
    if(res < 0||fds.revents&(POLLERR|POLLNVAL))
    {
        //an error occurred, check errno
        perror("error en el pipe");
    }
    return fds.revents&POLLIN;
}

void * pthread_input_control_child(void * struct_info){

    InputUtils * info = (InputUtils *)struct_info;
    RequestFather rqt_father;   

    while (1 == 1)
    {
        if (can_read_from_pipe(info->read_fd) == 1){

            if (read(info->read_fd, &rqt_father, sizeof(RequestFather)) != 0)
            {
                printf("Request del padre obtenido!\n");
                fflush(stdout);

                RequestFather * new_rqt_father = calloc(1, sizeof(RequestFather));
                new_rqt_father->action = rqt_father.action;
                new_rqt_father->id = rqt_father.id;
                /* Si la accion es actualizar la paciencia ent se hace la copia */
                if (rqt_father.action == 4)
                {
                    int i;
                    for (i = 0; i < 8; i++)
                    {
                        new_rqt_father->patience_act[i] = rqt_father.patience_act[i];
                    }
                }
                
                pthread_mutex_lock(info->sem);
                enqueue(info->to_do, new_rqt_father);
                pthread_mutex_unlock(info->sem);
            }        
        }
    }
}

/* Misma funcion pero para el proceso padre.*/
void * pthread_input_control_father(void * struct_info){

    InputUtils * info = (InputUtils *)struct_info;
    RequestPiece rqt_child;   

    while (1 == 1)
    {
        if (can_read_from_pipe(info->read_fd) == 1){

            read(info->read_fd, &rqt_child, sizeof(RequestPiece));

            printf("Request del hijo obtenido!\n");
            fflush(stdout);

            RequestPiece * new_rqt_child = calloc(1, sizeof(RequestPiece));
            new_rqt_child->action = rqt_child.action;
            new_rqt_child->id_piece = rqt_child.id_piece;

            pthread_mutex_lock(info->sem);
            enqueue(info->to_do, new_rqt_child);
            pthread_mutex_unlock(info->sem);
        }
    }
}

void usr_child_code(int child_request[2], int father_request[2] ){

    int num_pieces = 8;
    int i, rslt_cr_p;

    /* La cola que vamos a usar para encolar las solicitudes de los hilos*/
    Queue request_queue = new_queue();

    /* Semaforo mutex para la cola de solicitudes del hijo*/
    pthread_mutex_t sem_request_piece;
    pthread_mutex_init(&sem_request_piece, NULL);

    /* Inicializacion de info de control de los hilos */
    PPthreadsControl p_pthreads_control = {pieces_in_movement: {2, -1}};

    /* Inicializacion de los strucs de estados de los hilos */
    pPthreadState p_pthreads_states[num_pieces];

    /* INicializacion del struct del rey */
    p_pthreads_states[0].id = 0;
    p_pthreads_states[0].patience = 1000; //
    p_pthreads_states[0].in_movement = 0;
    p_pthreads_states[0].steps = 5;
    p_pthreads_states[0].is_alive = 1;
    p_pthreads_states[0].sem = &sem_request_piece;
    p_pthreads_states[0].request_queue = &request_queue;
    p_pthreads_states[0].main_pthreads_control = &p_pthreads_control;

    /* Inicialiazcion de los structs de los caballos */
    for (i = 1; i < num_pieces; i++)
    {
        p_pthreads_states[i].id = i + 1;
        p_pthreads_states[i].patience = 100; ///
        p_pthreads_states[i].in_movement = 0;
        p_pthreads_states[i].steps = 3;
        p_pthreads_states[i].is_alive = 1;
        p_pthreads_states[i].sem = &sem_request_piece;
        p_pthreads_states[i].request_queue = &request_queue;
        p_pthreads_states[i].main_pthreads_control = &p_pthreads_control;

    }
 
    /* Inicializacion de los hilos */
    pthread_t p_pthreads_t[num_pieces];

    for ( i = 0; i < num_pieces; i++)
    {
        rslt_cr_p =  pthread_create(&p_pthreads_t[i],NULL, pthread_piece, (void *)&p_pthreads_states[i]);
        if (rslt_cr_p != 0)
        {
            perror("Ocurrio un error al crear el hilo\n");
            exit(1);
        }
        
    }

    sleep(5);

    /* Creacion del hilo que controla el input. 
    Este hilo se usa para que el proceso principal no se bloquee */
    pthread_t pthread_input;
    pthread_mutex_t sem_to_do;
    Queue queue_to_do = new_queue();
    pthread_mutex_init(&sem_to_do, NULL);
    InputUtils info_input_control;
    info_input_control.read_fd = father_request[0];
    info_input_control.sem = &sem_to_do;
    info_input_control.to_do = &queue_to_do;

    rslt_cr_p = pthread_create(&pthread_input, NULL, pthread_input_control_child, (void *)&info_input_control);

    RequestFather rqt_father;
    while (1 == 1)
    {
        printf("HIJO: en el while \n");
        fflush(stdout);

        if (queue_to_do.length > 0)
        {
            RequestFather * r = (RequestFather *)peek(&queue_to_do);
            printf("HIJO: act: %d, id: %d \n", r->action, r->id);
            fflush(stdout);
            
            switch (r->action)
            {

            case 0:
                printf("Se muere la pieza: %d \n", r->id);
                fflush(stdout);

                /* Si la pieza ya estaba muerta no se hace nada */
                if (p_pthreads_states[r->id].is_alive == 0)
                {
                    break;
                }
                
                /* Hacemos que el hilo salga de su loop */
                p_pthreads_states[r->id].is_alive = 0;
                p_pthreads_states[r->id].patience = 0;
                p_pthreads_states[r->id].in_movement = 0;
                pthread_kill(p_pthreads_t[r->id], SIGUSR1);
                pthread_join(p_pthreads_t[r->id], NULL); 

                // /* Hay que avisar que se elimino el hilo? */

                break;
            
            case 1:
                printf("La pieza debe dejar de moverse: %d \n", r->id);
                fflush(stdout);

                /* Si la pieza ya estaba muerta no se hace nada */
                if (p_pthreads_states[r->id].is_alive == 0)
                {
                    break;
                }

                /* Asumiendo que el padre manda esta action porque sabe que la pieza se movia */
                p_pthreads_states[r->id].in_movement = 0;
                pthread_kill(p_pthreads_t[r->id], SIGUSR1);

                break;
    
            /* Se mueve una pieza */
            case 2:
                printf("Se mueve la pieza: %d \n", r->id);
                fflush(stdout);

                /* Si la pieza ya estaba muerta no se hace nada */
                if (p_pthreads_states[r->id].is_alive == 0)
                {
                    break;
                }

                p_pthreads_states[r->id].in_movement = 1;
                pthread_kill(p_pthreads_t[r->id], SIGUSR1);
                
                break;
            
            case 3:
                printf("El arreglo de paciencia es el sig: \n");
                fflush(stdout);
                int j;
                for (j = 0; j < 8; j++)
                {
                    printf("j: %d; p: %d \n", j, r->patience_act[j]);
                    /* Dudoso */
                    p_pthreads_states[j].patience = r->patience_act[j];
                    fflush(stdout);
                }
                
                break;
            
            default:
                printf("No se entendio la accion \n");
                break;
            }

            /* Hay que liberar la memoria del struct en el heap
               y eliminar el nodo ya revisado */
            dequeue(&queue_to_do);
            free(r);
        }
 
        sleep(1);

        pthread_mutex_lock(&sem_request_piece);
        if (request_queue.length > 0)
        {
            RequestPiece * r = (RequestPiece *)peek(&request_queue);

            // printf("HIJO: lo que enviare al padre_ act: %d, id: %d \n", r->action, r->id_piece);
            // fflush(stdout);

            write(child_request[1], r, sizeof(RequestPiece));
            dequeue(&request_queue);
            free(r);
            
        }
        pthread_mutex_unlock(&sem_request_piece);
    }    
    
    return;
}

