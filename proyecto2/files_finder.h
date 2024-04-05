#ifndef FILES_FINDER_H
#define FILES_FINDER_H

# include "queue.h"

/*
Funcion que busca las asignaturas inscritas de un estudiante en los directorios 
de comprobantes de ambas sedes
@param root_name: directorio raiz de ambas sedes
@param carnet: carnet del estudiante
@return: Queue cuyos nodos son apuntadores a arreglos de caracteres con los codigos 
        de las materias materias
*/
Queue * find_asignatures(char* root_name, char* carnet);

/*
Funcion que busca los estudiantes de una materia en las listas definitivas
@param root_name: directorio raiz de ambas sedes
@param course_code: codigo de la materia
@return: Queue cuyos nodos son apuntadores a arreglos de caracteres con los carnets 
        de los estudiantes que inscribieron la materia (No importa la seccion)
*/
Queue * find_students(char* root_name, char* course_code);

#endif