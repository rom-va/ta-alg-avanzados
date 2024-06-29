/* 
 * File:   main.cpp
 * Grupo: 
 * Algoritmo genético para el problema de la mochila multidimensional
 */

#include <cstdlib>
#include <iostream>
#include <vector>
#include <utility> 
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

// Parámetros del algoritmo genético
const int POPULATION_SIZE = 30; // Trabajamos con una población constante
const int NUM_GENERATIONS = 30;
const double MUTATION_RATE = 0.1;
//const double MATING_RATE = 0.5;
const double CROSSOVER_RATE = 0.8; // Invesión
//const int TOURNAMENT_SIZE = 3;
const int CUT_POINTS = 2;

const int NUM_CLASSES = 5;
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
    { { {7, {2, 1, 3}}, {14, {3, 2, 4}}, {16, {4, 3, 5}} } },
    { { {25, {5, 6, 7}}, {30, {6, 7, 8}}, {35, {7, 8, 9}} } },
    { { {18, {4, 5, 6}}, {22, {5, 6, 7}}, {28, {6, 7, 8}} } } 
};

// Contra estos comparamos los pesos de los items
int capacity[NUM_DIMENSIONS] = {10, 10, 10}; // Arreglo de "pesos" 

// Forma de guardar una solución o "individual"
struct Individual {
    vector<int> chromosome; // Cada elemento de chromosome corresponde a una clase de items
    int fitness;

    Individual(vector<int> chromo) : chromosome(chromo), fitness(0) {}
};

// Función "fitness" de una solución. Es la suma de los valores de los items
int fitness(const vector<int>& solution) {
    int total_value = 0;
    
    for (int i = 0; i < NUM_CLASSES; i++) { // Recorro las clases
        Item item = classes[i].items[solution[i]];
        total_value += item.value;
    }
    return total_value;
}

void evaluate_population(vector<Individual>& population) {
    int total_value = 0;
    
    for (Individual& individual : population) { // Modificamos los ind. de population
        individual.fitness = fitness(individual.chromosome);
    }    
}

void print_population(const vector<Individual>& population) {
    cout << "Población:" << endl;
    for (int k = 0; k < population.size(); k++) {
        cout << "Solución N°: "<< k+1 << " | ";
        cout << "Cromosoma: ";
        for (int i = 0; i < NUM_CLASSES; ++i) {
            cout << population[k].chromosome[i] << " ";
        }
        cout << "Fitness: " << population[k].fitness;
        cout << endl;
    }
}

void print_individual(const Individual &individual){
    cout << "Individuo:" << endl;    
    
    cout << "Cromosoma: ";
    for (int i = 0; i < NUM_CLASSES; ++i) {
        cout << individual.chromosome[i] << " ";
    }
    cout << "Fitness: " << individual.fitness;
    cout << endl;

}

vector<Individual> initial_population(){
    // Creamos la población inicial de manera randonómica
    // Pueden haber soluciones inválidas
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

void calculate_proportions(const vector<Individual> &population, vector<double> & proportions){
    int population_total_fitness = 0;
    
    for(int i=0; i < population.size(); i++)
        population_total_fitness += population[i].fitness;
    
    for(int i=0; i < population.size(); i++){
        double proportion = round( 100*(double) population[i].fitness / population_total_fitness );        
        proportions.push_back(proportion);
    }
}

void set_proportions(const  vector<double> & proportions, int roulette[]){
    int ind = 0;
    for(int i = 0; i < proportions.size(); i++){
        for(int j = 0; j < proportions[i]; j++){
            if (ind >= 100) break;
            roulette[ind]=i;
            ind++;
        }
    }
}

void proportional_selection(const vector<Individual> &population, vector<Individual> &parents){
    // Seleccionamos por medio de una ruleta con conveniencia
    int npadres, cont=0;
    vector<double> proportions;      
    int roulette[100]{-1};
    calculate_proportions(population, proportions);
    set_proportions(proportions, roulette);    
    npadres = 2;
     
    while(1){
        int ind = rand()%100;
        if (roulette[ind] > -1)
            parents.push_back(population[roulette[ind]]);
        cont++;
        if(cont >= npadres) break;
    }
}

Individual multi_cut_crossover(const Individual &parent1, const Individual &parent2){
    int chromosome_size = parent1.chromosome.size();
    vector<int> offspring(chromosome_size);

    // Genero los puntos de corte requeridos al azar
    vector<int> cut_points;
    for (int i = 0; i < CUT_POINTS; i++) {
        int cut_point = rand() % (chromosome_size - 1) + 1;
        cut_points.push_back(cut_point);
    }
    cut_points.push_back(chromosome_size-1); // ?
    sort(cut_points.begin(), cut_points.end()); // Ordeno los puntos de corte
    
    
    bool use_parent1 = true; // Flag para alternar entre los padres
    int last_cut = 0;
    for (int i = 0; i < cut_points.size(); i++) { // Recorro todos los puntos
        int current_cut = cut_points[i];
        if (use_parent1) {
            // Copia segmento del padre 1 al hijo
            for (int j = last_cut; j < current_cut; j++) {
                offspring[j] = parent1.chromosome[j];
            }
        } else {
            // Copia segmento del padre 2 al hijo
            for (int j = last_cut; j < current_cut; j++) {
                offspring[j] = parent2.chromosome[j];
            }
        }
        // Completamos el cromosoma solución
        if (i+1 == cut_points.size()){ // Último cutpoint
            if (use_parent1)
                offspring[chromosome_size - 1] = parent1.chromosome[chromosome_size - 1];            
            else
                offspring[chromosome_size - 1] = parent2.chromosome[chromosome_size - 1];            
        }
        
        use_parent1 = !use_parent1; // Alterna entre los padres
        last_cut = current_cut;
    }
    Individual off(offspring);
    off.fitness = fitness(offspring); 
    return off;
}

void genetic_algorithm() {
    vector<int> best_solution;
    int best_fitness = 0;

    // Inicializamos la población inicial (Randomly create initial population)
    vector<Individual> population = initial_population();
    
    // Comprobamos la población inicial
    evaluate_population(population);
    print_population(population);
    
    // Imprimimos mejor fitness
    auto best_individual = max_element(population.begin(), population.end(), [](Individual& a, Individual& b) {
        return a.fitness < b.fitness;
    });
    best_solution.push_back(best_individual->fitness);

    cout << "Poblacion inicial, best_fitness = " << best_individual->fitness << endl;
    
    // Generaciones (loop)
    for (int g = 0; g < NUM_GENERATIONS; ++g) {
        // Choose parent1 and parent2 from population by proportional selection
        vector<Individual> parents; // Elegimos dos padres                
        proportional_selection(population, parents);
        cout << "Padres elegidos: " << endl;
        print_population(parents);
        // Make offspring by recombining parent1 and parent2 using multi-cut circular crossover
        Individual offspring = multi_cut_crossover(parents[0], parents[1]);
        cout << "Hijo: " << endl;
        print_individual(offspring);
        
    }

    return;
}


int main(int argc, char** argv) {
    // Inicializamos la semilla del generador de números con el tiempo actual
    srand(time(0));
    
    // Llamamos al algoritmo genético que soluciona MKP
    genetic_algorithm();
    
    return 0;
}

