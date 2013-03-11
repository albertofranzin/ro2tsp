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

/*
 * constructor
 * number_of_nodes [int] : number of nodes in the graph
 *
 * construct the matrix graph
 * graph is supposed to be complete
 *
 * return : reference of the graph
 */
matrixGraph *newMatrixGraph(int number_of_nodes) {
	// allocate lists for nodes and edges
	// it compiles - but I'm not sure it will run without segfaulting
	node **nodeList = malloc(sizeof(node) * number_of_nodes);
	memset(nodeList, 0, sizeof(node) * number_of_nodes);
	edge **edgeList = malloc(sizeof(node) * number_of_nodes * (number_of_nodes - 1));
	memset(edgeList, 0, sizeof(node) * (number_of_nodes * (number_of_nodes - 1)) / 2 );

	matrixGraph *mg = malloc(sizeof(double) * number_of_nodes * number_of_nodes
			+ sizeof(node **) + sizeof(edge **));
	memset(mg, 0, sizeof(*mg));
	mg->no_of_nodes = number_of_nodes;
	mg->nodeList = nodeList;
	mg->edgeList = edgeList;

	int i, j;
	for (i = 0; i < number_of_nodes - 1; ++i) {
		node *nn = newNode(i);
	}

	return mg;
}

/*
 * delete
 * mg [matrixGraph] : the graph to be deleted
 *
 * delete the whole graph
 */
void deleteMatrixGraph(matrixGraph *mg) {
	free(mg->c);
	free(mg);
}

/*
 * randomInitializeGraph
 * graph [matrixGraph] : the graph
 * seed [long] : seed for the pseudorandom generator
 *
 * initialize the cost matrix with random values
 *
 * TODO use randomness
 */
void randomInitializeGraph(matrixGraph *graph, long seed) {
	/*
	 * initialize random with seed
	 */

	int i, j;
	for (i = 0; i < graph->no_of_nodes; ++i) {
		for (j = 0; j < i; ++j) {
			graph->c[i][j] = 4;
			// chosen with a dice - guaranteed it's random (xkcd \"{u}ber alles)
		}
	}
}

/*
 * matrixGraphOneTree
 * graph [matrixGraph] : the graph
 * tree [edge*] : the empty 1-tree to be filled
 *
 * builds a 1-tree
 *
 * return: the cost of the 1-tree
 */
double matrixGraphOneTree(matrixGraph *graph, edge **tree) {
	double cost = 0;
	int i, j;

	// delete the possible data contained in the 1-tree
	// remember that a 1-tree associated to a graph has |V|-2 edges
	/*
	TODO correct free of all the memory
	for (i = 0 ; i < graph->no_of_nodes - 2 ; i += sizeof(edge)) {
		deleteEdge(&(tree + i));
	}
	*/
	free(tree);

	// after the cleaning, create the tree
	*tree = malloc(sizeof(edge) * (graph->no_of_nodes - 2));
	memset(tree, 0, sizeof(edge) * (graph->no_of_nodes - 2));

	// recover the 1-tree
	i = 0;
	j = 0;
	// edges \in \delta(1) go into one set,
	// all the other edges fall into the other set
	set *deltaOfOne = newSet();
	set *EdgesWithoutOne = newSet();
	for (i = 0; i < graph->no_of_nodes; ++i) {
		for (j = 0; j < i; ++j) {
			edge *e = newEdge(i, j, graph->c[i][j]);
		}
	}
}


#endif