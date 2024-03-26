# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <dirent.h>
# include <sys/stat.h>
# include <sys/types.h>

#define MAX_SECTIONS 10

/*
Busca los archivos de comprobantes de un estudiante en el directorio de comprobantes
y devuelve un arreglo con los codigos de las materias y sus secciones
*/
char** find_asignatures(char* root_name, char* carnet){
    char** asignatures = (char**)malloc(8*sizeof(char*));
    if (asignatures == NULL)
    {
        perror("No se pudo reservar memoria. asignatures en find_asignatures");
        return NULL;
    }
    char* target_file = (char*)malloc(10*sizeof(char));
    if (target_file == NULL)
    {
        perror("No se pudo reservar memoria. target_file en find_asignatures");
        return NULL;
    }
    strcat(target_file, carnet);
    strcat(target_file, ".txt");
    char* carnet_prefix = (char*)malloc(3*sizeof(char));
    if (carnet_prefix == NULL)
    {
        perror("No se pudo reservar memoria. carnet_prefix en find_asignatures");
        return NULL;
    }
    strncpy(carnet_prefix, carnet, 2);
    carnet_prefix[2] = '\0';
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(root_name)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if(strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0){
                char* file_name = (char*)malloc(50*sizeof(char));
                strcpy(file_name, root_name);
                strcat(file_name, "/");
                strcat(file_name, ent->d_name);
                DIR *dir2;
                struct dirent *ent2;
                if ((dir2 = opendir(file_name)) != NULL) {
                    while ((ent2 = readdir(dir2)) != NULL) {
                        if(strcmp(ent2->d_name, "comprobantes") == 0){
                            char* file_name2 = (char*)malloc(50*sizeof(char));
                            strcpy(file_name2, file_name);
                            strcat(file_name2, "/");
                            strcat(file_name2, ent2->d_name);
                            DIR *dir3;
                            struct dirent *ent3;
                            if ((dir3 = opendir(file_name2)) != NULL) {
                                while ((ent3 = readdir(dir3)) != NULL) {
                                    if(strcmp(ent3->d_name, carnet_prefix) == 0){
                                        DIR *dir4;
                                        struct dirent *ent4;
                                        char* file_name3 = (char*)malloc(50*sizeof(char));
                                        strcpy(file_name3, file_name2);
                                        strcat(file_name3, "/");
                                        strcat(file_name3, ent3->d_name);
                                        if ((dir4 = opendir(file_name3)) != NULL) {
                                            while ((ent4 = readdir(dir4)) != NULL) {
                                                if(strcmp(ent4->d_name, target_file) == 0){
                                                    FILE *file;
                                                    char* file_name4 = (char*)malloc(50*sizeof(char));
                                                    strcpy(file_name4, file_name3);
                                                    strcat(file_name4, "/");
                                                    strcat(file_name4, ent4->d_name);
                                                    file = fopen(file_name4, "r");
                                                    if (file == NULL) {
                                                        perror("No se pudo abrir el archivo");
                                                        return NULL;
                                                    }
                                                    char* line = NULL;
                                                    int i = 0;
                                                    int j = 0;
                                                    size_t len = 0;
                                                    while (getline(&line, &len, file) != -1) {
                                                        if (j == 0){
                                                            j++;
                                                            continue;
                                                        }
                                                        char* token = strtok(line, " - ");
                                                        if (token != NULL){
                                                            asignatures[i] = (char*)malloc(10*sizeof(char));
                                                            if (asignatures[i] == NULL)
                                                            {
                                                                perror("No se pudo reservar memoria. asignatures[i] en find_asignatures");
                                                                return NULL;
                                                            }
                                                            strcpy(asignatures[i], token);
                                                            i++;
                                                        }
                                                    }
                                                    fclose(file);
                                                    free(line);
                                                    free(file_name4);
                                                }
                                            }
                                            closedir(dir4);
                                        } else {
                                            perror("No se pudo abrir el directorio");
                                            return NULL;
                                        }
                                        free(file_name3);
                                    }
                                }
                                closedir(dir3);
                            } else {
                                perror("No se pudo abrir el directorio");
                                return NULL;
                            }
                            free(file_name2);
                        }
                    }
                    closedir(dir2);
                } else {
                    perror("No se pudo abrir el directorio");
                    return NULL;
                }
                free(file_name);
            }
        }
    } else {
        perror("No se pudo abrir el directorio");
        return NULL;
    }
    closedir(dir);
    free(target_file);
    free(carnet_prefix);
    return asignatures;
}

/*
Busca los estudiantes inscritos en un curso sin importar la seccion
! No funciona bien si hay mas de una seccion de la misma materia
*/
char** find_students(char* root_name, char* course_code){
    char** students = (char**)malloc(100*sizeof(char*));
    if (students == NULL)
    {
        perror("No se pudo reservar memoria. students en find_students");
        return NULL;
    }
    char* target = (char*)malloc(30*sizeof(char));
    if (target == NULL)
    {
        perror("No se pudo reservar memoria. target en find_students");
        return NULL;
    }
    strcat(target, course_code);
    strcat(target, " seccion ");
    char* course_prefix = (char*)malloc(3*sizeof(char));
    if (course_prefix == NULL)
    {
        perror("No se pudo reservar memoria. carnet_prefix en find_asignatures");
        return NULL;
    }
    strncpy(course_prefix, course_code, 2);
    course_prefix[2] = '\0';
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(root_name)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if(strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0){
                char* file_name = (char*)malloc(50*sizeof(char));
                strcpy(file_name, root_name);
                strcat(file_name, "/");
                strcat(file_name, ent->d_name);
                DIR *dir2;
                struct dirent *ent2;
                if ((dir2 = opendir(file_name)) != NULL) {
                    while ((ent2 = readdir(dir2)) != NULL) {
                        if(strcmp(ent2->d_name, "listas") == 0){
                            char* file_name2 = (char*)malloc(50*sizeof(char));
                            strcpy(file_name2, file_name);
                            strcat(file_name2, "/");
                            strcat(file_name2, ent2->d_name);
                            DIR *dir3;
                            struct dirent *ent3;
                            if ((dir3 = opendir(file_name2)) != NULL) {
                                while ((ent3 = readdir(dir3)) != NULL) {
                                    if(strcmp(ent3->d_name, course_prefix) == 0){
                                        DIR *dir4;
                                        struct dirent *ent4;
                                        char* file_name3 = (char*)malloc(50*sizeof(char));
                                        strcpy(file_name3, file_name2);
                                        strcat(file_name3, "/");
                                        strcat(file_name3, ent3->d_name);
                                        if ((dir4 = opendir(file_name3)) != NULL) {
                                            int i = 0;
                                            while ((ent4 = readdir(dir4)) != NULL) {
                                                int seccion = 1;
                                                while (seccion <= MAX_SECTIONS){
                                                    char* target_file = (char*)malloc(30*sizeof(char));
                                                    if (target_file == NULL)
                                                    {
                                                        perror("No se pudo reservar memoria. target_file en find_students");
                                                        return NULL;
                                                    }
                                                    strcat(target_file, target);
                                                    char* section = (char*)malloc(3*sizeof(char));
                                                    if (section == NULL)
                                                    {
                                                        perror("No se pudo reservar memoria. section en find_students");
                                                        return NULL;
                                                    }
                                                    sprintf(section, "%d", seccion);
                                                    strcat(target_file, section);
                                                    strcat(target_file, ".txt");
                                                    if (strcmp(ent4->d_name, target_file) == 0){
                                                        FILE *file;
                                                        char* file_name4 = (char*)malloc(50*sizeof(char));
                                                        strcpy(file_name4, file_name3);
                                                        strcat(file_name4, "/");
                                                        strcat(file_name4, ent4->d_name);
                                                        file = fopen(file_name4, "r");
                                                        if (file == NULL) {
                                                            perror("No se pudo abrir el archivo");
                                                            return NULL;
                                                        }
                                                        char* line = NULL;
                                                        int j = 0;
                                                        size_t len = 0;
                                                        while (getline(&line, &len, file) != -1) {
                                                            if(j == 0){
                                                                j++;
                                                                continue;
                                                            }
                                                            students[i] = (char*)malloc(10*sizeof(char));
                                                            if (students[i] == NULL)
                                                            {
                                                                perror("No se pudo reservar memoria. students[i] en find_students");
                                                                return NULL;
                                                            }
                                                            char* token = strtok(line, "\n");
                                                            strcpy(students[i], token);
                                                            i++;
                                                        }
                                                        fclose(file);
                                                        free(line);
                                                        free(file_name4);
                                                        break;
                                                    }
                                                    seccion++;
                                                }
                                            }
                                            closedir(dir4);
                                        } else {
                                            perror("No se pudo abrir el directorio");
                                            return NULL;
                                        }
                                        free(file_name3);
                                    }
                                }
                                closedir(dir3);
                            } else {
                                perror("No se pudo abrir el directorio");
                                return NULL;
                            }
                            free(file_name2);
                        }
                    }
                    closedir(dir2);
                } else {
                    perror("No se pudo abrir el directorio");
                    return NULL;
                }
                free(file_name);
            }
        }
        closedir(dir);
    } else {
        perror("No se pudo abrir el directorio");
        return NULL;
    }
    free(target);
    free(course_prefix);
    return students;                               
}