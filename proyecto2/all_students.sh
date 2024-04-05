# !/bin/bash

# Script que lee todos los carnets de los estudiantes y realiza la busqueda de probabilidad de subir de cada uno
# de los estudiantes

# Si solo hay un parametro y es -h o --help, se imprime la ayuda
if [ $# -eq 1 ] && ([ "$1" == "-h" ] || [ "$1" == "--help" ]); then
    echo -e "\033[1;93mUso:\033[0m ./all_students.sh [Opciones]"
    echo -e "\033[1;93mOpciones:\033[0m"
    echo -e "\033[93m  -h, --help:\033[0m Muestra la ayuda"
    echo -e "\033[93m  -d <path>:\033[0m Directorio raiz de los comprobantes"
    echo -e "\033[93m  -c, --cohorte <cohorte>:\033[0m Ultima cohorte de la que se tiene informacion"
    echo -e "\033[93m  -p, --pcohorte <probabilidad>:\033[0m Probabilidad de tener carro de la ultima cohorte"
    echo -e "\033[93m  -i, --incremento <incremento>:\033[0m Incremento de la probabilidad de tener carro por cohorte"
    exit 0
fi

# Se revisa si el ejecutable pidecola.out existe. En caso de no existir, se imprime un mensaje de error
if [ ! -f pidecola.out ]; then
    echo "\033[91;1mError:\033[0m No se encontro el ejecutable pidecola.out"
    exit 1
fi

# Se revisa que la cantidad de argumentos es par o igual a 0 y menor o igual a 6. En caso de no serlo, se imprime un mensaje de error
if [ $(( $# % 2 )) -ne 0 ] || [ $# -gt 6 ]; then
    echo -e "\033[91;1mError:\033[0m Cantidad de argumentos invalida"
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

# Se crea un archivo temporal llamado all_students.txt. En caso de que ya exista, se elimina
if [ -f all_students.txt ]; then
    rm all_students.txt
fi

# Se recorren las subcarpetas de la carpeta de ROOT_PATH (son dos subcarpetas: Sartenejas y Litoral)
for CAMPUS in $(ls $ROOT_PATH); do
    # Se hace ls a todos los directorios en las carpetas ROOT_PATH/CAMPUS/comprobantes/ y se copian 
    # las primeras lineas de cada archivo en temp.txt (Eliminano el salto de linea al final de cada linea)
    for COHORTE in $(ls $ROOT_PATH/$CAMPUS/comprobantes/); do
        for FILE in $(ls $ROOT_PATH/$CAMPUS/comprobantes/$COHORTE); do
            head -n 1 $ROOT_PATH/$CAMPUS/comprobantes/$COHORTE/$FILE >> temp.txt
        done
    done
done

# Se ordena el archivo en all_students.txt y se eliminan las lineas duplicadas
sort temp.txt | uniq > all_students.txt
# Se elimina el archivo temporal
rm temp.txt

# Se imprime la cantidad de estudiantes
echo -e "Cantidad de estudiantes: \033[92;1m$(wc -l all_students.txt | cut -d ' ' -f 1)\033[0m"

# Se elimina el archivo de probabilidades si ya existe
if [ -f probabilities.txt ]; then
    rm probabilities.txt
fi

# Se recorren todos los estudiantes en all_students.txt
while read LINE; do
    # Se ejecuta el programa pidecola.out con el carnet del estudiante y los argumentos dados
    # Un ejemplo de la linea a a buscar es: "Probabilidad total de asistencia: \033[1;92mX.XX%\033[0m"
    # Siendo el formato de la linea: "Probabilidad total de asistencia: \033[1;92mX.XX%\033[0m"
    ./pidecola.out $LINE $@ | grep "Probabilidad total de asistencia: " | grep -o "[0-9]*\.[0-9]*" >> probabilities.txt
done < all_students.txt

cat probabilities.txt | sort -n > temp.txt
mv temp.txt probabilities.txt

# Se calcula la probabilidad promedio de subir
AVERAGE_PROBABILITY=$(awk '{s+=$1} END {print s/NR}' probabilities.txt)
echo -e "Probabilidad promedio de subir: \033[92;1m$AVERAGE_PROBABILITY%\033[0m"
# Se calcula la probabilidad maxima de subir
MAX_PROBABILITY=$(tail -n 1 probabilities.txt)
echo -e "Probabilidad maxima de subir: \033[92;1m$MAX_PROBABILITY%\033[0m"
# Se calcula la probabilidad minima de subir
MIN_PROBABILITY=$(head -n 1 probabilities.txt)
echo -e "Probabilidad minima de subir: \033[92;1m$MIN_PROBABILITY%\033[0m"
# Se calcula el porcentaje de estudiantes que subiran (75% o mas de probabilidad)
STUDENTS_UP75=$(awk '{if ($1 >= 75) print $1}' probabilities.txt | wc -l)
PERCENTAGE_UP75=$(echo "scale=2; $STUDENTS_UP75 / $(wc -l all_students.txt | cut -d ' ' -f 1) * 100" | bc)
echo -e "Porcentaje de estudiantes que subiran (75% o mas de probabilidad): \033[92;1m$PERCENTAGE_UP75%\033[0m"

# Se elimina el archivo de probabilidades y el archivo de estudiantes
rm probabilities.txt all_students.txt
