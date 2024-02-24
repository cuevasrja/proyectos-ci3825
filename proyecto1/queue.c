# include <stdio.h>
# include <stdlib.h>
# include "queue.h"

Queue new_queue(){

    Queue new_queue;
    new_queue.head = NULL;
    new_queue.tail = NULL;
    new_queue.length = 0;

    return new_queue;
}

void enqueue(Queue * queue, void * value){

    Node * new_node = ( Node *)malloc(sizeof(Node));
    /* Verificamos que malloc se ejecuta correctamente */
    if (new_node == NULL) {
        printf("Error al reservar memoria para el nodo. ");
        printf("enqueue(), queue.c \n");
        exit(1);
    }
    Node * head_node = queue -> head;
    new_node->value = value;
    new_node->next = NULL;

    /*Actualizamos la variable que lleva el numero de nodos*/
    queue->length += 1;

    if (head_node == NULL)
    {
        queue -> head = new_node;
        queue -> tail = new_node;
        return;
    }

    Node * tail_node = queue -> tail;
    tail_node -> next = new_node;
    queue -> tail = new_node;

}

void * peek( Queue * queue){
    if (queue->head != NULL)
    {
        return queue -> head -> value;
    }
    
}

void dequeue(Queue * queue){
    if (queue->head == NULL)
    {
        return;
    }

    Node * ref_head = queue->head;
    queue->head = queue->head->next;
    free(ref_head);
    queue->length -= 1; 
    
}

void free_nodes(struct Node * node)
{
    if (node->next != NULL)
        free_nodes(node->next);
        
    free(node);
}

