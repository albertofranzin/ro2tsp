#include <stdio.h>
#include "matrixGraph.h"
#include "set.h"
#include "plot.h"
#include "utils.h"


int main(int argc, char **argv) {

	parameters *pars = getParameters();

	initializeRandom(pars->seed);

	int no_of_nodes = pars->no_of_nodes;
	matrixGraph *graph = newMatrixGraph(no_of_nodes);
	printf("matrixGraph created\n");

	int i, j;

	for (i = 0; i < no_of_nodes; ++i) {
		for (j = 0; j < no_of_nodes; ++j) {
			printf("%f ", graph->c[i*no_of_nodes + j]);fflush(stdout);
		}
		printf("\n");
	}

	//printf("\n****\n%f\n******\n", graph->edgeList[atPosition(4,0)]->weight);


	printf("\n\n---\n\nComputing the 1-tree : \n\n");
	edge **el = malloc(sizeof(edge) * no_of_nodes);
	memset(el, 0, sizeof(edge)*no_of_nodes);
	double tc = matrixGraphOneTree(graph, &el);
	printf("total cost of the 1-tree : %f\n", tc);
	/*for (i = 0; i < no_of_nodes ; i++) {
		printf("%d %d, %f\n", el[i]->u->data,
							  el[i]->v->data,
							  el[i]->weight);
	}*/

	//printf("%f\n", graph->edgeList[atPosition(4,0)]->weight);


	if (pars->plot)
		plotGraph(graph, el, pars->plotOnlyTree);

	//printf("%f\n", graph->edgeList[atPosition(4,0)]->weight);

	/*for (i = 0; i < no_of_nodes; ++i) {
		for (j = 0; j < no_of_nodes; ++j) {
			printf("%f ", graph->c[i*no_of_nodes + j]);fflush(stdout);
		}
		printf("\n");
	}

	for (i = 0; i < 45; ++i) {
		printf("%f\n", graph->edgeList[i]->weight);
	}*/


	/*node **nl = malloc(sizeof(node)*(graph->no_of_nodes));
	memset(nl, 0, sizeof(node)*(graph->no_of_nodes));
	memcpy(&nl, &(graph->nodeList), sizeof(nl));
	int no_of_edges = no_of_nodes * (no_of_nodes - 1) / 2;
	printf("before iteration\n");
	for (i = 0; i < no_of_nodes; ++i) {
		//printf("%d\n", i);
		//printf("%d\n", el[i]->u->data);
		nl[el[i]->u->data]->deg++;
		nl[el[i]->v->data]->deg++;
	}

	printf("\n---\n");

	for (i = 0; i < no_of_nodes; ++i) {
		printf("deg(%d) = %d\n", i, nl[i]->deg);
	}

	sortNodesByDegree(&nl, 0, no_of_nodes);

	printf("\n");

	for (i = 0; i < no_of_nodes; ++i) {
		printf("deg(%d) = %d\n", i, nl[i]->deg);
	}*/

	return 0;
}