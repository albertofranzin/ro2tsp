#ifndef __MATRIXGRAPH_H
#define __MATRIXGRAPH_H

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "edge.h"
#include "set.h"

/*
 * matrixGraph
 *
 * graph represented as a square cost matrix
 * **c : costs (are doubles)
 * n [int] : number of nodes
 * nodeList [node**] : list of nodes
 * edgeList [edge**] : list of edges
 */
typedef struct _matrixGraph {
	double **c;
	size_t no_of_nodes;
	node **nodeList;
	edge **edgeList;
} matrixGraph;

matrixGraph *newMatrixGraph(int);
void deleteMatrixGraph(matrixGraph *);
void randomInitializeGraph(matrixGraph *, long);
double matrixGraphOneTree(matrixGraph *, edge **);

#endif