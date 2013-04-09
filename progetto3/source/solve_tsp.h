#ifndef SOLVE_TSP_H_
#define SOLVE_TSP_H_

#include "graph.h"
#include "onetree.h"

#define BIG 2000
#define SMALL -2000

void solve_tsp(graph* G, onetree* H, double* incumbent, int flag, double ub, int ub_max_iter, double alpha, int alpha_max_iter, int max_iter);
int is_cycle(onetree* OT);

#endif
