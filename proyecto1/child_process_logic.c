# include "child_process_logic.h"

void * pthread_input_control_child(void * struct_info){

    InputUtils * info = (InputUtils *)struct_info;
    RequestFather rqt_father;   

    while (1 == 1)
    {
        if (read(info->read_fd, &rqt_father, sizeof(RequestFather)) != 0)
        {
                RequestFather * new_rqt_father = calloc(1, sizeof(RequestFather));
                /* Verificamos que calloc se ejcute correctamente */
                if (new_rqt_father == NULL) {
                    printf("Error al reservar memoria para la solicitud. ");
                    printf("pthread_input_control_child(), child_process_logic.c \n");
                    exit(1);
                }
                new_rqt_father->action = rqt_father.action;
                new_rqt_father->id = rqt_father.id;
                /* Si la accion es actualizar la paciencia ent se hace la copia */
                if (rqt_father.action == 3)
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

void * pthread_input_control_father(void * struct_info){

    InputUtils * info = (InputUtils *)struct_info;
    RequestPiece rqt_child;   

    while (1 == 1)
    {
        if (read(info->read_fd, &rqt_child, sizeof(RequestPiece)) != 0)
        {

            RequestPiece * new_rqt_child = calloc(1, sizeof(RequestPiece));
            /* Verificamos que calloc se ejcute correctamente */
            if (new_rqt_child == NULL) {
                printf("Error al reservar memoria para la solicitud. ");
                printf("pthread_input_control_father(), father_process_logic.c \n");
                exit(1);
            }
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

    /* Inicializacion de los strucs de estados de los hilos */
    pPthreadState p_pthreads_states[num_pieces];

    /* INicializacion del struct del rey */
    p_pthreads_states[0].id = 0;
    p_pthreads_states[0].patience = 1000; 
    p_pthreads_states[0].in_movement = 0;
    p_pthreads_states[0].steps = 5;
    p_pthreads_states[0].is_alive = 1;
    p_pthreads_states[0].sem = &sem_request_piece;
    p_pthreads_states[0].request_queue = &request_queue;

    /* Inicialiazcion de los structs de los caballos */
    for (i = 1; i < num_pieces; i++)
    {
        p_pthreads_states[i].id = i;
        p_pthreads_states[i].patience = 1000; 
        p_pthreads_states[i].in_movement = 0;
        p_pthreads_states[i].steps = 15;
        p_pthreads_states[i].is_alive = 1;
        p_pthreads_states[i].sem = &sem_request_piece;
        p_pthreads_states[i].request_queue = &request_queue;

    }
 
    /* Inicializacion de los hilos */
    pthread_t p_pthreads_t[num_pieces];

    for ( i = 0; i < num_pieces; i++)
    {
        rslt_cr_p =  pthread_create(&p_pthreads_t[i],NULL, pthread_piece, (void *)&p_pthreads_states[i]);
        if (rslt_cr_p != 0)
        {
            perror("Ocurrio un error al crear el hilo \n");
            exit(1);
        }
        
    }

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

        pthread_mutex_lock(&sem_to_do);
        if (queue_to_do.length > 0)
        {
            RequestFather * r = (RequestFather *)peek(&queue_to_do);
            
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
                    // printf("j: %d; p: %d \n", j, r->patience_act[j]);
                    p_pthreads_states[j].patience = r->patience_act[j];
                    fflush(stdout);
                }
                
                break;
            
            default:
                printf("HIJO: No se entendio la accion \n");
                break;
            }

            /* Hay que liberar la memoria del struct en el heap
               y eliminar el nodo ya revisado */
            dequeue(&queue_to_do);
            free(r);
        }
        pthread_mutex_unlock(&sem_to_do);

        pthread_mutex_lock(&sem_request_piece);
        if (request_queue.length > 0)
        {
            RequestPiece * r = (RequestPiece *)peek(&request_queue);

            write(child_request[1], r, sizeof(RequestPiece));
            dequeue(&request_queue);
            free(r);
            
        }
        pthread_mutex_unlock(&sem_request_piece);
    }    
    
    return;
}

void IA_child_code(int child_request[2], int father_request[2] ){

    int num_pieces = 8;
    int i, rslt_cr_p;

    /* La cola que vamos a usar para encolar las solicitudes de los hilos*/
    Queue request_queue = new_queue();

    /* Semaforo mutex para la cola de solicitudes del hijo*/
    pthread_mutex_t sem_request_piece;
    pthread_mutex_init(&sem_request_piece, NULL);

    /* Inicializacion de los strucs de estados de los hilos */
    pPthreadState p_pthreads_states[num_pieces];

    /* INicializacion del struct del rey negro*/
    p_pthreads_states[8].id = 0;
    p_pthreads_states[8].patience = 1000; 
    p_pthreads_states[8].in_movement = 0;
    p_pthreads_states[8].steps = 5;
    p_pthreads_states[8].is_alive = 1;
    p_pthreads_states[8].sem = &sem_request_piece;
    p_pthreads_states[8].request_queue = &request_queue;

    /* Inicialiazcion de los structs de los caballos */
    for (i = 9; i < num_pieces + 8; i++)
    {
        p_pthreads_states[i].id = i;
        p_pthreads_states[i].patience = 1000; 
        p_pthreads_states[i].in_movement = 0;
        p_pthreads_states[i].steps = 15;
        p_pthreads_states[i].is_alive = 1;
        p_pthreads_states[i].sem = &sem_request_piece;
        p_pthreads_states[i].request_queue = &request_queue;
    }
 
    /* Inicializacion de los hilos */
    pthread_t p_pthreads_t[num_pieces];

    for ( i = 0; i < num_pieces; i++)
    {
        rslt_cr_p =  pthread_create(&p_pthreads_t[i],NULL, pthread_piece, (void *)&p_pthreads_states[i]);
        if (rslt_cr_p != 0)
        {
            perror("Ocurrio un error al crear el hilo \n");
            exit(1);
        }
        
    }

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

        pthread_mutex_lock(&sem_to_do);
        if (queue_to_do.length > 0)
        {
            RequestFather * r = (RequestFather *)peek(&queue_to_do);
            
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
                    // printf("j: %d; p: %d \n", j, r->patience_act[j]);
                    p_pthreads_states[j].patience = r->patience_act[j];
                    fflush(stdout);
                }
                
                break;
            
            default:
                printf("HIJO: No se entendio la accion \n");
                break;
            }

            /* Hay que liberar la memoria del struct en el heap
               y eliminar el nodo ya revisado */
            dequeue(&queue_to_do);
            free(r);
        }
        pthread_mutex_unlock(&sem_to_do);

        pthread_mutex_lock(&sem_request_piece);
        if (request_queue.length > 0)
        {
            RequestPiece * r = (RequestPiece *)peek(&request_queue);

            write(child_request[1], r, sizeof(RequestPiece));
            dequeue(&request_queue);
            free(r);
            
        }
        pthread_mutex_unlock(&sem_request_piece);
    }    
    
    return;
}

int handleChildRequests(Board * board, pthread_mutex_t * sem_to_do_father, Queue * to_do_queue_father){

    RequestPiece  new_r;
    int num_request = 0;

    pthread_mutex_lock(sem_to_do_father);
    num_request = to_do_queue_father->length;
    pthread_mutex_unlock(sem_to_do_father);

    while (num_request > 0)
    {
        pthread_mutex_lock(sem_to_do_father);

        RequestPiece * r = (RequestPiece *)peek(to_do_queue_father);
        new_r.action = r->action;
        new_r.id_piece = r->id_piece;
        
        dequeue(to_do_queue_father);
        free(r);

        pthread_mutex_unlock(sem_to_do_father);

        /* Tomamos la accion que corresponde a la solicitud */
        switch (new_r.action)
        {
            /*Una pieza solicita moverse un caracter*/
        case 0:
            int result;
            result = move_char_piece(board, new_r.id_piece);
            if (result == -1)
            {
                perror("ha ourrido algo al mover la pieza");
                return -1;
            }
            if (result == 0)
            {
                /* Hay que pedirle a la pieza que espere un segundo mas */
            }
            if (result == 1)
            {
                printBoard(board);
            }
            if (board->pieces[new_r.id_piece].moves_queue.length == 0)
            {
                printf("Se comenzaran a contar 20 segundos antes de pasarle el turno a la IA!\n");
            }
              
            break;
        
        case 1:
            /* La pieza nos avisa que ha perdido la paciencia */
            Piece * piece =  &board->pieces[new_r.id_piece];
            int bf = piece->cell_row;
            int bc = piece->cell_col;

            /*Aqui se deberia revisar si la pieza se puede comer a alguien o si solo quiere
            ser usada*/
            printf("La pieza de la celda (fila: %d, col: %d) ha perdido la paciencia", bf, bc);
            break;
        case 2:
            /* La pieza reclamo dos veces haber perdido la paciencia y ahora
            le pide al proceso principal que la mueva */
            /*Hay que calcular si hay un enemigo cerca para comerse. De ser el caso 
            movemos la pieza a esa celda*/

            /* Si no habia un enemigo para comerse, se escoge al azar un movimiento */

            printf("La pieza de la celda (fila: %d, col: %d) ha comenzado a moverse sola", bf, bc);
            break;
        default:
            printf("No se reconocio la action pedida por la pieza\n");
            break;
        }
        num_request--;
    }

}

void send_request_to_child(Board * board, int father_request_fd, int action, int piece_id){

    RequestFather new_request;
    new_request.action = action;
    new_request.id = piece_id;

    if (action == 3)
    {
        int i;
        int x = 0;
        /////////////
        if (board->turn == IA)
            x = 8;
        
        for(i = 0; i < 8; i++){
            Piece * piece_act = &board->pieces[i + x];
            new_request.patience_act[i] = piece_act->patience;
        }
    }
    
    write(father_request_fd, &new_request, sizeof(RequestFather));
}