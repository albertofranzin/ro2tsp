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
 *
 * computes a hamiltonian path in G using the Neareest Neighbour heuristic
 * According to wiki, on average it yields a path 25% longer than optimum,
 * but has an approximation factor of O(log|V|)
 *
 * return : cost of the solution
 */
double nearestNeighbour(graph *G, graph *H);

#endif