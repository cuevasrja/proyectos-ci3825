# include <stdio.h>

int calc_prob(FILE* ptr, int estudiante){
    
    //Buffer for line data storage
    char* fline[200]
    char* line[7]

    //inicializamos la probabilidad acumulada en 25 contando el 15% de llegar 
    //en el bus de la universidad y el 10% de llegar por transporte publico
    int prob = 25;
    int cohorte;

    //Primera linea
    fgets(fline, 200, ptr);
    
    //Leemos cada linea de estudiantes
    while( fgets(line, 7, ptr) != NULL){

        //Leemos la linea
        fgets(line, 7, ptr);

        //Extraemos la cohorte como entero
        cohorte = (line[0] - "0") * 10 + (line[1] - "0")
        
        //Si es una cohorte posterior al 2000 sumamos 100 para mantener
        //la distancia entre dos cohortes.
        if(cohorte < 70 ) cohorte += 100;

        //Sumamos la probabilidad del estudiante de tener carro segun su cohorte al acumulado.
        prob += 3 + (121-cohorte)*4 
    }

    //Si el estudiante por el que se consulta es de una cohorte posterior al 2000, sumamos 100 
    //para mantener la distancia entre dos cohortes.
    if(estudiante < 70 ) estudiante += 100;

    //Restamos la probabilidad del estudiante por el que se consulta de tener carro segun su 
    //cohorte al acumulado.
    prob -= 3 + (121-estudiante)*4 

    //La probabilidad maxima es de 100%
    if(prob > 100){
        prob = 100;
    }

    return prob
}

int 