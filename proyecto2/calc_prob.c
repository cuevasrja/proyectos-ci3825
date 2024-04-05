/*  Proyecto 2. Pide Cola
    Participantes:
        Sandibel Soares
        Carlo Herrera
        Juan Cuevas

    Archivo: calc_prob.c
    Contiene las funciones para:
        - Calcular la probabilidad de asistencia de un estudiante a 
        una materia
        - Calcular la cantidad de carros esperados en una materia y se 
        verifica si son suficientes para dar cola a los estudiantes que 
        lo necesiten    
*/

# include <stdio.h>   /* funcion printf */
# include <stdlib.h>  /* funcion free */
# include <math.h>    /* funcion ceil */
# include <string.h>  /* funcion strcpy */

# include "calc_prob.h"

/*  
    Calcula la probabilidad de asistencia de un estudiante a una materia 
    dado su carnet y una cola con los carnets de los estudiantes inscritos a 
    la materia
*/
float student_prob(Queue * students, char* carnet, float p, float q, int c){

    /*
    Inicializamos la probabilidad acumulada en 25 contando el 15% de llegar 
    en el bus de la universidad y el 10% de llegar por transporte publico
    */
    float prob = 25;
    int cohorte;

    char line[10];

    /*Leemos cada linea de estudiantes*/
    while(students->length != 0){

        /*Leemos la linea*/
        strcpy(line, peek(students));

        /*Extraemos la cohorte como entero*/
        cohorte = (line[0] - '0') * 10 + (line[1] - '0');
        
        /*
        Si es una cohorte posterior al 2000 sumamos 100 para mantener
        la distancia entre dos cohortes.
        */
        if(cohorte < 70 ) cohorte += 100;

        /*Sumamos la probabilidad del estudiante de tener carro segun su cohorte al acumulado.*/
        prob += p + ((c+100)-cohorte)*q;

        /* Liberamos el string y el nodo*/
        free(peek(students));
        dequeue(students);
    }

    /*Extraemos la cohorte del estudiante por el que se consulta como entero.*/
    cohorte = (carnet[0] - '0') * 10 + (carnet[1] - '0');

    /*
    Si el estudiante por el que se consulta es de una cohorte posterior al 2000, sumamos 100 
    para mantener la distancia entre dos cohortes.
    */
    if(cohorte < 70 ) cohorte += 100;

    /*
    Restamos la probabilidad del estudiante por el que se consulta de tener carro segun su 
    cohorte al acumulado.
    */
    prob -= p + ((c+100)-cohorte)*q;

    /*La probabilidad maxima es de 100%*/
    if(prob > 100) prob = 100;

    return prob;
}

/*
    Calcula la cantidad de carros esperados en una materia y se verificar 
    si son suficientes para dar cola a los estudiantes que lo necesiten
    Recibe una cola con los carnets de los estudiantes inscritos en la materia,
    el codigo de la matera, la probabilidad p de que los estudiantes de la ultima
    corte tengan un carro y el incremento q de la proporción de carro-habientes de 
    una cohorte a la anterior

*/
void car_prob(Queue * students, char* asignature, float p, float q, int c){

    char line[10];

    int cohorte;
    float cars = 0;
    float num_estudiantes = 0;
    int i;
    
    /*Leemos cada linea de estudiantes*/
    while( students->length != 0){

        strcpy(line, peek(students));

        num_estudiantes++;

        /*Extraemos la cohorte como entero*/
        cohorte = (line[0] - '0') * 10 + (line[1] - '0');
        
        /*
        Si es una cohorte posterior al 2000 sumamos 100 para mantener
        la distancia entre dos cohortes.
        */
        if(cohorte < 70 ) cohorte += 100;

        /*Sumamos la probabilidad del estudiante de tener carro segun su cohorte al acumulado.*/
        cars += (p + ((c+100)-cohorte)*q)/100;

        free(peek(students));
        dequeue(students);
    }

    num_estudiantes -= ceil(num_estudiantes * 0.25);

    printf("Para la asignatura \033[1;94m%s\033[0m se esperan \033[1;92m%.2f\033[0m carros. \n", asignature, cars);

    if((cars * 3) - num_estudiantes >= 0){
        printf("La cantidad de carros esperados es \033[1;92msuficiente\033[0m para la cantidad de estudiantes. \n");
    } else {
        printf("La cantidad de carros esperados es \033[1;91minsuficiente\033[0m para la cantidad de estudiantes. \n");
    }
}
