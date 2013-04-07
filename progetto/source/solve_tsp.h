#ifndef SOLVE_TSP_H_
#define SOLVE_TSP_H_

#include <stdlib.h>
#include "graph.h"
#include "utils.h"

#define BIG 100000
#define SMALL -100000

void solve_tsp(graph* G, graph *F, graph* H, double* incumbent, int flag);
int is_cycle(graph* G);

#endif
