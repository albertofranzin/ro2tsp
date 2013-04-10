#ifndef SOLVE_TSP_H_
#define SOLVE_TSP_H_

#include "graph.h"
#include "onetree.h"

void solve_tsp(graph* G, onetree* H, double* incumbent, int call_flag);
int is_cycle(onetree* OT);

#endif
