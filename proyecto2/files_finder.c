# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <dirent.h>
# include <sys/stat.h>
# include <sys/types.h>

# include "files_finder.h"
#define MAX_SECTIONS 10


/*
Busca los archivos de comprobantes de un estudiante en el directorio de comprobantes
y devuelve un arreglo con los codigos de las materias y sus secciones
*/
Queue * find_asignatures(char* root_name, char* carnet){

    /* cola donde guardaremos los codigos de las asignaturas que esta viendp el estudiante*/
    Queue * asignatures = new_queue();

    int file_found = 0;

    /* Vamos a abir cuatro directorios:
        el directorio raiz con todos los archivos (DACE o el dado)
        cada directorio de las sedes, ya que no sabemos a cual pertenece el estudiante
        sus directorios de comprobantes correspondientes y el dir de la cohorte
    */
    DIR * root_dir, *sede_dir, *comp_dir, *cohorte_dir;
    struct dirent *ent_root_dir, * ent_sede_dir, *ent_comp_dir, *ent_coh_dir;

    /* buffers donde guardaremos las rutas */
    char sede_path[ strlen(root_name) + 20 ];
    char comp_path[ strlen(root_name) + 30 ];
    char coh_path[ strlen(root_name)  + 35 ];
    char carnet_path[ strlen(root_name) + 50 ];

    /* Comprobamos poder abrir el directorio raiz */
    if ((root_dir = opendir(root_name)) == NULL )
    {
        printf("No se pudo abrir el directorio: %s \n", root_name);
        return NULL;
    }

    while ((ent_root_dir = readdir(root_dir)) != NULL && file_found == 0)
    {
        if(strcmp(ent_root_dir->d_name, ".") == 0 || strcmp(ent_root_dir->d_name, "..") == 0){
            continue;
        }
        
        /* Podemos hacer una funcion auxiliar para crear las rutas */
        strcpy(sede_path, root_name);
        strcat(sede_path, "/");
        strcat(sede_path, ent_root_dir->d_name);

        printf("Ruta a la sede: %s \n", sede_path);

        if ((sede_dir = opendir(sede_path)) == NULL)
        {
            printf("No se pudo abrir el directorio: %s \n", sede_path);
            return NULL;
        }

        while ((ent_sede_dir = readdir(sede_dir)) != NULL)
        {
            if (strcmp(ent_sede_dir->d_name, "comprobantes") != 0)
            {
                continue;
            }

            strcpy(comp_path, sede_path);
            strcat(comp_path, "/");
            strcat(comp_path, ent_sede_dir->d_name);

            printf("Ruta a los comprobantes de la sede: %s \n", comp_path);


            if ((comp_dir = opendir(comp_path)) == NULL)
            {
                printf("No se pudo abrir el directorio: %s \n", comp_path);
                return NULL;
            }

            while ((ent_comp_dir = readdir(comp_dir)) != NULL)
            {
                if(strncmp(ent_comp_dir->d_name, carnet, 2) != 0){
                    continue;
                }

                strcpy(coh_path, comp_path);
                strcat(coh_path, "/");
                strcat(coh_path, ent_comp_dir->d_name);

                printf("Ruta a la cohorte buscada: %s \n", coh_path);

                if ((cohorte_dir = opendir(coh_path)) == NULL)
                {
                    printf("No se pudo abrir el directorio: %s \n", coh_path);
                    return NULL;
                }

                while ((ent_coh_dir = readdir(cohorte_dir)) != NULL)
                {

                    if(strncmp(ent_coh_dir->d_name, carnet, 7) != 0){
                        continue;
                    }

                    /* Esto debe ir en otra funcion */
                    printf("Archivo encontrado!\n");
                    FILE *carnet_file;

                    strcpy(carnet_path, coh_path);
                    strcat(carnet_path, "/");
                    strcat(carnet_path, ent_coh_dir->d_name);

                    printf("Ruta al archivo buscada: %s \n", carnet_path);

                    carnet_file = fopen(carnet_path, "r");
                    if (carnet_file == NULL) {
                        perror("No se pudo abrir el archivo");
                        return NULL;
                    }
                    char* line = NULL;
                    int j = 0;
                    size_t len = 0;
                    while (getline(&line, &len, carnet_file) != -1) {
                        if (j == 0){
                            j++;
                            continue;
                        }
                        char* token = strtok(line, " - ");
                        if (token != NULL){
                            char * code_asig = (char*)malloc(10*sizeof(char));
                            if (code_asig == NULL)
                            {
                                perror("No se pudo reservar memoria. asignatures[i] en find_asignatures");
                                return NULL;
                            }
                            strcpy(code_asig, token);
                            enqueue(asignatures, (void *) code_asig);
                        }
                    }
                    /* Hay que revisar que se cerró bien */
                    fclose(carnet_file);
                    free(line);
                    file_found = 1;

                }
                     
                closedir(cohorte_dir);
            }

            closedir(comp_dir);
        }    

        closedir(sede_dir);
    }
    
    closedir(root_dir);
    return asignatures;
}


/*
Busca los estudiantes inscritos en un curso sin importar la seccion
*/
Queue * find_students(char* root_name, char* course_code){

    Queue * students = new_queue();

    char sede_path[ strlen(root_name) + 20 ];
    char list_path[ strlen(root_name) + 30 ];
    char apt_path[ strlen(root_name)  + 35 ];
    char course_path[ strlen(root_name) + 60 ];

    DIR * root_dir, *sede_dir, *list_dir, *apt_dir;
    struct dirent *ent_root_dir, * ent_sede_dir, *ent_list_dir, *ent_apt_dir;

    if ((root_dir = opendir(root_name)) == NULL )
    {
        printf("No se pudo abrir el directorio: %s \n", root_name);
        return NULL;
    }

    while ((ent_root_dir = readdir(root_dir)) != NULL)
    {
        if(strcmp(ent_root_dir->d_name, ".") == 0 || strcmp(ent_root_dir->d_name, "..") == 0){
            continue;
        }
        
        strcpy(sede_path, root_name);
        strcat(sede_path, "/");
        strcat(sede_path, ent_root_dir->d_name);

        printf("Ruta a la sede: %s \n", sede_path);

        if ((sede_dir = opendir(sede_path)) == NULL)
        {
            printf("No se pudo abrir el directorio: %s \n", sede_path);
            return NULL;
        }

        while ((ent_sede_dir = readdir(sede_dir)) != NULL)
        {
            if (strcmp(ent_sede_dir->d_name, "listas") != 0)
            {
                continue;
            }

            strcpy(list_path, sede_path);
            strcat(list_path, "/");
            strcat(list_path, ent_sede_dir->d_name);

            printf("Ruta a las listas de la sede: %s \n", list_path);


            if ((list_dir = opendir(list_path)) == NULL)
            {
                printf("No se pudo abrir el directorio: %s \n", list_path);
                return NULL;
            }

            while ((ent_list_dir = readdir(list_dir)) != NULL)
            {
                if(strncmp(ent_list_dir->d_name, course_code, 2) != 0){
                    continue;
                }

                strcpy(apt_path, list_path);
                strcat(apt_path, "/");
                strcat(apt_path, ent_list_dir->d_name);

                printf("Ruta al apartemonto buscada: %s \n", apt_path);

                if ((apt_dir = opendir(apt_path)) == NULL)
                {
                    printf("No se pudo abrir el directorio: %s \n", apt_path);
                    return NULL;
                }

                while ((ent_apt_dir = readdir(apt_dir)) != NULL)
                {

                    if(strncmp(ent_apt_dir->d_name, course_code, 6) != 0){
                        continue;
                    }

                    /* Esto debe ir en otra funcion */
                    printf("Archivo encontrado!\n");
                    FILE *course_file;

                    strcpy(course_path, apt_path);
                    strcat(course_path, "/");
                    strcat(course_path, ent_apt_dir->d_name);

                    printf("Ruta al archivo buscada: %s \n", course_path);

                    course_file = fopen(course_path, "r");
                    if (course_file == NULL) {
                        perror("No se pudo abrir el archivo");
                        return NULL;
                    }
                    char* line = NULL;
                    int j = 0;
                    size_t len = 0;
                    while (getline(&line, &len, course_file) != -1) {
                        if (j == 0){
                            j++;
                            continue;
                        }
                        char* token = strtok(line, "\n");
                        if (token != NULL){
                            char * carnet_i = (char*)malloc(10*sizeof(char));
                            if (carnet_i == NULL)
                            {
                                perror("No se pudo reservar memoria. asignatures[i] en find_students");
                                return NULL;
                            }
                            strcpy(carnet_i, token);
                            enqueue(students, (void *) carnet_i);
                        }
                    }
                    fclose(course_file);
                    free(line);
                }
                     
                closedir(apt_dir);
            }

            closedir(list_dir);
        }    

        closedir(sede_dir);
    }
    
    closedir(root_dir);
    return students;

}
