/* 
 * File:   main.cpp
 * Grupo: 
 * Algoritmo genético para el problema de la mochila multidimensional
 */

#include <cstdlib>
#include <iostream>
#include <ctime>
#include "funciones_repair.h"
#include "funciones_genetico_MKP.h"

using namespace std;

int main(int argc, char** argv) {
    // Inicializamos la semilla del generador de números con el tiempo actual
    srand(time(0));    
    
    // Utilizamos tres clases, cada una con tres items
    ItemClass classes[NUM_CLASSES] = { // Clases con sus respectivos items
        { { {10, {2, 3, 4}}, {15, {3, 4, 5}}, {20, {4, 5, 6}} } },
        { { {5, {1, 2, 2}}, {8, {2, 3, 3}}, {12, {3, 4, 4}} } },
        { { {7, {2, 1, 3}}, {14, {3, 2, 4}}, {16, {4, 3, 5}} } },
        { { {25, {5, 6, 7}}, {30, {6, 7, 8}}, {35, {7, 8, 9}} } },
        { { {18, {4, 5, 6}}, {22, {5, 6, 7}}, {28, {6, 7, 8}} } } 
    };

    // Contra estos comparamos los pesos de los items
    int capacity[NUM_DIMENSIONS] = {25, 25, 25}; // Arreglo de "pesos" 

    // Llamamos al algoritmo genético que soluciona MKP
    genetic_algorithm(classes, capacity);    
    
    // Pruebas de reparación
//    TendencyParameters initial_tendency_parameters(1, 1, 1, 1, 1, 1);
//    vector<int> chromosome = {0, 1, 2, 1, 2};
//    RHTF(initial_tendency_parameters, chromosome);
//    Individual indi(chromosome);
//    print_individual(indi);
    
    // Pruebas de genético para tendency parameters
    
    return 0;
}