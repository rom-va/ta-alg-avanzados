#ifndef FUNCIONES_REPAIR_H
#define FUNCIONES_REPAIR_H

#include <vector>
using namespace std;
#include "estructuras_repair.h"
#include "estructuras_genetico_MKP.h"

int tendencyUtility(const TendencyParameters& tp, vector<int> chromosome, int cclass, int item, ItemClass classes[], int capacity[]);
void tendency(const TendencyParameters& tp, vector<int>& chromosome, ItemClass classes[], int capacity []);
bool RHTF(const TendencyParameters& tp, vector<int>& chromosome, ItemClass classes[], int capacity[]);
vector<TendencyParameters> GATF(const vector<Individual>& solutions, ItemClass classes[], int capacity []);
#endif /* FUNCIONES_REPAIR_H */

