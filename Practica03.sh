#!/bin/bash

#Uso: ./Practica01.sh

#Guardamos los directorios necesarios
soluciones=$(pwd)/result
inicio = $(pwd)

#Colores para los mensajes por terminal
nocolor='\033[0m'
color='\033[0;34m'

echo -e "\n${color}Limpiando...\n${nocolor}"
rm -f $soluciones/*.txt

#Compila el codigo
echo -e "\n${color}Compilando en Codigo...\n${nocolor}"
make

#Se lanzan las ejecuciones de la Practica 01
#Los resultados se almacenan en 'sol'
echo -e "\n${color}Ejecutando CDOA para Solis Wets dimension 10...\n${nocolor}"
$inicio/main_CDOA 1 0 10 >> $soluciones/"SW_10.txt"
echo -e "\n${color}Ejecutando CDOA para Simplex dimension 10...\n${nocolor}"
$inicio/main_CDOA 2 0 10 >> $soluciones/"SX_10.txt"
echo -e "\n${color}Ejecutando CDOA para CMAES dimension 10...\n${nocolor}"
$inicio/main_CDOA 3 0 10 >> $soluciones/"CMAES_10.txt"

echo -e "\n${color}Ejecutando CDOA para Solis Wets dimension 30...\n${nocolor}"
$inicio/main_CDOA 1 0 30 >> $soluciones/"SW_30.txt"
echo -e "\n${color}Ejecutando CDOA para Simplex dimension 30...\n${nocolor}"
$inicio/main_CDOA 2 0 30 >> $soluciones/"SX_30.txt"
echo -e "\n${color}Ejecutando CDOA para CMAES dimension 30...\n${nocolor}"
$inicio/main_CDOA 3 0 30 >> $soluciones/"CMAES_30.txt"

echo -e "\n${color}Ejecutando CDOA para Solis Wets dimension 50...\n${nocolor}"
$inicio/main_CDOA 1 0 50 >> $soluciones/"SW_50.txt"
echo -e "\n${color}Ejecutando CDOA para Simplex dimension 50...\n${nocolor}"
$inicio/main_CDOA 2 0 50 >> $soluciones/"SX_50.txt"
echo -e "\n${color}Ejecutando CDOA para CMAES dimension 50...\n${nocolor}"
$inicio/main_CDOA 3 0 50 >> $soluciones/"CMAES_50.txt"
