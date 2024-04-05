# !/bin/bash

# Script que lee todos los codigos de las materias y realiza la busqueda de probabilidad de subir de cada una
# de las materias

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
if [ -f cars.txt ]; then
    rm cars.txt
fi

# Se recorren todos los codigos de las materias en all_courses.txt
while read LINE; do
    # Se ejecuta el programa pidecola.out con el codigo de la materia y los argumentos dados
    # Un ejemplo de la linea a a buscar es: "Para la asignatura \033[1;94mCI2511\033[0m se esperan \033[1;92m1.92\033[0m carros."
    # Siendo el formato de la linea: "Para la asignatura \033[1;94mCODIGO\033[0m se esperan \033[1;92mX.XX\033[0m carros."
    # Donde CODIGO es $LINE y X.XX la cantidad de carros esperados
    ./pidecola.out $LINE $@ | grep -oP "Para la asignatura \033\[1;94m$LINE\033\[0m se esperan \033\[1;92m[0-9]+\.[0-9]+\033\[0m carros." | grep -oP "[0-9]+\.[0-9]+" >> cars.txt
done < all_courses.txt

sort -n cars.txt > temp.txt
mv temp.txt cars.txt

# Se calcula el promedio de carros esperados
AVERAGE=$(awk '{s+=$1} END {print s/NR}' cars.txt)
echo -e "Promedio de carros esperados: \033[92;1m$AVERAGE\033[0m"
# Se calcula la cantidad maxima de carros esperados
MAX=$(tail -n 1 cars.txt)
echo -e "Maximo de carros esperados: \033[92;1m$MAX\033[0m"
# Se calcula la cantidad minima de carros esperados
MIN=$(head -n 1 cars.txt)
echo -e "Minimo de carros esperados: \033[92;1m$MIN\033[0m"

# Se elimina el archivo de materias y probabilidades
rm all_courses.txt cars.txt