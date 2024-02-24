# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <ctype.h>
# include "board.h"

# include "menu.h"
# include "queue.h"
# include "child_process_logic.h"
# include "pthread_pieces_logic.h"

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

    /* Creamos los pipes para comunicarnos con el proceso IA */
    int IA_request_fd[2];
    int IA_father_request_fd[2];
    pipe(IA_request_fd);
    pipe(IA_father_request_fd);

    /* creamos el proceso hijo que representa a la IA */
    pid_t id_process_IA = fork();
    if (id_process_IA == 0)
    {
        // el hijo escribe por IA_request_fd
        close(IA_request_fd[0]);
        dup(IA_request_fd[1]);

        // el hijo lee por IA_father_request_fd
        close(IA_father_request_fd[1]);
        dup(IA_father_request_fd[0]);

        usr_child_code(IA_request_fd, IA_father_request_fd);

        return 0;
    }
    
    /* De aqui para abajo solo hay codigo del padre */

    /* el padre solo lee de child request */
    close(child_request_fd[1]);
    dup(child_request_fd[0]);

    /* el padre solo escribe en father_request */
    close(father_request_fd[0]);
    dup(father_request_fd[1]);

    /* el padre solo lee de IA_request_fd */
    close(IA_request_fd[1]);
    dup(IA_request_fd[0]);

    /* el padre solo escribe en father_request */
    close(IA_father_request_fd[0]);
    dup(IA_father_request_fd[1]);

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


    /* Ahora hacemos el hilo que pide el input del usuario */
    pthread_t pthread_input_menu;
    pthread_mutex_t sem_moves_cursor;
    pthread_mutex_init(&sem_moves_cursor, NULL);
    Queue moves_cursor_queue = new_queue();

    /* Hacemos el struct con su info*/
    InfoMenu info_menu;
    info_menu.game_running = 1;
    info_menu.ask_for_input = 1;
    info_menu.check_selection = 0;
    info_menu.check_play = 0;
    info_menu.cursor_in_valid_cell = 0;
    info_menu.sem = &sem_moves_cursor;
    info_menu.moves_cursor_queue = &moves_cursor_queue;
    info_menu.board = &board;
    info_menu.exit = 0; 

    /* Lo creamos y comprobamos que haya salido bien su creacion */
    rslt_cr_p = pthread_create(&pthread_input_menu, NULL, pthread_input_menu_control, (void *)&info_menu);
    if (rslt_cr_p != 0)
    {
        perror("No se logro crear el hilo que controla el input del menu\n");
        exit(1);
    }

   /* si la seleccion es valida ponemos en un arreglo la fila y columna de la celda*/
   int valid_piece_cell[2] = {-1, -1};
   int result_to_do;
   int start_time;
   int now;
   
    printBoard(&board);
    showMenu();
    printf("Ingrese una opción (Solo puede ingresar un máximo de %d acciones): ", INPUT);
    /* Mientras que no haya un ganador */
    while (!isWinner(&board) && info_menu.exit != 1) {

        if (board.move_request == 1)
        {
            now = time(NULL);
            if (now - start_time > 20 && board.turn == IA){
                board.turn = USER;
                board.move_request = 0;
                info_menu.ask_for_input = 1;
                printf("\nSe acabo el tiempo de la IA!\n");
                pthread_kill(pthread_input_menu, SIGUSR1);

            }
        }
        
        
        if (board.turn == USER)
        {      
        pthread_mutex_lock(&sem_moves_cursor);
        if (moves_cursor_queue.length > 0)
        {

            /* Vamos a mover el cursor */
            int * ref_cp = peek(&moves_cursor_queue);
            int cursor_pos[2] = {ref_cp[0], ref_cp[1]};
            move_cursor(&board, cursor_pos);
            dequeue(&moves_cursor_queue);
            free(ref_cp);
            printBoard(&board);
            showMenu();
            printf("Ingrese una opción (Solo puede ingresar un máximo de %d acciones): ", INPUT);
        }
        pthread_mutex_unlock(&sem_moves_cursor);

        /* Revisamos si hay que chequear una seleccion*/
        if (info_menu.check_selection == 1)
        {
            is_selection_valid(&board);

            /**/
            valid_piece_cell[0] = board.cursor.board_row;
            valid_piece_cell[1] = board.cursor.board_col;
            info_menu.check_selection = 0;
            info_menu.cursor_in_valid_cell = board.cursor.is_cell_valid;
        }

        /* Si el cursor escogio una pieza valida, revisamos si hay que chequear una jugada*/
        if (info_menu.check_play == 1)
        {
            int is_valid = is_play_valid(&board, valid_piece_cell);
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

                board.move_request = 1;
                board.turn = IA;
                info_menu.ask_for_input = 0;
                pthread_kill(pthread_input_menu, SIGUSR1);
                start_time = time(NULL);

            }
            else
            {
                printf("Jugada invalida!!\n");
                printf("NO se enviara al proceso!\n");
            }
            
            
            board.cursor.is_cell_valid = 0;
            info_menu.cursor_in_valid_cell = 0;
            info_menu.check_play = 0;
        }
        }

        
        result_to_do = handleChildRequests(&board, &sem_to_do_father, &to_do_queue_father);
        if (result_to_do == -1)
        {
            exit(1);
        }

        
    }
    
    /* Si hay un ganador, decimos quien es */
    if (isWinner(&board)) {
        printf("El ganador es: %s\n", board.winner == 0 ? "Usuario" : "IA");
    }

    kill(id_process_user, SIGKILL);
    kill(id_process_IA, SIGKILL);
    return 0;
}