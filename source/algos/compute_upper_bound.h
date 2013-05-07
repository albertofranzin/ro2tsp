#ifndef COMPUTE_UPPER_BOUND_H_
#define COMPUTE_UPPER_BOUND_H_

#include "../data/graph.h"
#include "../data/onetree.h"

// calcola un upper bound del costo di un tour ottimo su G
// al termine della procedura sarà memorizzato in OT il miglior (di costo minore) tour trovato
// Hp: G, OT inizializzati
double compute_upper_bound(graph* G, onetree* OT, int algo);

// calcola un tour di G secondo l'euristica nearest-neighbour
// Hp: G, OT inizializzati
// Hp: 1 <= start_node <= n, dove n è il numero di nodi di G
double compute_nearest_neighbour(graph *G, onetree* OT, int start_node);

#endif
