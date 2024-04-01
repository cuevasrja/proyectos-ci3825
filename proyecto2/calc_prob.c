# include <stdio.h>
# include <math.h>

//Calculo de  Probabilidad con valores por defecto
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

void car_prob(FILE* ptr){

    //Buffer for line data storage
    char* fline[200]
    char* line[7]

    float cars;
    int cohorte;
    float num_estudiantes = 0;
    int i;

    //Primera linea
    fgets(fline, 200, ptr);
    
    //Leemos cada linea de estudiantes
    while( fgets(line, 7, ptr) != NULL){n

        //Leemos la linea
        fgets(line, 7, ptr);

        num_estudiantes++;

        //Extraemos la cohorte como entero
        cohorte = (line[0] - "0") * 10 + (line[1] - "0")
        
        //Si es una cohorte posterior al 2000 sumamos 100 para mantener
        //la distancia entre dos cohortes.
        if(cohorte < 70 ) cohorte += 100;

        //Sumamos la probabilidad del estudiante de tener carro segun su cohorte al acumulado.
        cars += 0.03 + (121-cohorte)*0.04 
    }

    num_estudiantes = num_estudiantes * 0.25;

    fprintf("Para la asignatura ");
    for(i = 0; i < 6; i++){
        fprintf("%c", fline[i]);
    }
    fprintf(" se esperan %d carros \n", cars);

    if((cars * 3) - num_estudiantes >= 0){
        fprintf("La cantidad de carros esperados es suficiente para la cantidad de estudiantes.");
    } else {
        fprintf("La cantidad de carros esperados es insuficiente para la cantidad de estudiantes.");
    }
}