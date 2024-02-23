# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <ctype.h>
# include "board.h"

# include "menu.h"
# include "queue.h"
# include "child_process_logic.h"
# include "pthread_pieces_logic.h"

/*-1 si da error*/
int handleChildRequests(Board * board, pthread_mutex_t * sem_to_do_father, Queue * to_do_queue_father){

    RequestPiece  new_r;
    int num_request = 0;

    pthread_mutex_lock(sem_to_do_father);
    num_request = to_do_queue_father->length;
    pthread_mutex_unlock(sem_to_do_father);

    while (num_request > 0)
    {
        pthread_mutex_lock(sem_to_do_father);

        printf("\nPADRE: tama 1 de la cola de to_do %d \n", to_do_queue_father->length);

        RequestPiece * r = (RequestPiece *)peek(to_do_queue_father);

        printf("PADRE: lo que recibi de mi hijo: %d, id: %d \n", r->action, r->id_piece);
        fflush(stdout);

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
            printf("\nDentro del caso 0. Pieza id: %d \n", new_r.id_piece);
            printf("Tama de la cola de mov de la pieza: %d \n", board->pieces[new_r.id_piece].moves_queue.length);
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
            
            
            ////////
            break;
        
        case 1:
            /* La pieza nos avisa que ha perdido la paciencia */
            printf("\nDentro del caso 1. Pieza id: %d \n", new_r.id_piece);
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
            printf("\nDentro del caso 2. Pieza id: %d \n", new_r.id_piece);

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


int main() {

    /* Se crea el tablero */
    Board board = newBoard();

    /* Creamos los pipes para comunicarnos con el proceso usuario */
    int child_request_fd[2];
    int father_request_fd[2];
    pipe(child_request_fd);
    pipe(father_request_fd);

    /* creamos el proceso hijo que representa al usuario */
    pid_t id_process_user = fork();
    if (id_process_user == 0)
    {
        // el hijo escribe por child_request
        close(child_request_fd[0]);
        dup(child_request_fd[1]);

        // el hijo lee por father_request
        close(father_request_fd[1]);
        dup(father_request_fd[0]);

        /* Todo el codigo del proceso hijo user esta en esta funcion*/
        usr_child_code(child_request_fd, father_request_fd);

        return 0;
    }
    
    /* De aqui para abajo solo hay codigo del padre */

    /* el padre solo lee de child request */
    close(child_request_fd[1]);
    dup(child_request_fd[0]);

    /* el padre solo escribe en father_request */
    close(father_request_fd[0]);
    dup(father_request_fd[1]);

    /* La primera solicitud que le haremos a los procesos sera 
    actualizar la paciencia de sus piezas*/
    send_request_to_child(&board, father_request_fd[1], 3, -1);    

    /* En esta cola van las solicitudes del proceso user hijo al padre*/
    Queue to_do_queue_father = new_queue();

    /* Creamos el hilo del padre que lee*/
    pthread_t pthread_input;
    pthread_mutex_t sem_to_do_father;
    pthread_mutex_init(&sem_to_do_father, NULL);

    /* Informacion para que el hilo pueda comunicar lo que obtenga*/
    InputUtils info_input_control;
    info_input_control.read_fd = child_request_fd[0];
    info_input_control.sem = &sem_to_do_father;
    info_input_control.to_do = &to_do_queue_father;
    int rslt_cr_p;

    rslt_cr_p = pthread_create(&pthread_input, NULL, pthread_input_control_father, (void *)&info_input_control);
    if (rslt_cr_p != 0)
    {
        perror("No se logro crear el hilo que controla el input del padre");
        exit(1);
    }


    /* options es un arreglo de caracteres que guarda el input del usuario */
    char options[INPUT];
    int j;
    for (j = 0; j < INPUT; j++) {
        options[j] = '\0';
    }

    /* cursor_position es un arreglo de enteros que indica en que posicion 
       debe estar el cursor.
       La primera posicion corresponde al eje horizontal, si es un entero negativo 
       significa que el cursor debe ir a la izq desde donde esta y si es positivo a la derecha
       La segunda posicion es el eje vertical. Si es un int positivo, el cursor baja, de lo
       contrario sube.
    */
   int cursor_position[2] = {0, 0};
   /* Nos indica si se selecciono la casilla de forma que revisamos si es valida la opcion */
   int check_selection = 0;
   /* si la seleccion es valida ponemos en un arreglo la fila y columna de la celda*/
   int valid_piece_cell[2] = {-1, -1};
   int check_play = 0;
   int result_to_do;

    /* Mientras que no haya un ganador */
    while (!isWinner(&board)) {
        printBoard(&board);
        showMenu();
        enterOptions(options);
        if (willExit(options, id_process_user)){ /* Si el usuario quiere salir, salimos */
            printf("Saliendo...\n");
            break;
        }
        int i = 0;
        /* Mientras que el caracter no sea nulo y no se haya llegado al final del arreglo */
        while(options[i] != '\0' && i < INPUT) {
            switch (tolower(options[i])) {
                case 'w': /* Arriba */
                    printf("Mover arriba\n");
                    cursor_position[1] -=1;
                    break;
                case 'a': /* Izquierda */
                    printf("Mover izquierda\n");
                    cursor_position[0] -=1;
                    break;
                case 's': /* Abajo */
                    printf("Mover abajo\n");
                    cursor_position[1] +=1;
                    break;
                case 'd': /* Derecha */
                    printf("Mover derecha\n");
                    cursor_position[0] +=1;
                    break;
                case 'x': /* Seleccionar pieza */
                    /* Si llegamos hasta aca, significa que ya tenemos 
                    la futura casilla del cursor en cursor position.
                    volvemos i == INPUT para salir del while */
                    
                    /*Si el cursor habia seleccionado una casilla valida, 
                     entonces toca revisar si la nueva accion es una jugada valida*/
                    if (board.cursor.is_cell_valid == 1){
                        check_play = 1;
                        printf("Seleccionar casilla de destino\n");

                    } 
                    else
                    {
                        /* Si el cursor estaba en +, procedemos a ver si es una seleccion
                        correcta */
                        check_selection = 1;
                        printf("Seleccionar pieza\n");
                    }
                    
                    i = INPUT;

                    break;
                case 'q': // Exit
                    printf("Salir\n");
                    willExit(options, id_process_user);
                    break;
                default: 
                    printf("Opción inválida\n");
                    break;
            }
            i++;
        }

        /* Vamos a mover el cursor */
        move_cursor(&board, cursor_position);
        // /* Limpiamos el arreglo */
        cursor_position[0] = 0;
        cursor_position[1] = 0;
        /* Revisamos si hay que chequear una seleccion*/
        if (check_selection == 1)
        {
            is_selection_valid(&board);
            printf("Chequeando la casilla de seleccion: F%d C%d", board.cursor.board_row, board.cursor.board_col );

            /**/
            valid_piece_cell[0] = board.cursor.board_row;
            valid_piece_cell[1] = board.cursor.board_col;
            check_selection = 0;
        }
        /* Si el cursor esta en un seleccion valida, revisamos si hay que chequear una jugada*/
        if (check_play == 1)
        {
            int is_valid = is_play_valid(&board, valid_piece_cell);
            printf("Despues del is_play_valid: tam de la cola de id 0 %d \n", board.pieces[0].moves_queue.length);
            if (is_valid == 1)
            {
                printf("Se selecciono una jugada valida!\n");
                printf("Se enviara al proceso!\n");

                int id_piece = board.cells[valid_piece_cell[0]][valid_piece_cell[1]].owner;
                
                board.cells[valid_piece_cell[0]][valid_piece_cell[1]].owner = -1;
                /* El padre le escribe la solicitud de movimiento al proceso usuario*/
                send_request_to_child(&board, father_request_fd[1], 2, id_piece);
                
                /* Actualizamos el arreglo de piezas moviendose*/
                board.pieces_in_mov[id_piece] = 1;

            }
            else
            {
                printf("Jugada invalida!!\n");
                printf("NO se enviara al proceso!\n");
            }
            
            
            board.cursor.is_cell_valid = 0;
            check_play = 0;
        }

        
        result_to_do = handleChildRequests(&board, &sem_to_do_father, &to_do_queue_father);
        if (result_to_do == -1)
        {
            willExit(options, id_process_user);
        }

        
    }
    
    /* Si hay un ganador, decimos quien es */
    if (isWinner(&board)) {
        printf("El ganador es: %s\n", board.winner == 0 ? "Usuario" : "IA");
    }
    return 0;
}