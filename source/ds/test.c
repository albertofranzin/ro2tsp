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

	/*for (i = 0; i < no_of_nodes; ++i) {
		for (j = 0; j < no_of_nodes; ++j) {
			printf("%f ", graph->c[i*no_of_nodes + j]);fflush(stdout);
		}
		printf("\n");
	}*/

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

	if (pars->plot)
		plotGraph(graph, el, pars->plotOnlyTree);

	return 0;
}