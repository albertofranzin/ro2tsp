#ifndef SOLVE_TSP_H_
#define SOLVE_TSP_H_

#include "../base/constants.h"
#include "../data/graph.h"
#include "../data/onetree.h"
#include "../data/edgelist.h"
#include <stdio.h>
#include <stdlib.h>
#include "../data/edge.h"
#include "../data/edgelist.h"
#include <string.h>
#include "../algos/compute_lagrange.h"
#include "../algos/compute_upper_bound.h"
#include "../data/cycle.h"


void solve_tsp(graph* G, onetree* H, double* incumbent, onetree* ONE_TREE, double z, int call_flag);
int is_cycle(onetree* OT);
int propagate_constraints(graph* G, edgelist* new_edges, edgelist* old_edges);
#endif
