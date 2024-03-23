#include <stdio.h>  /* printf */
#include <stdlib.h> /* EXIT_FAILURE, EXIT_SUCCESS*/
#include <string.h> /* strlen(), strcpy(), strcmp()*/

#include "calc_prob.h"

/* Funcion que procesa los argumentos pasados por la linea de comando
   Retorna:
    -1 si hay errorres en los argumentos,
    0  si se tiene que calcular la probabilidad asociada a un carnet,
    1  si se tiene que calcular la cantidad de carros de una materia
*/
int process_arguments(int argc, char const *argv[], char carnet[10], char course_code[10],\
                      int *last_cohort, float *p_cohort, float *increase, char root_dir[50]){

    int result = -1;

    /* Debe haber como minimo dos argumentos: el nombre de archivo y el carnet|codigo_materia */
    if (argc == 1)
    {
        printf("error: debe ingresar el carnet de un estudiante o el codigo de una materia\n");
        return -1;
    }

    /* Hay que verificar si se pasó un carnet o el codigo de una materia (y que sean validos)*/
    /* asumiendo que los carnets siempre tiene 7 caracteres */
    if (strlen(argv[1]) == 7)
    {
        /* si todos los caracteres estan entre [48, 57]  (los ascii de 0 hasta 9) 
           nos estan pasando un carnet
        */
       result = 0;
    }
    /* Asumiendo que los codigos de materia siempre tienen 6 caracteres (??)*/
    else if (strlen(argv[1]) == 6){

        /* podemos revisar si los primeros dos caracteres del argumento NO estan entre
           [48, 57], esto podria indicar que es un cod de materia*/

        result = 1; 
    }
    else
    {
        printf("error: carnet|materia con formato incorrecto\n");
        return EXIT_FAILURE;
    }
    
    /* falta -h --help*/
    int i;
    for (i = 2; i < argc; i += 2)
    {
        if (i + 1 >= argc)
        {
            printf("error: cantidad de argumentos insuficiente\n");
            return -1;
        }
        
        if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--cohorte") == 0)
        {
            if (atoi(argv[i + 1]) == 0)
            {
                printf("error: el arguemnto para --cohorte debe ser un entero\n");
                return -1;
            }
            *last_cohort = atoi(argv[i+1]);
            
        }

        if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--pcohorte") == 0)
        {
            if (atof(argv[i + 1]) == 0)
            {
               printf("error: la probabilidad de la cohorte debe ser un numero\n");
               return -1;
            }
            *p_cohort = atof(argv[i+1]);
            
        }

        if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--incremento") == 0)
        {
            if (atof(argv[i + 1]) == 0)
            {
                printf("error: el incremento entre cohortes debe ser un numero\n");
                return -1;
            }
            *increase = atof(argv[i+1]);

        }

        if (strcmp(argv[i], "-d")  == 0 )
        {
            /* deberiamos comprobar aqui que el directorio existe? */
            strcpy(root_dir, argv[i+1]);

        }
            
    }


    return result;

}

int main(int argc, char const *argv[])
{
    /* Variables */
    char  carnet[10];
    char  course_code[10];
    int   last_cohort = 21;
    float p_cohort = 3;
    float increase = 4;
    char  root_dir[50];
    strcpy(root_dir, "./DACE");

    /* Procesar los argumentos pasados por la linea de comandos */
    int r = process_arguments(argc, argv, carnet, course_code, \
                              &last_cohort, &p_cohort, &increase, root_dir);

    
    if (r == 0)
    {
         /* Si r == 0, llamamos el codigo que calcula la proba por carnet*/
    }
    else if ( r == 1)
    {
        /* Si r == 1, llamamos el codigo que calcula la cantidad de carros por materia */
    }
    else
    {
        return EXIT_FAILURE;
    }
    

    return EXIT_SUCCESS;
}
