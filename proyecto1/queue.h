#ifndef QUEUE
#define QUEUE

typedef struct Node
{
    void * value;
    struct Node * next;
    
} Node;  

typedef struct queue
{
    /* Apuntador al primer elemento de la cola */
    struct Node * head;
     /* Apuntador al ultimo elemento de la cola */
    struct Node * tail;
    /* Cantidad de elementos de la cola */
    int length;
} Queue;


Queue new_queue();

void enqueue(Queue * queue, void * value);

void * peek( Queue * queue);

void dequeue( Queue * queue);

void free_nodes( Node * node);

#endif