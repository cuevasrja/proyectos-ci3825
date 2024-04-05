# include <stdio.h>
# include <math.h>
# include <string.h>

# include "calc_prob.h"

float student_prob(Queue * students, char* carnet){

    /*
    inicializamos la probabilidad acumulada en 25 contando el 15% de llegar 
    en el bus de la universidad y el 10% de llegar por transporte publico
    */
    float prob = 25.0;
    int cohorte;

    char *line[7];

    //Leemos cada linea de estudiantes
    while(students->length != 0){

        //Leemos la linea
        line = peek(students);

        //Extraemos la cohorte como entero
        cohorte = (line[0] - "0") * 10 + (line[1] - "0");
        
        /*
        Si es una cohorte posterior al 2000 sumamos 100 para mantener
        la distancia entre dos cohortes.
        */
        if(cohorte < 70 ){
            cohorte += 100;
        }

        //Sumamos la probabilidad del estudiante de tener carro segun su cohorte al acumulado.
        prob += 3 + (121-cohorte)*4;

        dequeue(students);
    }

    //Extraemos la cohorte del estudiante por el que se consulta como entero.
    cohorte = (carnet[0] - "0") * 10 + (carnet[1] - "0");

    //Si el estudiante por el que se consulta es de una cohorte posterior al 2000, sumamos 100 
    //para mantener la distancia entre dos cohortes.
    if(cohorte < 70 ) cohorte += 100;

    //Restamos la probabilidad del estudiante por el que se consulta de tener carro segun su 
    //cohorte al acumulado.
    prob -= 3 + (121-cohorte)*4;

    //La probabilidad maxima es de 100%
    if(prob > 100) prob = 100;

    return prob;
}

float student_prob(Queue * students, char* carnet, float p, float q){

    //inicializamos la probabilidad acumulada en 25 contando el 15% de llegar 
    //en el bus de la universidad y el 10% de llegar por transporte publico
    float prob = 25;
    int cohorte;

    char *line[7];

    //Leemos cada linea de estudiantes
    while(students->length != 0){

        //Leemos la linea
        line = peek(students);

        //Extraemos la cohorte como entero
        cohorte = (line[0] - "0") * 10 + (line[1] - "0");
        
        //Si es una cohorte posterior al 2000 sumamos 100 para mantener
        //la distancia entre dos cohortes.
        if(cohorte < 70 ) cohorte += 100;

        //Sumamos la probabilidad del estudiante de tener carro segun su cohorte al acumulado.
        prob += p + (121-cohorte)*q;

        dequeue(students);
    }

    //Extraemos la cohorte del estudiante por el que se consulta como entero.
    cohorte = (carnet[0] - "0") * 10 + (carnet[1] - "0");

    //Si el estudiante por el que se consulta es de una cohorte posterior al 2000, sumamos 100 
    //para mantener la distancia entre dos cohortes.
    if(cohorte < 70 ) cohorte += 100;

    //Restamos la probabilidad del estudiante por el que se consulta de tener carro segun su 
    //cohorte al acumulado.
    prob -= 3 + (121-cohorte)*4;

    //La probabilidad maxima es de 100%
    if(prob > 100) prob = 100;

    return prob;
}

void car_prob(Queue * students, char* asignature){

    //Buffer for line data storage
    char* line[7];

    int cohorte;
    float cars = 0;
    float num_estudiantes = 0;
    int i;
    
    //Leemos cada linea de estudiantes
    while( students->length != 0){

        line = peek(students);

        num_estudiantes++;

        //Extraemos la cohorte como entero
        cohorte = (line[0] - '0') * 10 + (line[1] - '0');
        
        //Si es una cohorte posterior al 2000 sumamos 100 para mantener
        //la distancia entre dos cohortes.
        if(cohorte < 70 ) cohorte += 100;

        //Sumamos la probabilidad del estudiante de tener carro segun su cohorte al acumulado.
        cars += 0.03 + (121-cohorte)*0.04;

        dequeue(students);
    }

    num_estudiantes -= ceil(num_estudiantes * 0.25);

    fprintf("Para la asignatura ");
    for(i = 0; i < 6; i++){
        fprintf("%c", asignature[i]);
    }
    fprintf(" se esperan %.2f carros. \n", cars);

    if((cars * 3) - num_estudiantes >= 0){
        fprintf("La cantidad de carros esperados es suficiente para la cantidad de estudiantes. \n");
    } else {
        fprintf("La cantidad de carros esperados es insuficiente para la cantidad de estudiantes. \n");
    }
}

void car_prob(Queue * students, char* asignature, float p, float q){

    //Buffer for line data storage
    char* line[7];

    int cohorte;
    float cars = 0;
    float num_estudiantes = 0;
    int i;
    
    //Leemos cada linea de estudiantes
    while( students->length != 0){

        line = peek(students);

        num_estudiantes++;

        //Extraemos la cohorte como entero
        cohorte = (line[0] - '0') * 10 + (line[1] - '0');
        
        /*
        Si es una cohorte posterior al 2000 sumamos 100 para mantener
        la distancia entre dos cohortes.
        */
        if(cohorte < 70 ) cohorte += 100;

        //Sumamos la probabilidad del estudiante de tener carro segun su cohorte al acumulado.
        cars += p + (121-cohorte)*q;

        dequeue(students);
    }

    num_estudiantes -= ceil(num_estudiantes * 0.25);

    printf("Para la asignatura ");
    for(i = 0; i < 6; i++){
        fprintf("%c", asignature[i]);
    }
    printf(" se esperan %.2f carros. \n", cars);

    if((cars * 3) - num_estudiantes >= 0){
        printf("La cantidad de carros esperados es suficiente para la cantidad de estudiantes. \n");
    } else {
        printf("La cantidad de carros esperados es insuficiente para la cantidad de estudiantes. \n");
    }
}
