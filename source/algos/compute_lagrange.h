#ifndef COMPUTE_LAGRANGE_H_
#define COMPUTE_LAGRANGE_H_

#include <stdlib.h>
#include <math.h>
#include "../base/constants.h"
#include "../data/graph.h"
#include "../data/edge.h"
#include "../data/elist.h"
#include "../data/onetree.h"

// calcola e ritorna un lower bound del costo di un tour ottimo su G cercando di risolvere il duale lagrangiano del problema TSP su G con un algoritmo subgradiente
// al termine della procedura sarà memorizzato in OT il miglior (di costo maggiore) 1-albero trovato 
// i principali parametri che regolano l'esecuzione dell'algoritmo sono ALPHA, MAX_N_ITERATIONS, MAX_N_ITERATIONS_NO_IMPROV, ALPHA_HALVING_TIME e sono specificati nel file constants.h
// Hp: G, OT inizializzati
// Hp: ub è un upper bound del costo ottimo di un tour su G

int compute_lagrange(graph* G, onetree* OT, double ub, double* lb);
int compute_incremental_lagrange(graph* G, onetree* OT, double ub, double* lb);

#endif
