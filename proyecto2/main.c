/*  Proyecto 2. Pide Cola
    Participantes:
        Sandibel Soares
        Carlo Herrera
        Juan Cuevas

    Archivo: main.c
    Procesa el input del usuario dado por la línea de comandos.
    Proporciona mensajes de ayuda si se introduce mal alguna flag.
    Imprime mensajes de bienvenida y despedida.
    Dependiendo del input la función main decide qué acción realizar:
        Calcular la probabilidad asociada a un carnet.
        Calcular la cantidad de carros asociados a una materia.
*/

#include <stdio.h>  /* funcion printf */
#include <stdlib.h> /* constantes EXIT_FAILURE, EXIT_SUCCESS */
#include <string.h> /* funciones strlen, strcpy, strcmp */

# include "calc_prob.h"     /* funciones calc_prob, car_prob */
# include "files_finder.h"  /* funciones find_asignatures, find_students*/

/* Funcion que procesa los argumentos pasados por la linea de comando
   Retorna:
    -1 si hay errores en los argumentos,
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
    /* asumiendo que los carnets siempre tienen 7 caracteres */
    if (strlen(argv[1]) == 7)
    {
        /* si todos los caracteres estan entre [48, 57]  (los ascii de 0 hasta 9) 
           nos estan pasando un carnet
        */
       int i;
       for (i = 0; i < 7; i++)
       {
            if (argv[1][i] < 48 || argv[1][i] > 57)
            {
                printf("\033[91;1mError:\033[0m carnet invalido \n");
                printf("Use %s -h/--help para obtener ayuda\n", argv[0]);
                return -1;
            }
            
       }
       
       strcpy(carnet, argv[1]);
       result = 0;
    }
    /* Asumiendo que los codigos de materia siempre tienen 6 caracteres */
    else if (strlen(argv[1]) == 6){

        /* Nos estan pasando el codigo de una materia si:
            Los dos primeros caracteres estan entre [65, 90] (los ascii de A a Z)
            Si el tercer caracter esta entre [65, 90] (A...Z) o [48, 57] (0..9)
            El resto de caracteres estan entre [48, 57] (0..9)
        */
        int i;
        for (i = 0; i < 2; i++)
        {
            if (argv[1][i] < 65 || argv[1][i] > 90)
            {
                printf("\033[91;1mError:\033[0m Codigo de materia invalido \n");
                printf("Use %s -h/--help para obtener ayuda\n", argv[0]);
                return -1;
            }
        }

        if ( (argv[1][2] < 65 || argv[1][2] > 90) && (argv[1][2] < 48 || argv[1][2] > 57))
        {
            printf("\033[91;1mError:\033[0m Codigo de materia invalido \n");
            printf("Use %s -h/--help para obtener ayuda\n", argv[0]);
            return -1;
        }

        for (i = 3; i < 6; i++)
        {
            if (argv[1][i] < 48 || argv[1][i] > 57)
            {
                printf("\033[91;1mError:\033[0m Codigo de materia invalido \n");
                printf("Use %s -h/--help para obtener ayuda\n", argv[0]);
                return -1;
            }
        }

        strcpy(course_code, argv[1]);
        result = 1; 
    }
    else
    {
        printf("\033[91;1mError:\033[0m Carnet o codigo de materia invalido\n");
        printf("Use %s -h/--help para obtener ayuda\n", argv[0]);
        return -1;
    }
    
    
    /* Se procesan los posibles flags pasados */
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
            strcpy(root_dir, argv[i+1]);

        }
            
    }


    return result;

}

/* Funcion que imprime mensajes de ayuda con respecto al uso del programa y las flags */
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

/* Funcion que imprime la bienvenida al programa */
void print_welcome(){
    printf("👋 \033[93;1mBienvenido al sistema de análisis de datos de PideCola 3.0\033[0m 👋\n");
    printf("💻 \033[93;1mDesarrollado por:\033[0m\n");
    printf("  \033[93;1m-\033[0m Sandibel Soares\n");
    printf("  \033[93;1m-\033[0m Carlo Herrera\n");
    printf("  \033[93;1m-\033[0m Juan Cuevas\n\n");
}

/* Funcion que imprime la despedida al finalizar el programa */
void print_goodbye(){
    printf("\n");
    printf("👋 \033[93;1mGracias por usar el sistema de análisis de datos de PideCola 3.0\033[0m 👋\n");
    printf("\033[93;1mPD:\033[0m Alguien por favor que nos de una cola, nos quedamos en la uni 🥺\n");
}

/*  Funcion principal. 
    Llama a process_arguments para procesar el input
    Si no hubo errores, se decide qué acción realizar:
        Calcular la probabilidad asociada a un carnet.
        Calcular la cantidad de carros asociados a una materia
    Si alguna funcion reporta un error se retorna EXIT_FAILURE, 
    EXIT_SUCCESS de lo contrario.
*/
int main(int argc, char const *argv[])
{
    /* Variables */

    /* Arreglo de caracteres en donde se guarda el carnet pasado */
    char  carnet[10];
    /* Arreglo de caracteres en donde se guarda el codigo de una materia pasada */
    char  course_code[10];
    /* Ultima corte de la universidad, por defecto la 22 */
    int   last_cohort = 22;
    /* Probabilidad de que la ultima corte tenga un carro, por defecto 3 */
    float p_cohort = 3;
    /* Incremento de la probabilidad de tener carro con respecto a la ultima corte */
    float increase = 4;
    /* Ruta a la carpeta principal que contiene todos los archivos de DACE, por defecto  "./DACE"*/
    char  root_dir[100];
    strcpy(root_dir, "./DACE");

    /* Se comprueba si el usuario esta pidiendo ayuda para el uso del programa */
    if (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0))
    {
        print_help(argv);
        return EXIT_SUCCESS;
    }

    /* Se procesan los argumentos pasados por la linea de comandos */
    int r = process_arguments(argc, argv, carnet, course_code, \
                              &last_cohort, &p_cohort, &increase, root_dir);

    /* Si r == -1, hay un error en los argumentos pasados y se aborta el program */
    if (r == -1)
    {
        return EXIT_FAILURE;
    }

    print_welcome();


    if (r == 0)
    {
        /* Si r == 0, significa que hay una consulta por carnet*/

        printf("🧮 Vamos a calcular la probabilidad de que el estudiante \033[92;1m%s\033[0m pueda subir a la universidad en cola 🧮\n", carnet);
        
        /* Buscamos las asignaturas qu el estudiante pre-inscribio*/
        Queue * asignatures = find_asignatures(root_dir, carnet);
        /*Nos aseguramos que find_asignatures nos devolvio una cola */
        if (asignatures == NULL)
        {
            printf("\033[91;1mError:\033[0m Hubo un error al encontrar el comprobante del estudiante con carnet: %s\n", carnet);
            return EXIT_FAILURE;
        }
        if (asignatures->length == 0){
            printf("\033[91;1mError:\033[0m El estudiante no inscribio el trimestre\n");
            return EXIT_FAILURE;
        }

        /* Imprimimos lo encotrado */
        printf("📚 Asignaturas preinscritas por el estudiante:\n");
        print_queue(asignatures);
        printf("\033[94;1mTotal de asignaturas preinscritas:\033[0m %d\n", asignatures->length);

        /* Variable que usaremos para acceder a la lista de estudiantes de cada material*/
        Queue * students;
        /* La probabilidad global de que el estudiante pueda asistir a sus materias */
        float global_p = 0.0;
        /* Probabilidad de la materia que se este analizando */
        float course_p = 0.0;
        /* Codigo del curso que se analiza */
        char * course;

        /* Comenzamos a analizar los cursos que estan en la cola provista por find_asignatures*/
        while ( asignatures->head != NULL)
        {
            /* Accedemos al nombre de la materia */
            course = (char *)peek(asignatures);

            /* Buscamos los estudiantes de la materia */
            students = find_students(root_dir, course);
            
            /* Chequeamos que no se haya producido un error al usar la funcion find_students */
            if (students == NULL)
            {
                printf("\033[91;1mError:\033[0m Se produjo un error al buscar la lista de %s\n", course);
                return EXIT_FAILURE;
            }

            /* Revisamos si el estudiante retiro la materia en correccion */
            if (search(students, carnet) == FALSE)
            {
                printf("El estudiante retiro la materia: \033[94;1m%s\033[0m\n", course);
                /* Liberamos la memoria del string y del nodo*/
                free(course);
                dequeue(asignatures);
                continue;
            }
            
            /* Calculamos la probabilidad de la materia */
            course_p = student_prob(students, carnet, p_cohort, increase, last_cohort);
            global_p += course_p;
            
            /* Liberamos el string, el nodo de la cola de asignatura y el struct students*/
            free(course);
            dequeue(asignatures);
            free(students);
            
        }

        /* La probabilidad maxima es de 100%*/
        if(global_p > 100) global_p = 100;

        printf("Probabilidad total de asistencia: \033[92;1m%.2f%%\033[0m \n", global_p);
            
        
    }
    else if ( r == 1)
    {
        /* Si r == 1, significa que es una consulta por codigo de materia*/

        printf("🧮 Vamos a calcular la cantidad promedio de carros para la materia \033[92;1m%s\033[0m y si son suficientes para que todos los estudiantes puedan subir 🧮\n", course_code);

        /* Buscamos la lista de carnets de los estudiantes inscritos en la materia */
        Queue * students = find_students(root_dir, course_code);

        /* Revisamos que no se haya producido ningun error en la funcion find_students*/
        if (students == NULL)
        {
            printf("\033[91;1mError:\033[0m Hubo un error al buscar la lista de la materia %s\n", course_code);
            return EXIT_FAILURE;
        }
        if (students->length == 0){
            printf("\033[91;1mError:\033[0m No hay una sección abierta de la materia %s\n", course_code);
            return EXIT_FAILURE;
        }

        /*Imprimimos la lista de estudiantes encontrada*/
        printf("🎓 Estudiantes inscritos en la materia:\n");
        print_queue(students);
        printf("\033[94;1mTotal de estudiantes inscritos:\033[0m %d\n", students->length);

        /* Calculamos la cantidad de carros esperados y si son suficientes*/
        car_prob(students, course_code, p_cohort, increase, last_cohort);

        /*liberamos la memoria del struct students*/
        free(students);
    }
    
    print_goodbye();
    
    return EXIT_SUCCESS;
}
