#include "graph.h"

#ifndef SOLVE_TSP_H_
#define SOLVE_TSP_H_

void solve_tsp(graph* G, graph* H, double* incumbent, int flag);
int is_cycle(graph* G);

#endif
