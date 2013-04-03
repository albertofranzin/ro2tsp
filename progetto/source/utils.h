#ifndef __UTILS_H
#define __UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include "graph.h"

/* constants and variables */

typedef struct _parameters {
	unsigned int no_of_nodes;
	unsigned int seed;
	unsigned short plot;
	unsigned short plotOnlyTree;
	unsigned int heuristic_trials;
} parameters;

// config file
#define FILE_CONFIG "config"

// define infinity for double
#define INF 1000

/* methods */

// position in the list of costs (matrixGraph->edgeList):
// Corresponding edge (i,j) is in position (binom(i-1, 2) + j)
// in the list of edges - look for 'triangular numbers'.
// Note that since we start counting from 0, the formula
// for triangular numbers should be adjusted accordingly.
// A swap is needed if j > i, but it's fine since graph is directed;
// note that j=i cannot happen, because we don't have self-loops.
// inline int atPosition(int, int);

unsigned long initializeRandom(unsigned int seed);

short parHash(char *);
parameters *getParameters();

int snbdComp(const void *, const void *);
int sebwComp(const void *, const void *);

//void appendDouble(double **n, double, int);

/*
 * matching
 * - G : the graph on which computing the matching
 *
 * compute maximal matching using the Hungarian algorithm
 */
double matching(graph *G);

#endif