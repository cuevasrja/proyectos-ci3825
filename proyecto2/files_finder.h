#ifndef FILES_FINDER_H
#define FILES_FINDER_H

# include "queue.h"

/*
Funcion que busca las asignaturas de un estudiante en el directorio de comprobantes
@param root_name: directorio raiz de ambas sedes
@param carnet: carnet del estudiante
@return: arreglo de strings con los codigos de las materias y sus secciones
*/
Queue * find_asignatures(char* root_name, char* carnet);

/*
Funcion que busca los estudiantes de una materia en las listas definitivas
@param root_name: directorio raiz de ambas sedes
@param course_code: codigo de la materia
@return: arreglo de strings con los carnets de los estudiantes
*/
Queue * find_students(char* root_name, char* course_code);

#endif