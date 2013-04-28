#ifndef COMPUTE_MST_H_
#define COMPUTE_MST_H_

#include "graph.h"
#include "tree.h"
#include "list.h"

// calcola un albero ricoprente di costo minimo con radice r per il grafo G, e memorizza il riultato in T
// Hp: G, T inizializzati
// Hp: 1 <= r <= n dove n = numero nodi di G

void compute_mst(graph* G, tree* T, int r);

#endif
