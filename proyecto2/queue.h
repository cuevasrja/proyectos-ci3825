# define TRUE (1 == 1)
# define FALSE (0 == 1)

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

/* Constructor */
Queue * new_queue();

/*
Función que encola un objeto en la cola
@param queue Cola que se va a modificar
@param value Valor a agregar
*/
void enqueue(Queue * queue, void * value);

/*
Función que retorna el primer objeto de la cola
@param queue Cola de la que se va a obtener el primer objeto
@return Primer objeto de la cola
*/
void * peek( Queue * queue);

/*
Función que elimina el primer objeto de la cola
@param queue Cola de la que se va a eliminar el primer objeto
*/
void dequeue( Queue * queue);

/*
Función que libera la memoria de los nodos de la cola
@param node Nodo que se va a liberar
*/
void free_nodes( Node * node);

/*
Función que busca un elemento en especifico de la cola
@param queue Cola en la que se va a buscar el elemento
@param  x el elemento a buscar
*/
int search(Queue * queue, char * x);

/*
Función que imprime los elementos de la cola
@param queue Cola que se va a imprimir
*/
void print_queue(Queue * queue);

#endif