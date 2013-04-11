#ifndef COMPUTE_UPPER_BOUND_H_
#define COMPUTE_UPPER_BOUND_H_

#include "compute_nearest_neighbour.h"
#include <stdlib.h>
#include "graph.h"
#include "onetree.h"
#include "tree.h"

double compute_upper_bound(graph *G, tree *OT);

/*
 * heur2opt
 * G: graph
 * OT : tree computed by another heuristic
 *     (will be modified if it's not 2-opt)
 * cost : cost of the solution computed by the heuristic
 *
 * compute a 2-opt admissible solution, in order to swap crossing edges
 *
 * return : cost of the 2-opt solution
 */
double heur2opt(onetree *G, tree *OT, double cost);

#endif
