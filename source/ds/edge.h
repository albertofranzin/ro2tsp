#ifndef __EDGE_H
#define __EDGE_H

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "node.h"

/*
 * edge for an undirected graph
 * the two nodes are equivalent
 * weight is integer
 */
typedef struct _edge {
	node *u;
	node *v;
	double weight;
} edge;

edge *newEdge(node *u, node *v, double w);
void deleteEdge(edge *e);

#endif