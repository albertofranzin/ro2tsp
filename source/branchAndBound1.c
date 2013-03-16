#include <stdio.h>
#include "ds/matrixGraph.h"
#include "ds/utils.h"

edge **solveTSP(matrixGraph *graph) {
	printf("\n\n---\n\nComputing the 1-tree : \n\n");
	edge **el = malloc(sizeof(edge) * graph->no_of_nodes);
	memset(el, 0, sizeof(edge)*graph->no_of_nodes);

	double lowerBound = matrixGraphOneTree(graph, &el);

	printf("total cost of the 1-tree : %f\n", lowerBound);

	if (lowerBound < incumbent) {
		incumbent = lowerBound;
	}

	if(isTSP) {
		return el;
	}

	return NULL;
}

double incumbent = DOUBLE_INFINITY;

int main(int argc, char const *argv[]) {
	int no_of_nodes = 10;
	matrixGraph *graph = newMatrixGraph(no_of_nodes);
	printf("matrixGraph created\n");

	edge **opt = solveTSP(graph);

	return 0;
}
