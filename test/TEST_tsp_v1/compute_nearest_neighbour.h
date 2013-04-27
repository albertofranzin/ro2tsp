#ifndef COMPUTE_NEAREST_NEIGHBOUR_H_
#define COMPUTE_NEAREST_NEIGHBOUR_H_

#include "graph.h"
#include "onetree.h"

// calcola un tour di G secondo l'euristica nearest-neighbour
// Hp: G, OT inizializzati
// Hp: 1 <= start_node <= n, dove n è il numero di nodi di G
double compute_nearest_neighbour(graph *G, onetree* OT, int start_node);

#endif
