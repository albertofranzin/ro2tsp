#include <stdio.h>
#include "ds/matrixGraph.h"
#include "ds/utils.h"

short isTSP() {
	return 0;
}

//edge **
void solveTSP(matrixGraph *graph, double cost_delta) {

	printf("\n\n---\n\nComputing the 1-tree : \n\n");
	edge **el = malloc(sizeof(edge) * graph->no_of_nodes);
	memset(el, 0, sizeof(edge)*graph->no_of_nodes);

	double BB_COST_DELTA = 2.0;

	double lowerBound = matrixGraphOneTree(graph, &el);

	printf("total cost of the 1-tree : %f\n", lowerBound);

	if (zincumbent < lowerBound) {
		zincumbent = lowerBound;
		incumbent = (void *)el;
		return;
	}

	/*if(isTSP()) {
		return el;
	}*/

	int i = 0;


	// copy nodes to order them in degree order
	node **nl = malloc(sizeof(node *)*(graph->no_of_nodes));
	for (i = 0; i < graph->no_of_nodes; ++i) {
		nl[i] = malloc(sizeof(node));
		nl[i]->data = graph->nodeList[i]->data;
		nl[i]->deg = graph->nodeList[i]->deg;
		nl[i]->x = graph->nodeList[i]->x;
		nl[i]->y = graph->nodeList[i]->y;
	}

	int no_of_nodes = graph->no_of_nodes,
		no_of_edges = no_of_nodes * (no_of_nodes - 1) / 2;
	//printf("before iteration\n");

	for (i = 0; i < no_of_nodes; ++i) {
		//printf("%d\n", i);
		//printf("%d\n", el[i]->u->data);
		nl[el[i]->u->data]->deg++;
		nl[el[i]->v->data]->deg++;
	}

	/*for (i = 0; i < graph->no_of_nodes; ++i) {
		printf("deg : %d\n", nl[i]->deg);
	}*/

	//printf("sorting nodes\n");
	qsort(nl, graph->no_of_nodes, sizeof(node *), snbdComp);

	for (i = 0; i < graph->no_of_nodes; ++i) {
		printf("%d : deg : %d\n", i, nl[i]->deg);
	}/**/

	// choose branching node:
	// for now, take the first node in the list with degree = 3
	int over = 0;
	i = 0;
	while(nl[i++]->deg < 3) over++;
	i--;

	printf("branch on node %d : %d : %d\n", i, nl[i]->data, nl[i]->deg);

	// get adjacent nodes if node i in the 1-tree
	// I have to get the number first, and then malloc and fill in the list
	// so 2 scans, which sucks, but I can't do better now
	int adjn = getNumberOfAdjacentNodesInOneTree(graph, el, nl[i]);
	node **adjl = malloc(sizeof(node *) * adjn);

	getAdjacentNodesInOneTree(graph, el, nl[i], &adjl, adjn);
	for (i = 0; i < adjn; ++i) {
		printf("adjacent : %d\n", adjl[i]->data);
	}

	// branch: for now, just consider edges in the bovine way
	// (1 out, 1 in-2 out, 1,2 in, others out)
	int j;

	// memcopy cost matrix, to not fuck up costs
	/*double **cmBackUp = malloc(sizeof(double)*graph->no_of_nodes*graph->no_of_nodes);
	memset(cmBackUp, 0, sizeof(double)*graph->no_of_nodes*graph->no_of_nodes);
	memcpy(&cmBackUp, &(graph->c), sizeof(cmBackUp));*/

	double delta;
	graph->c[nl[i]->data * no_of_nodes + adjl[0]->data] += BB_COST_DELTA;
	graph->c[adjl[0]->data * no_of_nodes + nl[i]->data] += BB_COST_DELTA;

	delta = BB_COST_DELTA * 2.0;
	// deny first neighbour
	solveTSP(graph, delta);

	graph->c[nl[i]->data * no_of_nodes + adjl[0]->data] -= 2*BB_COST_DELTA;
	graph->c[adjl[0]->data * no_of_nodes + nl[i]->data] -= 2*BB_COST_DELTA;
	graph->c[nl[i]->data * no_of_nodes + adjl[1]->data] += BB_COST_DELTA;
	graph->c[adjl[1]->data * no_of_nodes + nl[i]->data] += BB_COST_DELTA;

	delta = 0;
	solveTSP(graph, delta);

	graph->c[nl[i]->data * no_of_nodes + adjl[1]->data] -= 2*BB_COST_DELTA;
	graph->c[adjl[1]->data * no_of_nodes + nl[i]->data] -= 2*BB_COST_DELTA;

	for (j = 2; j < adjn; ++j) {
		graph->c[nl[i]->data * no_of_nodes + adjl[j]->data] += BB_COST_DELTA;
		graph->c[adjl[j]->data * no_of_nodes + nl[i]->data] += BB_COST_DELTA;
	}

	delta = -BB_COST_DELTA * (adjn - 2.0);
	solveTSP(graph, delta);

	//return NULL;
}

double zincumbent = DOUBLE_INFINITY;
void *incumbent;

int main(int argc, char const *argv[]) {
	parameters *pars = getParameters();
	initializeRandom(pars->seed);

	int no_of_nodes = pars->no_of_nodes;
	matrixGraph *graph = newMatrixGraph(no_of_nodes);
	printf("matrixGraph created\n");

	//edge **opt = 
	solveTSP(graph, 0.0);

	printf("%f\n", zincumbent);

	return 0;
}
