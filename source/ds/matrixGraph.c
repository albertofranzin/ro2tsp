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
	matrixGraph *mg = (matrixGraph *)malloc(sizeof(matrixGraph));//(sizeof(long) + sizeof(void *) * 3);
	memset(mg, 0, sizeof(matrixGraph));//sizeof(long) + sizeof(void *) * 3);
	node **nodeList = (node **)malloc(sizeof(node) * number_of_nodes);
	memset(nodeList, 0, sizeof(node) * number_of_nodes);
	edge **edgeList = (edge **)malloc(sizeof(node) * (number_of_nodes * (number_of_nodes - 1))/2);
	memset(edgeList, 0, sizeof(node) * (number_of_nodes * (number_of_nodes - 1)) / 2 );

	//note to self : it's useless to search how to access a list which has not been allocated...
	double *c = malloc(sizeof(double)*number_of_nodes*number_of_nodes);
	memset(c, 0, sizeof(double)*number_of_nodes*number_of_nodes);

	mg->no_of_nodes = number_of_nodes;
	mg->nodeList = nodeList;
	mg->edgeList = edgeList;
	mg->c = c;

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

	// compute cost of edges using euclidean distance between nodes
	for (i = 0; i < mg->no_of_nodes; ++i) {
		for (j = 0; j < i; ++j) {
			//printf("%d %d", i, j);fflush(stdout);
			//printf("%f\n", graph->c[i][j]);
			node *n1 = mg->nodeList[i];
			node *n2 = mg->nodeList[j];
			mg->c[i*mg->no_of_nodes + j] = distance(n1->x, n1->y, n2->x, n2->y);
			mg->c[j*mg->no_of_nodes + i] = mg->c[i*mg->no_of_nodes + j];
		}
	}

	int counter = 0;
	for (i = 0; i < number_of_nodes; ++i) {
		for (j = 0; j < i; ++j) {
			//printf(" -- %d\n", counter);
			//printf("%d\n", mg->nodeList[i]);
			//printf("%f\n", mg->c[i][j]);
			edge *ne = newEdge( mg->nodeList[i],
								mg->nodeList[j],
								mg->c[i * mg->no_of_nodes + j]);
			//printf(" -- \n");
			mg->edgeList[counter++] = ne;
		}
	}

	printf("matrixGraph.c :: newMatrixGraph :: list of edges now populated :: %d\n",
		counter);

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
 */
void randomInitializeGraph(matrixGraph **ggraph, size_t maxWeight) {

	matrixGraph *graph = (matrixGraph *)(*ggraph);
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
double matrixGraphOneTree(matrixGraph *graph, edge ***ttree) {
	double cost = 0;
	int i, j;
	edge **tree = (edge **)(*ttree);

	// recover the 1-tree
	i = 0;
	j = 0;

	// find spanning tree of nodes in edgesWithout1st
	// using Prim-Dijkstra
	//short isInTree[graph->no_of_nodes - 1];// = {0};

	// here, now, I ignore the fact that the 1st node has been "removed"
	// thus, code will use the same index as pseudocode
	short flag[graph->no_of_nodes];
	int pred[graph->no_of_nodes], h;
	double L[graph->no_of_nodes], min, total_cost = 0.;

	// just...
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
		L[i] = graph->c[graph->no_of_nodes + i];
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
			if (flag[j] == 0 && graph->c[h * graph->no_of_nodes + j] < L[j]) {
				L[j] = graph->c[h * graph->no_of_nodes + j];
				pred[j] = h;
			}
		}
	}

	// partially (bubble-)sorting the edges insisting on the first node,
	// in order to "close the circle".

	// First, malloc and brutal memory copy
	edge **deltaOf1st = malloc(sizeof(edge)*(graph->no_of_nodes-1));
	memset(deltaOf1st, 0, sizeof(edge)*(graph->no_of_nodes-1));
	// fill in deltaOf1st with the first positions of graph->edgeList
	memcpy(&deltaOf1st, &(graph->edgeList), sizeof(deltaOf1st));

	// then scan the list twice
	// remember that we need the tho cheapest edges
	for (i = 0; i < 2; ++i) {
		for (j = i+1; j < graph->no_of_nodes; ++j) {
			if (deltaOf1st[i]->weight > deltaOf1st[j]->weight) {
				edge *tmp = deltaOf1st[i];
				deltaOf1st[i] = deltaOf1st[j];
				deltaOf1st[j] = tmp;
			}
		}
	}

	tree[0] = deltaOf1st[0];
	tree[1] = deltaOf1st[1];
	total_cost += deltaOf1st[0]->weight + deltaOf1st[1]->weight;

	// finally, fill in the tree with the right edges
	for (i = 2; i < graph->no_of_nodes ; ++i) {
		// Corresponding edge (h,j) is in position (binom(h-1, 2) + j)
		// in the list of edges - look for 'triangular numbers'.
		// Note that since we start counting from 0, the formula
		// for triangular numbers should be adjusted accordingly.
		// A swap is needed if j > h, but it's fine since graph is directed;
		// note that j=h cannot happen, because we don't have self-loops.
		j = i;
		h = pred[j];
		if (h < j) {
			int tmp = h;
			h = j;
			j = tmp;
		}
		tree[i] = (edge *)(graph->edgeList[(h*(h-1))/2 + j]);

		// at the same time, spare a loop by computing the total cost of the 1-tree
		total_cost += L[i];
	}

	free(deltaOf1st);

	return total_cost;

}
