#ifndef COMPUTE_MST_H_
#define COMPUTE_MST_H_

#include "../data/graph.h"
#include "../data/tree.h"
#include "../data/list.h"

// calcola un albero ricoprente di costo minimo con radice r per il grafo G, e memorizza il riultato in T
// Hp: G, T inizializzati
// Hp: 1 <= r <= n dove n = numero nodi di G

int compute_mst(graph* G, tree* T, int root);

#endif
