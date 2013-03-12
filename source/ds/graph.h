#ifndef __GRAPH_H
#define __GRAPH_H

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "set.h"
#include "node.h"
#include "edge.h"

typedef struct _graph {
	set *nodes;
	set *edges;
} graph;

/*
 * create a graph
 */
graph *newGraph() {
	// create sets for nodes and edges
	set *nnodes = newSet();
	set *nedges = newSet();

	// create graph
	graph *ng = malloc(sizeof(graph));
	memset(ng, 0, sizeof(graph));
	ng->nodes = nnodes;
	ng->edges = nedges;

	return ng;
}

/*
 * delete a graph
 */
void deleteGraph(graph *g) {
	deleteSet(g->nodes);
	deleteSet(g->edges);
	free(g);
}

#endif