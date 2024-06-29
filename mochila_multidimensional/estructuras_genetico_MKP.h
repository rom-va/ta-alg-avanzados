#ifndef ESTRUCTURAS_GENETICO_MKP_H
#define ESTRUCTURAS_GENETICO_MKP_H

#include <vector>
using namespace std;

struct Item { // Items 
    int value;
    vector<int> weights;
};

struct ItemClass { // Clases de items
    vector<Item> items;
};

// Forma de guardar una solución o "individual"
struct Individual {
    vector<int> chromosome; // Cada elemento de chromosome corresponde a una clase de items
    int fitness;

    Individual(vector<int> chromo) : chromosome(chromo), fitness(1) {}
};

// Parámetros del algoritmo genético
const int POPULATION_SIZE = 5; // Trabajamos con una población constante
const int NUM_GENERATIONS = 30;
const double MUTATION_RATE = 0.3;
//const double MATING_RATE = 0.5;
const double CROSSOVER_RATE = 0.8; // Invesión
//const int TOURNAMENT_SIZE = 3;
const int CUT_POINTS = 2;

const int NUM_CLASSES = 5;
const int NUM_ITEMS_PER_CLASS = 3;
const int NUM_DIMENSIONS = 3; // Número de restricciones de capacidad 

#endif /* ESTRUCTURAS_GENETICO_MKP_H */

