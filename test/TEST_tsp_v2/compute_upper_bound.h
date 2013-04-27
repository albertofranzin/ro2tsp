#ifndef COMPUTE_UPPER_BOUND_H_
#define COMPUTE_UPPER_BOUND_H_

#include "graph.h"
#include "onetree.h"
#include "compute_nearest_neighbour.h"

// calcola un upper bound del costo di un tour ottimo su G
// al termine della procedura sarà memorizzato in OT il miglior (di costo minore) tour trovato
// Hp: G, OT inizializzati
double compute_upper_bound(graph *G, onetree* OT);

#endif
