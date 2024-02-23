# include <stdio.h>
# include <stdlib.h>
# include <ctype.h>
# include <unistd.h>
# include <pthread.h>
# include <signal.h>
# include <wait.h>
# include <poll.h>

# include "queue.h"
# include "child_process_logic.h"
# include "pthread_pieces_logic.h"

# define TRUE 1 == 1
# define FALSE !TRUE

int main() {

    int child_request_fd[2];
    int father_request_fd[2];
    pipe(child_request_fd);
    pipe(father_request_fd);

    pid_t id_process = fork();

    if (id_process == 0)
    {
        // el hijo escribe por child_request
        close(child_request_fd[0]);
        dup(child_request_fd[1]);

        // el hijo lee por father_request
        close(father_request_fd[1]);
        dup(father_request_fd[0]);

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

    /* En esta cola van las solicitudes del hijo al padre*/
    Queue to_do_queue_father = new_queue();

    /* Una variable temporal solo para enviar una vez las soliciutdes de prueba*/
    int send_request = 1;

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
    

    while (TRUE)
    {
        if (send_request == 1)
        {
            /* Dandole indicaciones al hijo */
            printf("PADRE: Creando solicitudes en la cola\n");
            fflush(stdout);

            /* Pedir que se mueva */
            RequestFather new_request0;
            new_request0.action = 2;
            new_request0.id = 0;
            for (size_t i = 0; i < 8; i++)
            {
                new_request0.patience_act[i] = i;
            }
            write(father_request_fd[1], &new_request0, sizeof(RequestFather));

            // /* Pedir que NO se mueva */
            // RequestFather new_request1;
            // new_request1.action = 1;
            // new_request1.id = 0;
            // for (size_t i = 0; i < 8; i++)
            // {
            //     new_request1.patience_act[i] = i;
            // }

            // write(father_request_fd[1], &new_request1, sizeof(RequestFather));

            // /* Pedir que se muera un hilo */
            // RequestFather new_request3;
            // new_request3.action = 0;
            // new_request3.id = 0;
            // for (size_t i = 0; i < 8; i++)
            // {
            //     new_request3.patience_act[i] = i;
            // }
            // write(father_request_fd[1], &new_request3, sizeof(RequestFather));

            send_request = 0;
        }
        
        /* vamos a esperar un poco para revisar las solicitudes del hijo*/
        /* Esto es solo para hacer las pruebas*/
        // sleep(5);

        pthread_mutex_lock(&sem_to_do_father);
        if (to_do_queue_father.length > 0)
        {
            RequestPiece * r = (RequestPiece *)peek(&to_do_queue_father);

            printf("PADRE: lo que recibi de mi hijo: %d, id: %d \n", r->action, r->id_piece);
            fflush(stdout);

            dequeue(&to_do_queue_father);
            free(r);
            
        }
        pthread_mutex_unlock(&sem_to_do_father);


    }
    

    

    sleep(50);

                    
    kill(id_process, SIGKILL);
    
    return 0;
}