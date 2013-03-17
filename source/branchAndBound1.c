#include <stdio.h>
#include "ds/matrixGraph.h"
#include "ds/utils.h"

short isTSP() {
	return 0;
}

edge **solveTSP(matrixGraph *graph) {
	printf("\n\n---\n\nComputing the 1-tree : \n\n");
	edge **el = malloc(sizeof(edge) * graph->no_of_nodes);
	memset(el, 0, sizeof(edge)*graph->no_of_nodes);

	double lowerBound = matrixGraphOneTree(graph, &el);

	printf("total cost of the 1-tree : %f\n", lowerBound);

	if (zincumbent < lowerBound) {
		zincumbent = lowerBound;
		incumbent = (void *)el;
	}

	if(isTSP()) {
		return el;
	}

	return NULL;
}

double zincumbent = DOUBLE_INFINITY;
void *incumbent;

int main(int argc, char const *argv[]) {
	parameters *pars = getParameters();
	initializeRandom(pars->seed);

	int no_of_nodes = pars->no_of_nodes;
	matrixGraph *graph = newMatrixGraph(no_of_nodes);
	printf("matrixGraph created\n");

	edge **opt = solveTSP(graph);

	return 0;
}
