#ifndef SOLVE_TSP_H_
#define SOLVE_TSP_H_

#include "constants.h"
#include "graph.h"
#include "onetree.h"
#include "edgelist.h"
#include <stdio.h>
#include <stdlib.h>
#include "edge.h"
#include "edgelist.h"
#include <string.h>
#include "compute_lagrange.h"
#include "compute_upper_bound.h"
#include "cycle.h"


void solve_tsp(graph* G, onetree* H, double* incumbent, onetree* ONE_TREE, double z, int call_flag);
int is_cycle(onetree* OT);
int propagate_constraints(graph* G, edgelist* new_edges, edgelist* old_edges);
#endif
