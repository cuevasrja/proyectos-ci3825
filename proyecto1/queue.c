# include <stdio.h>
# include <stdlib.h>
# include "queue.h"

/* Constructor */
Queue new_queue(){

    Queue new_queue;
    new_queue.head = NULL;
    new_queue.tail = NULL;
    new_queue.length = 0;

    return new_queue;
}

/*
Función que encola un objeto en la cola
@param queue Cola que se va a modificar
@param value Valor a agregar
*/
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

/*
Función que retorna el primer objeto de la cola
@param queue Cola de la que se va a obtener el primer objeto
@return Primer objeto de la cola
*/
void * peek( Queue * queue){
    if (queue->head != NULL)
    {
        return queue -> head -> value;
    }
    
}

/*
Función que elimina el primer objeto de la cola
@param queue Cola de la que se va a eliminar el primer objeto
*/
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

/*
Función que libera la memoria de los nodos de la cola
@param node Nodo que se va a liberar
*/
void free_nodes(struct Node * node)
{
    if (node->next != NULL)
        free_nodes(node->next);
        
    free(node);
}

