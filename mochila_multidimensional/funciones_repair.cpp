#include <cstdlib>
#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;

#include "funciones_repair.h"
#include "funciones_genetico_MKP.h"

// Función para calcular la tendency utility de un item 
int tendencyUtility(const TendencyParameters& tp, vector<int> chromosome, int cclass, int item, ItemClass classes[], int capacity[]) { // indices de clase y item
    
    int res = 0; // Acumulador
    Item cur = classes[cclass].items[chromosome[cclass]]; // Ítem actual (current) del cromo
    int m = cur.weights.size(); // Num. de dimensiones (pesos a comprobar)

    for (int k = 0; k < m; ++k) { // Itero las dimensiones
        int delta_wc_cur = capacity[k] - cur.weights[k]; // Δwc, current
        int delta_wc_r = capacity[k] - classes[cclass].items[item].weights[k]; // Δwc, new item

        if (delta_wc_cur < 0 && delta_wc_r < 0) {
            if (delta_wc_cur >= delta_wc_r) {
                res += tp.mmi * abs(delta_wc_cur - delta_wc_r);
            } else {
                res += tp.mmd * abs(delta_wc_cur - delta_wc_r);
            }
        } else if (delta_wc_cur > 0 && delta_wc_r > 0) {
            if (delta_wc_cur >= delta_wc_r) {
                res += tp.ppi * abs(delta_wc_cur - delta_wc_r);
            } else {
                res += tp.ppd * abs(delta_wc_cur - delta_wc_r);
            }
        } else if (delta_wc_cur < 0 && delta_wc_r > 0) {
            res += tp.mp * abs(delta_wc_cur - delta_wc_r);
        } else if (delta_wc_cur > 0 && delta_wc_r < 0) {
            res += tp.pm * abs(delta_wc_cur - delta_wc_r);
        }
    }
    return res;
}

// Función Tendency
void tendency(const TendencyParameters& tp, vector<int>& chromosome, ItemClass classes[], int capacity []) {    
    if (!is_valid_chromosome(chromosome, classes, capacity)) {
        int cmax = -1, rmax = -1; // Ubicación del item com mayor utilidad
        int max_utility = -1;

        // Iterar sobre todas las clases y todos los ítems para encontrar la mejor utilidad de tendencia
        for (int c = 0; c < NUM_CLASSES; ++c) {
            for (int r = 0; r < NUM_ITEMS_PER_CLASS; ++r) {
                // Calcular la utilidad de tendencia para el item r en la clase c
                int utility = tendencyUtility(tp, chromosome, c, r, classes, capacity);

                // Guardamos la mejor utilidad
                if (utility > max_utility) {
                    max_utility = utility;
                    cmax = c;
                    rmax = r;
                }
            }
        }

        // Cambiar un item en el cromosoma que maximice la utilidad de tendencia
        if (cmax != -1 && rmax != -1) {
            chromosome[cmax] = rmax;
        }
    }
}

// Función para reparar una solución inválida con la función de tendencia
bool RHTF(const TendencyParameters& tp, vector<int>& chromosome, ItemClass classes[], int capacity[]) {
//    cout << "-------------------------------------------------------------" << endl;
    for (int i = 0; i < 30; i++) {
        tendency(tp, chromosome, classes, capacity); // Modificamos el cromosoma
//        Individual ind(chromosome);
//        print_individual(ind);
        if (is_valid_chromosome(chromosome, classes, capacity)) return true;
    }
    return false; // No se logró reparar
}

// Algoritmo genético para aproximar los parámetros de tendencia - GAFT para una solución
vector<TendencyParameters> GATF(const vector<Individual>& solutions, ItemClass classes[], int capacity []) {
    // Generamos de manera aleatoria la población inicial   
    vector<Individual> tendency_population = create_initial_tendency_population(100);
    print_population(tendency_population);
    evaluate_tendency_population(tendency_population, classes, capacity, solutions);
    print_population(tendency_population);
    
    double best_fitness = 0;
    vector<int> best_solution;
    
    for (int g = 0; g < NUM_TENDENCY_GENERATIONS; g++) {   
        // Elegimos los padres por medio de selección proporcional
        vector<Individual> parents;
        proportional_selection_parents(tendency_population, parents);
        cout << "Padres: " << endl;
        print_population(parents);
        // Generamos el offspring por uniform crossover
        Individual offspring = uniform_crossover(parents[0], parents[1]);
        cout << "Hijo: " << endl;
        print_individual(offspring);
        
        // Mutate offspring by swapping randomly chosen two genes
        mutate_swap_2_genes(offspring);
        cout << "Hijo mutado: "<< endl;
        print_individual(offspring);
        
        // Get the feasibility of offspring from RHTF(offspring, given solution s) in Algorithm
        offspring.fitness = fitness_tendency(offspring.chromosome, classes, capacity, solutions);
        cout << "Hijo mutado con fitness calculado: " << endl;
        print_individual(offspring);
        
        // Replace an individual in the population with offspring
        int individual_to_replace = proportional_selection_replace(tendency_population);
        tendency_population.erase(tendency_population.begin() + individual_to_replace);
        tendency_population.push_back(offspring);    
        cout << "Nueva población:" << endl;
        print_population(tendency_population);
        
        // Guardo la mejor solución        
        auto best_individual = max_element(tendency_population.begin(), tendency_population.end(), [](Individual& a, Individual& b) {
            return a.fitness < b.fitness;
        });      
        cout << "Poblacion actual, best_fitness = " << best_individual->fitness << endl;
        
        if (best_individual->fitness > best_fitness){
            best_fitness = best_individual->fitness;
            best_solution = best_individual->chromosome;
        }
    }
    
    cout << "Mejor solucion del ejercicio: ";
    for (int i = 0; i < best_solution.size(); i++) {
        cout << best_solution[i] << " ";
    }
    cout <<  " Fitness: " << best_fitness << endl;
    
    // Conversión a TendencyParameters
    vector<TendencyParameters> vtp;    
    
    for (int i = 0; i < tendency_population.size(); i++) {
        Individual ind = tendency_population[i];
        TendencyParameters tp(ind.chromosome[0], ind.chromosome[1],ind.chromosome[2],ind.chromosome[3],
                ind.chromosome[4],ind.chromosome[5]);
        vtp.push_back(tp);
    }

    return vtp;
}
