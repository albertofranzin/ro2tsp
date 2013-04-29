#ifndef __HEURISTICS_H
#define __HEURISTICS_H

#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include "graph.h"
#include "utils.h"

/*
 * nearestNeighbour
 * - G : the original graph
 * - H : the hamiltonian path computed
 * - root : starting node
 *
 * computes a hamiltonian path in G using the Neareest Neighbour heuristic
 * According to wiki, on average it yields a path 25% longer than optimum,
 * but has an approximation factor of O(log|V|)
 *
 * H is not used, I leave it hoping it will be used later
 *
 * return : cost of the solution
 */
double nearestNeighbour(graph *G, graph *H, int root);

/*
 * heuristicBound
 * - G : the original graph
 * - H : graph containing the solution given by the heuristic
 * - howMany : number of times to repeat the heuristic
 * the only heuristic tried for now is nearest neighbour
 *
 * compute a heuristic bound by calling the chosen heuristic mutiple times
 * in order to try to obtain a better value
 *
 * H is not used, I leave it hoping it will be used later
 *
 * return : bound
 */
double heuristicBound(graph *G, graph *H, int howMany);

/*
 * nn2opt
 * - G : the original graph
 * - H : graph containing the solution given by the heuristic
 * - root : starting node
 *
 * Compute an admissible solution using the Nearest Neighbour heuristic,
 * then apply the 2-opt heuristic to swap crossing edges.
 *
 * return : cost of the solution
 */
double nn2opt(graph *G, graph *H, int root);

#endif
