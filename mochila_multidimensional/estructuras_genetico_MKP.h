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
struct Individual { // También usado para tendency parameters
    vector<int> chromosome; // Cada elemento de chromosome corresponde a una clase de items
    double fitness;

    Individual(vector<int> chromo) : chromosome(chromo), fitness(0) {}
};

// Parámetros del algoritmo genético
const int POPULATION_SIZE = 10; // Trabajamos con una población constante
const int TENDENCY_POPULATION_SIZE = 10;
const int NUM_TENDENCY_PARAMETERS = 6;
const int NUM_GENERATIONS = 100;
const int NUM_TENDENCY_GENERATIONS = 50;
const double MUTATION_RATE = 0.3;
//const double MATING_RATE = 0.5;
//const double CROSSOVER_RATE = 0.8; // Invesión
//const int TOURNAMENT_SIZE = 3;
const int CUT_POINTS = 2;

const int NUM_CLASSES = 10;
const int NUM_ITEMS_PER_CLASS = 3;
const int NUM_DIMENSIONS = 3; // Número de restricciones de capacidad 

#endif /* ESTRUCTURAS_GENETICO_MKP_H */

