#ifndef __MATRIXGRAPH_H
#define __MATRIXGRAPH_H

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "edge.h"
#include "set.h"
#include "utils.h"

#define MAX_WEIGHT 100

/*
 * matrixGraph
 *
 * graph represented as a square cost matrix
 * *c : costs (are doubles) - is a matrix, declared as array,
 * 			else I wasn'ì't able to access it, don't know why
 *			Of course, indexing now is a bit a mess
 * n [int] : number of nodes
 * nodeList [node**] : list of nodes
 * edgeList [edge**] : list of edges
 */
typedef struct _matrixGraph {
	double *c;
	int no_of_nodes;
	node **nodeList;
	edge **edgeList;
} matrixGraph;

matrixGraph *newMatrixGraph(size_t);
void deleteMatrixGraph(matrixGraph *);
void randomInitializeGraph(matrixGraph **, size_t, size_t);
double matrixGraphOneTree(matrixGraph *, edge ***);

#endif
