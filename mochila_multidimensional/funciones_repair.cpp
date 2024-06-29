#include <cstdlib>
#include <cmath>
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
void RHTF(const TendencyParameters& tp, vector<int>& chromosome, ItemClass classes[], int capacity[]) {
    while (!is_valid_chromosome(chromosome, classes, capacity)) {
        tendency(tp, chromosome, classes, capacity); // Aplicar tendency hasta que el cromosoma sea viable
        Individual ind(chromosome);
        print_individual(ind);
    }
}
