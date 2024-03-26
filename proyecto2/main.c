#include <stdio.h>  /* printf */
#include <stdlib.h> /* EXIT_FAILURE, EXIT_SUCCESS*/
#include <string.h> /* strlen(), strcpy(), strcmp()*/

# include "calc_prob.h"
# include "files_finder.h"

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
    if (argc <= 1)
    {
        printf("\033[91;1mError:\033[0m Debe ingresar el carnet de un estudiante o el codigo de una materia\n");
        printf("Use %s -h/--help para obtener ayuda\n", argv[0]);
        return -1;
    }
    else if (argc%2 == 1)
    {
        printf("\033[91;1mError:\033[0m Cantidad de argumentos invalida\n");
        printf("Use %s -h/--help para obtener ayuda\n", argv[0]);
        return -1;
    }
    

    /* Hay que verificar si se pasó un carnet o el codigo de una materia (y que sean validos)*/
    /* asumiendo que los carnets siempre tiene 7 caracteres */
    if (strlen(argv[1]) == 7)
    {
        /* si todos los caracteres estan entre [48, 57]  (los ascii de 0 hasta 9) 
           nos estan pasando un carnet
        */
       strcpy(carnet, argv[1]);
       result = 0;
    }
    /* Asumiendo que los codigos de materia siempre tienen 6 caracteres (??)*/
    else if (strlen(argv[1]) == 6){

        /* podemos revisar si los primeros dos caracteres del argumento NO estan entre
           [48, 57], esto podria indicar que es un cod de materia*/
        strcpy(course_code, argv[1]);
        result = 1; 
    }
    else
    {
        printf("\033[91;1mError:\033[0m Carnet o codigo de materia invalido\n");
        printf("Use %s -h/--help para obtener ayuda\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    int i;
    for (i = 2; i < argc; i += 2)
    {   
        if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--cohorte") == 0)
        {
            if (atoi(argv[i + 1]) == 0)
            {
                printf("\033[91;1mError:\033[0m La cohorte debe ser un numero entero\n");
                printf("Use %s -h/--help para obtener ayuda\n", argv[0]);
                return -1;
            }
            *last_cohort = atoi(argv[i+1]);
            
        }

        if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--pcohorte") == 0)
        {
            if (atof(argv[i + 1]) == 0)
            {
               printf("\033[91;1mError:\033[0m La probabilidad de cohorte debe ser un numero\n");
               printf("Use %s -h/--help para obtener ayuda\n", argv[0]);
               return -1;
            }
            *p_cohort = atof(argv[i+1]);
            
        }

        if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--incremento") == 0)
        {
            if (atof(argv[i + 1]) == 0)
            {
                printf("\033[91;1mError:\033[0m El incremento debe ser un numero\n");
                printf("Use %s -h/--help para obtener ayuda\n", argv[0]);
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

void print_help(char const *argv[]){
    printf("\033[93;1mUso:\033[0m %s <carnet|codigo_materia> [opciones]\n", argv[0]);
    printf("Calcula la probabilidad de que un estudiante o una seccion pueda subir a la universidad por cola\n");
    printf("\033[93;1mOpciones:\033[0m\n");
    printf("  \033[92m-c, --cohorte <cohorte>\033[0m\n       Cohorte de los estudiantes\n");
    printf("  \033[92m-p, --pcohorte <probabilidad>\033[0m\n       Probabilidad de tener un carro en la cohorte\n");
    printf("  \033[92m-i, --incremento <incremento>\033[0m\n       Incremento de la probabilidad por cohorte\n");
    printf("  \033[92m-d <directorio>\033[0m\n       Directorio donde se encuentran los archivos de los estudiantes\n");
    printf("\033[93;1mEjemplos:\033[0m\n");
    printf("  %s 20180001 -c 21 -p 0.3 -i 0.04 -d ./DACE\n", argv[0]);
    printf("  %s 1234567 -c 21 -p 0.3 -i 0.04 -d ./DACE\n", argv[0]);
    printf("  %s 1910056\n", argv[0]);
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

    if (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0))
    {
        print_help(argv);
        return EXIT_SUCCESS;
    }

    /* Procesar los argumentos pasados por la linea de comandos */
    int r = process_arguments(argc, argv, carnet, course_code, \
                              &last_cohort, &p_cohort, &increase, root_dir);

    
    if (r == 0)
    {
        printf("Carnet: \033[92;1m%s\033[0m\n", carnet);
        /* Si r == 0, llamamos el codigo que calcula la proba por carnet*/
        char** asignatures = find_asignatures(root_dir, carnet);
        if (asignatures[0] == NULL)
        {
            printf("\033[91;1mError:\033[0m No se encontraron asignaturas para el carnet %s\n", carnet);
            return EXIT_FAILURE;
        }
        int i = 0;
        while (asignatures[i] != NULL)
        {
            printf("%s\n", asignatures[i]);
            i++;
        }
        free(asignatures);
    }
    else if ( r == 1)
    {
        /* Si r == 1, llamamos el codigo que calcula la cantidad de carros por materia */
        printf("Codigo de materia: \033[92;1m%s\033[0m\n", course_code);
        char** students = find_students(root_dir, course_code);
        if (students[0] == NULL)
        {
            printf("\033[91;1mError:\033[0m No se encontraron estudiantes para la materia %s\n", course_code);
            return EXIT_FAILURE;
        }
        int i = 0;
        while (students[i] != NULL)
        {
            printf("%d. %s\n", i, students[i]);
            i++;
        }
    }
    else
    {
        return EXIT_FAILURE;
    }
    

    return EXIT_SUCCESS;
}
