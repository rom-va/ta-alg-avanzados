#ifndef FUNCIONES_GENETICO_MKP_H
#define FUNCIONES_GENETICO_MKP_H

#include "estructuras_genetico_MKP.h"

int fitness(const vector<int>& solution, ItemClass classes[]);
void evaluate_population(vector<Individual>& population, ItemClass classes[], int capacity []);
void print_population(const vector<Individual>& population);
void print_individual(const Individual &individual);
bool is_valid_chromosome(const vector<int> &chromosome, ItemClass classes[], int capacity[]);
vector<Individual> initial_population();
void calculate_proportions(const vector<Individual> &population, vector<double> & proportions);
void set_proportions(const  vector<double> & proportions, int roulette[]);
void calculate_reverse_proportions(const vector<Individual> &population, vector<double> & proportions);
int proportional_selection_replace(const vector<Individual> &population);
void proportional_selection_parents(const vector<Individual> &population, vector<Individual> &parents);
Individual multi_cut_crossover(const Individual &parent1, const Individual &parent2, ItemClass classes[]);
void mutate_individual(Individual & offspring, ItemClass classes[]);
int best_individual(const vector<Individual> &population);
void genetic_algorithm(ItemClass classes[], int capacity[]);
vector<Individual> create_initial_tendency_population(int n);
void evaluate_tendency_population(vector<Individual> & tendency_population, 
        ItemClass classes [], int capacity [], const vector<Individual> & solutions);
double fitness_tendency(const vector<int> & chromosome, ItemClass classes[], int capacity[], 
        vector<Individual> solutions);
Individual uniform_crossover(const Individual & parent1, 
        const Individual & parent2);
void mutate_swap_2_genes(Individual & offspring);
#endif /* FUNCIONES_GENETICO_MKP_H */

