#ifndef COMPUTE_UPPER_BOUND_H_
#define COMPUTE_UPPER_BOUND_H_

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "../base/constants.h"
#include "../base/utils.h"

#include "../data/graph.h"
#include "../data/onetree.h"
#include "../data/tree.h"
#include "../data/cycle.h"

#include "compute_lagrange.h"

#include "../solvers/solve_tsp.h"


double compute_upper_bound(graph *G, cycle *C, int algo);


double compute_nearest_neighbour(graph *G, cycle *C, int start_node, int algo);


/*
 * heur2opt
 * G : graph
 * C : cycle computed by another heuristic
 *     (will be modified if it's not 2-opt)
 * cost : cost of the solution computed by the heuristic
 *
 * compute a 2-opt admissible solution, in order to swap crossing edges
 *
 * return : cost of the 2-opt solution
 */
double heur2opt(graph *G, cycle *C, double ccost);

/*
 * trimmedBranchAndBound
 * G : graph
 * OT : one-tree computed
 *
 * trim the graph, by setting 'longest' edges to infinity
 * execute a branch and bound on trimmed graph
 * 'longest' == longer than a certain threshold, for now fixed at
 * median of the lengths of edges insisting on a node
 *
 * return : cost of the onetree
 */
double trimmedBranchAndBound(graph *G, onetree *OT);

/*
 * improved2opt
 * G : graph
 * C : cycle computed by another heuristic
 *     (will be modified if it's not 2-opt)
 * cost : cost of the solution computed by the heuristic
 *
 * compute a 2-opt solution, swap 3 random edges and apply 2-opt again
 *
 * return : cost of the solution
 */
double improved2opt(graph *G, cycle *C, double ccost);

/*
 * random_cycle_generation
 * - G : graph
 * - C : the best cycle computed
 * - num_cycles : number of cycles to be tried
 *
 * generate num_cycles random cycles, and keep the best one
 * (many thanks to Matteo Boscariol for the idea)
 *
 * return : cost of the returned cycle
 */
double random_cycle_generation(graph *G, cycle *C, int num_cycles);


#endif
