#ifndef SOLVE_TSP_H_
#define SOLVE_TSP_H_

#include "graph.h"
#include "onetree.h"
#include "edgelist.h"

void solve_tsp(graph* G, onetree* H, double* incumbent, onetree* ONE_TREE, double z, int call_flag);
int is_cycle(onetree* OT);
int propagate_constraints(graph* G, edgelist* new_edges, edgelist* old_edges);
#endif
