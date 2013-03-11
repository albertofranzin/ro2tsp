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
	size_t weight;
} edge;

/*
 * create an edge, given two nodes and a cost
 */
edge *newEdge(node *u, node *v, size_t w) {
	edge *ne = malloc(sizeof(node));
	memset(ne, 0, sizeof(node));
	ne->u = u;
	ne->v = v;
	ne->weight = w;
	return ne;
}

/*
 * delete an edge
 * just a free
 */
void deleteEdge(edge *e) {
	deleteNode(e->u);
	deleteNode(e->v);
	free(e);
}

#endif