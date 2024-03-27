# !/bin/bash

# Script que lee todos los codigos de las materias y realiza la busqueda de probabilidad de subir de cada una
# de las materias

# Se revisa que la cantidad de argumentos es par o igual a 0 y menor o igual a 6. En caso de no serlo, se imprime un mensaje de error
if [ $(( $# % 2 )) -ne 0 ] || [ $# -gt 6 ]; then
    echo "\033[91;1mError:\033[0m Cantidad de argumentos invalida"
    exit 1
fi

ROOT_PATH="DACE"
# Revisamos si hay un flag -d en los argumentos (Puede estar en la posicion 1, 3 o 5)
if [ "$1" == "-d" ]; then
    ROOT_PATH=$2
fi
if [ "$3" == "-d" ]; then
    ROOT_PATH=$4
fi
if [ "$5" == "-d" ]; then
    ROOT_PATH=$6
fi

# Se crea un archivo temporal llamado all_courses.txt. En caso de que ya exista, se elimina
if [ -f all_courses.txt ]; then
    rm all_courses.txt
fi

# Se recorren las subcarpetas de la carpeta de ROOT_PATH (son dos subcarpetas: Sartenejas y Litoral)
for CAMPUS in $(ls $ROOT_PATH); do
    # Se hace ls a todos los directorios en las carpetas ROOT_PATH/CAMPUS/listas/ y se copian 
    # las primeras lineas de cada archivo en temp.txt (Eliminano el salto de linea al final de cada linea)
    # tomando solo el codigo de la materia de la primera linea (Ejemplo: "CI2511 - Logica Simbolica" -> "CI2511")
    for DEPARTAMENTO in $(ls $ROOT_PATH/$CAMPUS/listas/); do
        # Tomar en cuenta que los nombres de los archivos en listas/$DEPARTAMENTO pueden tener espacios
        ls $ROOT_PATH/$CAMPUS/listas/$DEPARTAMENTO | while read FILE; do
            head -n 1 $ROOT_PATH/$CAMPUS/listas/$DEPARTAMENTO/"$FILE" | cut -d ' ' -f 1 >> temp.txt
        done
    done
done

# Se ordena el archivo en all_courses.txt y se eliminan las lineas duplicadas
sort temp.txt | uniq > all_courses.txt
# Se elimina el archivo temporal
rm temp.txt

# Se imprime la cantidad de materias
echo -e "Cantidad de materias: \033[92;1m$(wc -l all_courses.txt | cut -d ' ' -f 1)\033[0m"

# Se elimina el archivo de probabilidades si ya existe
if [ -f probabilities.txt ]; then
    rm probabilities.txt
fi

# Se recorren todos los codigos de las materias en all_courses.txt
while read LINE; do
    # TODO: Revisar la probabilidad de subir de cada materia
    # Se ejecuta el programa pidecola.out con el codigo de la materia y los argumentos dados
    # ./pidecola.out $LINE $@ | grep "Probabilidad de subir: " | cut -d ' ' -f 4 >> probabilities.txt
    echo -e "Material: \033[92;1m$LINE\033[0m"
done < all_courses.txt

# Se calcula la probabilidad promedio de subir
AVERAGE_PROBABILITY=$(awk '{s+=$1} END {print s/NR}' probabilities.txt)
echo -e "Probabilidad promedio de subir: \033[92;1m$AVERAGE_PROBABILITY\033[0m"
# Se calcula la probabilidad maxima de subir
MAX_PROBABILITY=$(sort -n probabilities.txt | tail -n 1)
echo -e "Probabilidad maxima de subir: \033[92;1m$MAX_PROBABILITY\033[0m"
# Se calcula la probabilidad minima de subir
MIN_PROBABILITY=$(sort -n probabilities.txt | head -n 1)
echo -e "Probabilidad minima de subir: \033[92;1m$MIN_PROBABILITY\033[0m"
# Se calcula el porcentaje de materias que probablemente suban (75% >= probabilidad)
# Se cuenta la cantidad de materias que tienen una probabilidad mayor o igual a 75
PROBABLE_COURSES=$(awk '$1 >= 75' probabilities.txt | wc -l)
# Se calcula el porcentaje de materias que probablemente suban
PROBABLE_PERCENTAGE=$(echo "scale=2; $PROBABLE_COURSES / $(wc -l all_courses.txt | cut -d ' ' -f 1) * 100" | bc)
echo -e "Porcentaje de materias que probablemente suban (75% o mas de probabilidad): \033[92;1m$PROBABLE_PERCENTAGE%\033[0m"

# Se elimina el archivo de materias y probabilidades
rm all_courses.txt probabilities.txt