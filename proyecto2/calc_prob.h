#ifndef CALC_PROB
#define CALC_PROB

# include "queue.h"

/*
Funcion que toma los estudiantes de una sección y calcula la probabilidad de que un 
estudiante pueda conseguir cola con alguno de sus compañeros.
@param students: arreglo de strings con los carnets de los estudiantes de un curso.
@param carnet: carnet del estudiante por el que se consulta.
@param p: probabilidad porcentual de que un estudiante de la cohorte mas reciente tenga carro.
@param q: incremento de la proporción de carro-habientes de una cohorte a la anterior.
@param c: ultima cohorte de la que se tiene información.
@return: probabilidad porcentual de que el estudiante pueda conseguir cola.
*/
float student_prob(Queue * students, char* carnet, float p, float q, int c);

/*
Funcion que toma los estudiantes de una sección y calcula la cantidad de carros esperados 
para la materia y si esta es suficiente para que todos puedan subir. 
@param students: arreglo de strings con los carnets de los estudiantes de un curso.
@param asignature: código de la asignatura por la que se consulta.
@param p: probabilidad porcentual de que un estudiante de la cohorte mas reciente tenga carro.
@param q: incremento de la proporción de carro-habientes de una cohorte a la anterior. 
@param c: ultima cohorte de la que se tiene información.
*/
void car_prob(Queue * students, char* asignature, float p, float q, int c);

#endif