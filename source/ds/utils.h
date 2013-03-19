#ifndef __UTILS_H
#define __UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include "edge.h"

/* constants and variables */

typedef struct _parameters {
	unsigned int no_of_nodes;
	unsigned int seed;
	unsigned short plot;
	unsigned short plotOnlyTree;
} parameters;

// config file
#define FILE_CONFIG "config"

// define infinity for double
#define DOUBLE_INFINITY DBL_MAX

// cost increment/decrement to deny/impose an edge in the solution
// since we operate in [0,1]^2, 2 should be enough.
//#define BB_COST_DELTA 2.0;

// global declaration for incumbent solution
extern double zincumbent;
extern void *incumbent;

/* methods */

// this was just too long to stay in that line with all the names used there
#define distance(x1, y1, x2, y2) \
	sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));

// position in the list of costs (matrixGraph->edgeList):
// Corresponding edge (i,j) is in position (binom(i-1, 2) + j)
// in the list of edges - look for 'triangular numbers'.
// Note that since we start counting from 0, the formula
// for triangular numbers should be adjusted accordingly.
// A swap is needed if j > i, but it's fine since graph is directed;
// note that j=i cannot happen, because we don't have self-loops.
int atPosition(int, int);

void initializeRandom(unsigned int seed);

short parHash(char *);
parameters *getParameters();

void sortNodesByDegree(node ***, int , int );

void appendDouble(double **n, double, int);

#endif