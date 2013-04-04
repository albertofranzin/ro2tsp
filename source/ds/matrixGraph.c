#include "matrixGraph.h"

/*
 * constructor
 * number_of_nodes [int] : number of nodes in the graph
 *
 * construct the matrix graph
 * graph is supposed to be complete
 *
 * return : reference of the graph
 */
matrixGraph *newMatrixGraph(size_t number_of_nodes) {

	// allocate lists for nodes and edges
	//note to self : it's useless to search how to access a list which has not been allocated...
	matrixGraph *mg = malloc(sizeof(matrixGraph));
	mg->no_of_nodes = number_of_nodes;
	mg->nodeList = malloc(sizeof(node *) * number_of_nodes);
	mg->edgeList = malloc(sizeof(edge *) * (number_of_nodes * (number_of_nodes - 1))/2);

	//memory allocated
	printf("matrixGraph.c :: newMatrixGraph :: memory allocated\n");

	//randomInitializeGraph(&mg, 100);
	//printf("matrixGraph.c :: newMatrixGraph :: matrixGraph populated\n");

	//now, populate the lists of nodes and edges
	int i, j;
	for (i = 0; i < number_of_nodes; ++i) {
		node *nn = newNode(i);
		nn->x = rand() / 1.0 / RAND_MAX;
		nn->y = rand() / 1.0 / RAND_MAX;
		mg->nodeList[i] = nn;
	}

	printf("matrixGraph.c :: newMatrixGraph :: list of nodes now populated\n");

	int counter = 0;
	// compute cost of edges using euclidean distance between nodes
	for (i = 0; i < mg->no_of_nodes; ++i) {
		for (j = 0; j < i; ++j) {
			//printf("%d %d", i, j);fflush(stdout);
			//printf("%f\n", graph->c[i][j]);
			node *n1 = mg->nodeList[i];
			node *n2 = mg->nodeList[j];
			mg->edgeList[atPosition(n1->data,n2->data)] = newEdge(n1, n2,
						sqrt((n1->x - n2->x)*(n1->x - n2->x) +
							 (n1->y - n2->y)*(n1->y - n2->y))
					);
		}
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
	//free(mg->c);
	free(mg);
}

/*
 * randomInitializeGraph
 * graph [matrixGraph] : the graph
 * seed [long] : seed for the pseudorandom generator
 *
 * initialize the cost matrix with random values
 */
void randomInitializeGraph(matrixGraph **ggraph, size_t maxWeight) {

	/*matrixGraph *graph = (matrixGraph *)(*ggraph);
	printf("matrixGraph.c :: randomInitializeGraph :: graph retrieved\n");
	//printf("%d\n", graph->no_of_nodes);
	int i, j;
	printf("matrixGraph.c :: randomInitializeGraph :: cycle begins :: #nodes = %d\n",
			graph->no_of_nodes);
	for (i = 0; i < graph->no_of_nodes; ++i) {
		for (j = 0; j < i; ++j) {
			//printf("%d %d", i, j);fflush(stdout);
			//printf("%f\n", graph->c[i][j]);
			graph->c[i*graph->no_of_nodes + j] = (double)(rand() % maxWeight) + 1.;
			graph->c[j*graph->no_of_nodes + i] = graph->c[i*graph->no_of_nodes + j];
		}
	}*/
}

double matrixGraphMST(matrixGraph *graph, edge ***ttree) {
	edge **tree = (edge **)(*ttree);

	short flag[graph->no_of_nodes - 1];
	int pred[graph->no_of_nodes - 1];
	double L[graph->no_of_nodes - 1], min, total_cost = 0.;
	int i, j, h;

	// vertices not in the connected component (delta of 1 - node 0 is not considered)
	for (i = 0; i < graph->no_of_nodes ; ++i) {
		flag[i] = 0;
		pred[i] = 1;
		L[i] = graph->edgeList[atPosition(0,i)]->weight;
	}

	// select the n-1 edges of the tree
	for (i = 0; i < graph->no_of_nodes - 1; ++i) {
		min = MAX_WEIGHT + 1;
		// choose the lowest-cost edge in delta(connected component)
		for (j = 1; j < graph->no_of_nodes; ++j) {
			if (flag[j] == 0 && L[j] < min) {
				min = L[j];
				h = j;
			}
		}
		flag[h] = 1; // node h is added to the connected component
		// update cost and predecessor for every node not yet in the connected component
		for (j = 1; j < graph->no_of_nodes; ++j) {
			if (flag[j] == 0 && graph->edgeList[atPosition(h,j)]->weight < L[j]) {
				L[j] = graph->edgeList[atPosition(h,j)]->weight;
				pred[j] = h;
			}
		}
	}

	for (i = 0; i < graph->no_of_nodes - 1; ++i) {
		h = pred[i];
		tree[i] = (edge *)(graph->edgeList[atPosition(h,i)]);

		// at the same time, spare a loop by computing the total cost of the 1-tree
		total_cost += L[i];
	}

	return total_cost;
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
double matrixGraphOneTree(matrixGraph *graph, edge ***ttree) {
	double cost = 0;
	int i, j;
	edge **tree = (edge **)(*ttree);

	// recover the 1-tree
	i = 0;
	j = 0;

	// find spanning tree of nodes using Prim-Dijkstra

	// here, now, I ignore the fact that the 1st node has been "removed"
	// thus, code will use the same index as pseudocode
	short flag[graph->no_of_nodes];
	int pred[graph->no_of_nodes], h;
	double L[graph->no_of_nodes], min, total_cost = 0.;

	// just\dots
	flag[0] = 0;
	pred[0] = 0;
	L[0] = 0;

	flag[1] = 1;
	pred[1] = 0;
	L[1] = 0;

	// vertices not in the connected component (delta of 1 - node 0 is not considered)
	for (i = 2; i < graph->no_of_nodes; ++i) {
		flag[i] = 0;
		pred[i] = 1;
		L[i] = graph->edgeList[atPosition(1,i)]->weight;
	}

	// select the n-1 edges of the tree
	for (i = 1; i < graph->no_of_nodes - 1; ++i) {
		min = MAX_WEIGHT + 1;
		// choose the lowest-cost edge in delta(connected component)
		for (j = 2; j < graph->no_of_nodes; ++j) {
			if (flag[j] == 0 && L[j] < min) {
				min = L[j];
				h = j;
			}
		}
		flag[h] = 1; // node h is added to the connected component
		// update cost and predecessor for every node not yet in the connected component
		for (j = 2; j < graph->no_of_nodes; ++j) {
			if (flag[j] == 0 && graph->edgeList[atPosition(h,j)]->weight < L[j]) {
				L[j] = graph->edgeList[atPosition(h,j)]->weight;
				pred[j] = h;
			}
		}
	}

	// partially (bubble-)sorting the edges insisting on the first node,
	// in order to "close the circle".

	edge **deltaOf1st = malloc(sizeof(edge *)*(graph->no_of_nodes -1));
	// fill in deltaOf1st with the first positions of graph->edgeList
	for (i = 0; i < graph->no_of_nodes - 1; ++i) {
		deltaOf1st[i] = graph->edgeList[atPosition(i+1,0)];
	}

	/*for (i = 0; i < graph->no_of_nodes - 1; ++i) {
		printf("--- (%d, %d), %f\n", deltaOf1st[i]->u->data, deltaOf1st[i]->v->data, deltaOf1st[i]->weight);
	}*/

	// then scan the list twice
	// remember that we need the tho cheapest edges
	for (i = 0; i < 2; ++i) {
		for (j = i+1; j < graph->no_of_nodes - 1; ++j) {
			if (deltaOf1st[i]->weight > deltaOf1st[j]->weight) {
				edge *tmp = deltaOf1st[i];
				deltaOf1st[i] = deltaOf1st[j];
				deltaOf1st[j] = tmp;
			}
		}
	}
	//printf("two lowest-cost edges : %f %f \n", deltaOf1st[0]->weight, deltaOf1st[1]->weight);
	//qsort(deltaOf1st, graph->no_of_nodes - 1, sizeof(edge *), sebwComp);

	tree[0] = deltaOf1st[0];
	tree[1] = deltaOf1st[1];
	total_cost += deltaOf1st[0]->weight + deltaOf1st[1]->weight;

	// finally, fill in the tree with the right edges
	for (i = 2; i < graph->no_of_nodes ; ++i) {
		//j = i;
		h = pred[i];
		tree[i] = (edge *)(graph->edgeList[atPosition(h,i)]);

		// at the same time, spare a loop by computing the total cost of the 1-tree
		total_cost += L[i];
		// and node degree in the 1-tree
		/*tree[i]->u->deg++;
		tree[i]->v->deg++;*/

	}

	free(deltaOf1st);

	return total_cost;

}

int getNumberOfAdjacentNodesInOneTree(matrixGraph *graph, edge **el, node *n) {
	int i, adjn = 0;
	for (i = 0; i < graph->no_of_nodes; ++i) {
		if (el[i]->u->data == n->data || el[i]->v->data == n->data) {
			adjn++;
		}
	}
	return adjn;
}

/*
 * getAdjacentNodesInOneTree
 * get adjacent nodes of a node in the 1-tree sugbraph
 *
 * graph : the complete graph
 * el : the 1-tree
 * n : the node
 * al : the adjacent list (to be modified)
 *
 * return : the number of adjacent nodes
 */
void getAdjacentNodesInOneTree(matrixGraph *graph, edge **el, node *n, node ***aal, int adjn) {
	int i, j = 0;
	// don't want to scan all the 1-tree once before,
	// so, initially malloc enough space for all
	//node **tmpal = malloc(sizeof(node) * graph->no_of_nodes);
	//memset(tmpal, 0, sizeof(node) * graph->no_of_nodes);

	// scan
	node **al = (node **)(*aal);
	for (i = 0; i < graph->no_of_nodes; ++i) {
		if (el[i]->u->data == n->data) {
			al[j++] = el[i]->v;
		} else if (el[i]->v->data == n->data) {
			al[j++] = el[i]->u;
		}
	}

	// now transfer the list
	
	/*al = malloc(sizeof(node) * adjn);
	memset(al, 0, sizeof(node) * adjn);
	memcpy(&al, &tmpal, sizeof(node) * adjn);

	free(tmpal);*/
	//printf("hey\n");
	//return adjn;
}

/*
 * cloneGraph
 * original : the original matrixGraph to be cloned
 *
 * duplicate an entire graph
 *
 * return : a copy of the original graph
 */
matrixGraph *cloneGraph(matrixGraph *original) {
	// have to malloc instead of calling the proper function, because here
	// I don't need the graph to be initialized
	matrixGraph *copy = malloc(sizeof(matrixGraph));
	copy->no_of_nodes = original->no_of_nodes;
	copy->nodeList = malloc(sizeof(node *) * copy->no_of_nodes);
	copy->edgeList = malloc(sizeof(edge *) * (copy->no_of_nodes * (copy->no_of_nodes - 1))/2);

	int i;

	for (i = 0; i < copy->no_of_nodes; ++i) {
		copy->nodeList[i] = newNode(original->nodeList[i]->data);
		copy->nodeList[i]->deg = original->nodeList[i]->deg;
		copy->nodeList[i]->x = original->nodeList[i]->x;
		copy->nodeList[i]->y = original->nodeList[i]->y;
	}

	for (i = 0; i < (copy->no_of_nodes * (copy->no_of_nodes - 1))/2; ++i) {
		copy->edgeList[i] = newEdge(
					copy->nodeList[original->edgeList[i]->u->data],
					copy->nodeList[original->edgeList[i]->v->data],
					original->edgeList[i]->weight
				);
	}

	return copy;
}
