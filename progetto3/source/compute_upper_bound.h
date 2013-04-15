#ifndef COMPUTE_UPPER_BOUND_H_
#define COMPUTE_UPPER_BOUND_H_

#include "constants.h"
#include "compute_nearest_neighbour.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "graph.h"
#include "onetree.h"
#include "tree.h"
#include "compute_path.h"


double compute_upper_bound(graph *G, tree *T);

/*
 * heur2opt
 * G: graph
 * T : tree computed by another heuristic
 *     (will be modified if it's not 2-opt)
 * cost : cost of the solution computed by the heuristic
 *
 * compute a 2-opt admissible solution, in order to swap crossing edges
 *
 * return : cost of the 2-opt solution
 */
double heur2opt(graph *G, tree *T, double cost);


#endif
