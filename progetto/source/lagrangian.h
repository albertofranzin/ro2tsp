#ifndef __LAGRANGIAN_H
#define __LAGRANGIAN_H

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "graph.h"

/*
 * setup for lagrangian relaxation
 * - mult : array of lagrangian multipliers
 * - decreased_ago : number of iterations passed
 *       since last update of relaxation value
 * - alpha : step size coefficient
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
 *
 */

#endif