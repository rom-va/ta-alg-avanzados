#include <cstdlib>
#include <iostream>
#include <utility> 
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

#include "funciones_genetico_MKP.h"
#include "estructuras_repair.h"
#include "funciones_repair.h"

// Función "fitness" de una solución. Es la suma de los valores de los items
int fitness(const vector<int>& solution, ItemClass classes[]) {
    int total_value = 0;
    
    for (int i = 0; i < NUM_CLASSES; i++) { // Recorro las clases
        Item item = classes[i].items[solution[i]];
        total_value += item.value;
    }
    return total_value;
}

void evaluate_population(vector<Individual>& population, ItemClass classes[], int capacity []) {
    int total_value = 0;
    
    for (Individual& individual : population) { // Modificamos los ind. de population
        individual.fitness = fitness(individual.chromosome, classes);
        if (!is_valid_chromosome(individual.chromosome, classes, capacity)){
            individual.fitness = -1;
        }
    }
}

void print_population(const vector<Individual>& population) {
    cout << "Población:" << endl;
    for (int k = 0; k < population.size(); k++) {
        cout << "Solución N°: "<< k+1 << " | ";
        cout << "Cromosoma: ";
        for (int i = 0; i < population[0].chromosome.size(); ++i) {
            cout << population[k].chromosome[i] << " ";
        }
        cout << "Fitness: " << population[k].fitness;
        cout << endl;
    }
}

void print_individual(const Individual &individual){
    cout << "Individuo:" << endl;    
    
    cout << "Cromosoma: ";
    for (int i = 0; i < individual.chromosome.size(); ++i) {
        cout << individual.chromosome[i] << " ";
    }
    cout << "Fitness: " << individual.fitness;
    cout << endl;

}

bool is_valid_chromosome(const vector<int> &chromosome, ItemClass classes[], int capacity[]){    
    vector<int> total_weights(NUM_DIMENSIONS, 0);  // Inicializamos en cero los pesos
    
    for (int i = 0; i < NUM_CLASSES; i++) {
        Item item = classes[i].items[chromosome[i]];
        
        for (int j = 0; j < NUM_DIMENSIONS; j++) {
            total_weights[j] += item.weights[j];
        }
    }
    
    for (int j = 0; j < NUM_DIMENSIONS; j++) {
        if (total_weights[j] > capacity[j]) {
            return false;
        }
    }
    return true;
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
//        if (is_valid_chromosome(chromo, classes, capacity))
            population.push_back(Individual(chromo)); // Usamos constructor
//        else
//            i--;
    }
    
    return population;
}

void calculate_proportions(const vector<Individual> &population, vector<double> & proportions){
    int population_total_fitness = 0;
    
    for(int i=0; i < population.size(); i++)
        population_total_fitness += population[i].fitness;
    
    for(int i=0; i < population.size(); i++){
        double proportion = round( 100*(double) population[i].fitness / population_total_fitness );        
        if (population[i].fitness == 0){
            // Si su fitnes = 0, le colocamos una proporción de 1
            proportion = 1;
        }
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

void calculate_reverse_proportions(const vector<Individual> &population, vector<double> & proportions){
    int population_total_fitness = 0;
    
    for(int i=0; i < population.size(); i++)
        population_total_fitness += population[i].fitness;
    
    for(int i=0; i < population.size(); i++){
        double proportion = round( 100*(double) ( 1 - population[i].fitness / population_total_fitness ) );        
        proportions.push_back(proportion);
    }
}

int proportional_selection_replace(const vector<Individual> &population){ // Devuelve index del individuo a reemplazar
    
    vector<double> proportions;      
    int roulette[100]{-1};
    calculate_reverse_proportions(population, proportions);
    set_proportions(proportions, roulette);    
    
    int index = rand()%100;
    while (roulette[index] <= -1)
        index = rand()%100;
    
    return roulette[index];    
}


void proportional_selection_parents(const vector<Individual> &population, vector<Individual> &parents){
    parents.erase(parents.begin(), parents.end());
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

Individual multi_cut_crossover(const Individual &parent1, const Individual &parent2, ItemClass classes[]){
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
    off.fitness = fitness(offspring, classes); 
    return off;
}

void mutate_individual(Individual & offspring, ItemClass classes[]){
    int cont = 0;
    int chromo_size = offspring.chromosome.size();
    int num_mutated_genes = round(chromo_size * MUTATION_RATE);
   
    while(cont < num_mutated_genes){ // Muto cierto número de genes del cromosoma
        int random_index = rand() % chromo_size;
        int old_item = offspring.chromosome[random_index];
        
        while (old_item == offspring.chromosome[random_index]){ // Nuevo item
            offspring.chromosome[random_index] = rand() % NUM_ITEMS_PER_CLASS;
        }      
        cont++;
    }    
    offspring.fitness = fitness(offspring.chromosome, classes);
}

int best_individual(const vector<Individual> &population){ // Índice de mejor individuo
    int best_fitness = 0;
    int best_individual = 0;
    for (int i = 0; i < population.size(); i++) {
        if (population[i].fitness > best_fitness){
            best_fitness = population[i].fitness;
            best_individual = i;
        }
    }
    return best_individual;
}

void genetic_algorithm(ItemClass classes[], int capacity[]) {
    vector<int> best_solution;
    int best_fitness = 0;

    // Inicializamos la población inicial (Randomly create initial population)
    vector<Individual> population = initial_population();
    
    // Comprobamos la población inicial
    evaluate_population(population, classes, capacity);
    print_population(population);
    
    // Imprimimos mejor fitness
//    auto best_individual = max_element(population.begin(), population.end(), [](Individual& a, Individual& b) {
//        return a.fitness < b.fitness;
//    });
//    cout << "Poblacion inicial, best_fitness = " << best_individual->fitness << endl;
    
    // GAFT para generar los parámetros de tendencia
    vector<TendencyParameters> vtp = GATF(population, classes, capacity);
    
    // Generaciones (loop)
    for (int g = 0; g < NUM_GENERATIONS; ++g) {
        // Choose parent1 and parent2 from population by proportional selection
        vector<Individual> parents; // Elegimos dos padres                
        proportional_selection_parents(population, parents);
        cout << "Padres elegidos: " << endl;
        print_population(parents);
        // Make offspring by recombining parent1 and parent2 using multi-cut circular crossover
        Individual offspring = multi_cut_crossover(parents[0], parents[1], classes);
        cout << "Hijo: " << endl;
        print_individual(offspring);
        // Mutate offspring by changing randomly chosen t genes;
        mutate_individual(offspring, classes);
        cout << "Hijo mutado: " << endl;
        print_individual(offspring);
        // Repair offspring using GATF(offspring) in Algorithm
        bool repaired_flag = false;
        for (int k = 0; k < vtp.size(); k++) {
            if (RHTF(vtp[k], offspring.chromosome, classes, capacity)){
                cout << "Hijo REPARADO" << endl;
                repaired_flag = true;
                break;
            }
        }
        offspring.fitness = fitness(offspring.chromosome,classes);
        print_individual(offspring);
        
        // Los siguientes pasos solo son aplicados a soluciones válidas
        if (repaired_flag){
            // Replace an individual in the population with offspring;        
            int individual_to_replace = proportional_selection_replace(population);
            population.erase(population.begin() + individual_to_replace);
            population.push_back(offspring);

            print_population(population);

            // Guardo la mejor solución        
            auto best_individual = max_element(population.begin(), population.end(), [](Individual& a, Individual& b) {
                return a.fitness < b.fitness;
            });      
            cout << "Poblacion actual, best_fitness = " << best_individual->fitness << endl;

            if (best_individual->fitness > best_fitness){
                best_fitness = best_individual->fitness;
                best_solution = best_individual->chromosome;
            } 
        }       
    }

    cout << "Mejor solucion del ejercicio: ";
    for (int i = 0; i < best_solution.size(); i++) {
        cout << best_solution[i] << " ";
    }
    cout <<  " Fitness: " << best_fitness << endl;
    return;
}

vector<Individual> create_initial_tendency_population(int n){ // n: rango 
    // Creamos la población inicial de manera randonómica
    vector<Individual> t_population;  
    
    for (int i = 0; i < TENDENCY_POPULATION_SIZE; i++) {
        vector<int> chromosome(NUM_TENDENCY_PARAMETERS);
        for (int j = 0; j < NUM_TENDENCY_PARAMETERS; j++) {            
            chromosome[j] = rand() % n; 
        }
        t_population.push_back(Individual(chromosome)); // Usamos constructor
    }
    
    return t_population;
}

double fitness_tendency(const vector<int> & chromosome, ItemClass classes[], int capacity[], 
        vector<Individual> solutions){
    // Evalúo cuántas soluciones los parámetros del cromosoma convierten en válidas    
    int cont = 0;
    
    // Armo TendencyParameters
    TendencyParameters parameters(chromosome[0], chromosome[1], chromosome[2],chromosome[3], chromosome[4],
            chromosome[5]);
    for (int i = 0; i < solutions.size(); i++) {        
        if (RHTF(parameters, solutions[i].chromosome, classes, capacity)){
            cont++;
        }
    }
    return (double) cont/solutions.size();    
}

void evaluate_tendency_population(vector<Individual> & tendency_population, 
        ItemClass classes [], int capacity [], const vector<Individual> & solutions){
    int total_value = 0;
    
    for (Individual& individual : tendency_population) { // Modificamos los ind. de population
        individual.fitness = fitness_tendency(individual.chromosome, classes, capacity, solutions);
    }    
}

Individual uniform_crossover(const Individual & parent1, 
        const Individual & parent2) {
    
    int chromosome_size = parent1.chromosome.size();
    vector<int> offspring(chromosome_size);

    // Uniform crossover
    for (int i = 0; i < chromosome_size; i++) {
        if (rand() % 2 == 0) {
            offspring[i] = parent1.chromosome[i]; 
        } else {
            offspring[i] = parent2.chromosome[i];
        }
    }
    return Individual(offspring);
}

void mutate_swap_2_genes(Individual & offspring){
    int chromosome_size = offspring.chromosome.size();

    // Selección de dos posiciones distintas
    int pos1 = rand() % chromosome_size;
    int pos2 = rand() % chromosome_size;
    
    while (pos1 == pos2) 
        pos2 = rand() % chromosome_size;
    
    // Swap
    int aux = offspring.chromosome[pos1];
    offspring.chromosome[pos1] = offspring.chromosome[pos2];
    offspring.chromosome[pos2] = aux;    
}