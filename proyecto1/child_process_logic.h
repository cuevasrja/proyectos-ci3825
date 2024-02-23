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

void * pthread_input_control_child(void * struct_info);

void * pthread_input_control_father(void * struct_info);

void usr_child_code(int child_request[2], int father_request[2]);



#endif