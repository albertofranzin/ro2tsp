#ifndef __LAGRANGIAN_H
#define __LAGRANGIAN_H

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "graph.h"
#include "heuristics.h"

/*
 * setup for lagrangian relaxation
 * - mult : array of lagrangian multipliers
 * - decreased_ago : number of iterations passed
 *       since last update of relaxation value
 * - alpha : step size parameter
 *       (to be halved after decreased_ago iterations without enhancing)
 */
typedef struct _lagrangian {
	double *mult;
	int decreased_ago;
	int wait_for;
	double alpha;
} lagrangian;

/*
 * initializer
 * -n : number of nodes in the graph
 *
 * default values are initialized here
 * Later may be parameters...
 */
lagrangian* initLagrange(int n);

/*
 * update lagrangian coefficients
 * - G : the graph
 * - l : the lagrangian setup
 */
void updateLagrange(graph *G, lagrangian *l);

/*
 * compute cost of a graph when lagrangian multipliers are applied
 * - ONE_TREE : 1-tree
 * - G : graph
 * - l : the lagrangian setup
 * - ub : an upper bound to the solution
 * - rs : the value of the relaxed solution
 *
 * compute the cost of the graph and then subtract the cost imposed
 * by lagrangian multipliers.
 # At the same time, update lagrangian multipliers.
 *
 * return : cost of the graph
 */
double getLagrangeGraphCost(graph *ONE_TREE, graph *G, lagrangian **ll, double ub, double rs);

#endif