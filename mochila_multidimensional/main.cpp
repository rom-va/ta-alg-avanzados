/* 
 * File:   main.cpp
 * Grupo: 
 * Algoritmo genético para el problema de la mochila multidimensional
 */

#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

// Parámetros del algoritmo genético
const int POPULATION_SIZE = 30; // Trabajamos con una población constante
const int NUM_GENERATIONS = 30;
const double MUTATION_RATE = 0.1;
const double CROSSOVER_RATE = 0.8; // Invesión

const int NUM_CLASSES = 3;
const int NUM_ITEMS_PER_CLASS = 3;
const int NUM_DIMENSIONS = 3; // Número de restricciones de capacidad 

struct Item { // Items 
    int value;
    vector<int> weights;
};

struct ItemClass { // Clases de items
    vector<Item> items;
};

// Utilizamos tres clases, cada una con tres items
ItemClass classes[NUM_CLASSES] = { // Clases con sus respectivos items
    { { {10, {2, 3, 4}}, {15, {3, 4, 5}}, {20, {4, 5, 6}} } },
    { { {5, {1, 2, 2}}, {8, {2, 3, 3}}, {12, {3, 4, 4}} } },
    { { {7, {2, 1, 3}}, {14, {3, 2, 4}}, {16, {4, 3, 5}} } }
};

// Forma de guardar una solución o "individual"
struct Individual {
    vector<int> chromosome; // Cada elemento de chromosome corresponde a una clase de items
    int fitness;

    Individual(vector<int> chromo) : chromosome(chromo), fitness(0) {}
};

// Contra estos comparamos los pesos de los items
int capacity[NUM_DIMENSIONS] = {10, 10, 10}; // Arreglo de "pesos" 

// Función "fitness" de una solución. Es la suma de los valores de los items
int fitness(const vector<int>& solution) {
    int total_value = 0;
    
    for (int i = 0; i < NUM_CLASSES; i++) { // Recorro las clases
        Item item = classes[i].items[solution[i]];
        total_value += item.value;
    }
    return total_value;
}

void print_population(const vector<Individual>& population) {
    cout << "Población:" << endl;
    for (int k = 0; k < POPULATION_SIZE; k++) {
        cout << "Solución N°: "<< k+1 << " | ";
        cout << "Cromosoma: ";
        for (int i = 0; i < NUM_CLASSES; ++i) {
            cout << population[k].chromosome[i] << " ";
        }
        cout << "Fitness: " << population[k].fitness;
        cout << endl;
    }
}
vector<Individual> initial_population(){
    // Creamos la población inicial de manera randonómica
    vector<Individual> population;
    
    for (int i = 0; i < POPULATION_SIZE; i++) {
        vector<int> chromo(NUM_CLASSES);
        for (int j = 0; j < NUM_CLASSES; j++) {
            // Elijo aleatoriamente un item por clase de items
            chromo[j] = rand() % NUM_ITEMS_PER_CLASS; 
        }        
        population.push_back(Individual(chromo)); // Usamos constructor
    }
    
    return population;
}

void genetic_algorithm() {
    vector<int> best_solution;
    int best_fitness = 0;

    // Inicializamos la población inicial
    vector<Individual> population = initial_population();
    
    // Comprobamos la población inicial
    print_population(population);
    
    

    return;
}


int main(int argc, char** argv) {
    // Inicializamos la semilla del generador de números con el tiempo actual
    srand(time(0));
    
    // Llamamos al algoritmo genético que soluciona MKP
    genetic_algorithm();
    
    return 0;
}

