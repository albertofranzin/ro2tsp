#include "edge.h"

/*
 * create an edge, given two nodes and a cost
 */
edge *newEdge(node *u, node *v, double w) {
	//printf("edge.c :: newEdge :: allocating memory for edge\n");fflush(stdout);
	edge *ne = malloc(sizeof(edge));
	memset(ne, 0, sizeof(edge));
	//printf("edge.c :: newEdge :: memory allocated\n");
	ne->u = u;
	ne->v = v;
	ne->weight = w;
	return ne;
}

/*
 * delete an edge
 * just some frees
 */
void deleteEdge(edge *e) {
	deleteNode(e->u);
	deleteNode(e->v);
	free(e);
}
