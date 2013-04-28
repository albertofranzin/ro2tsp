#ifndef COMPUTE_OT_H_
#define COMPUTE_OT_H_

#include "graph.h"
#include "onetree.h"
#include "list.h"

// calcola l'1-albero di G di costo minimo e memorizza il risultato in OT
// Hp: G, OT inizializzati
// Hp: n > 2 dove n è il numero di nodi di G
void compute_ot(graph* G, onetree* OT);

#endif
