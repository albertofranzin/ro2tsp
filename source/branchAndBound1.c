#include <stdio.h>
#include "ds/branchBoundUtils.h"
//#include "ds/matrixGraph.h"
//#include "ds/utils.h"

//edge **
short solveTSP(matrixGraph *graph, matrixGraph *original) {

	printf("\n\nbeginning solveTSP\n");
	printf("branchAndBound1.c :: solveTSP :: ");
	printf("Computing the 1-tree\n");
	edge **el = malloc(sizeof(edge) * graph->no_of_nodes);
	memset(el, 0, sizeof(edge)*graph->no_of_nodes);

	int i, j;
	double lowerBound = matrixGraphOneTree(graph, &el);

	//printf("branchAndBound1.c :: solveTSP :: ");
	//printf("total cost of the 1-tree : %f (incumbent is %f)\n", lowerBound, zincumbent);

	printf("branchAndBound1.c :: solveTSP :: looking for stopping criteria\n");
	switch(stoppingCriteria(original, el, &zopt, &zincumbent)) {
		case  1 : printf("bene\n");
				  return 1;
		case -1 : printf("male\n");
				  //return -1;
				  break;
		default : printf("nay\n");
				  break;
	}

	matrixGraph *g1 = cloneGraph(graph),
				*g2 = cloneGraph(graph),
				*g3 = cloneGraph(graph);

	// choose branching node:
	// best policy to choose branching node is not a problem addressed now
	// anyway, the correct place to (try to) solve is inside the
	// chooseBranchingNode() method.

	node *branchingNode = (node *)chooseBranchingNode(graph, el);

	if (branchingNode == NULL) {
		printf("branchAndBound1.c :: solveTSP :: no nodes to branch on\n");
		return 0;
	}

	// get adjacent nodes if node i in the 1-tree
	// I have to get the number first, and then malloc and fill in the list
	// so 2 scans, which sucks, but I can't do better now
	int adjn = getNumberOfAdjacentNodesInOneTree(graph, el, branchingNode);
	node **adjl = malloc(sizeof(node *) * adjn);

	getAdjacentNodesInOneTree(graph, el, branchingNode, &adjl, adjn);
	for (i = 0; i < adjn; ++i) {
		printf("adjacent : %d\n", adjl[i]->data);
	}

	printf("branch on node %d , with deg %d\n", branchingNode->data, branchingNode->deg);

	// branch: for now, just consider edges in the bovine way
	// (1 out, 1 in-2 out, 1,2 in, others out)

	double delta = 2;

	// let e1, e2, ... en the n edged to the n neighours
	// deny e1
	printf("branchAndBound1.c :: solveTSP :: denying edge (%d,%d)\n",
			adjl[0]->data, branchingNode->data);
	g1->edgeList[atPosition(adjl[0]->data, branchingNode->data)]->weight += delta;

	printf("branchAndBound1.c :: solveTSP :: start first branch\n");
	if(solveTSP(g1, original) == 1) return 1;

	// impose e1, deny e2
	g2->edgeList[atPosition(adjl[0]->data, branchingNode->data)]->weight -= delta;
	g2->edgeList[atPosition(adjl[1]->data, branchingNode->data)]->weight += delta;

	printf("branchAndBound1.c :: solveTSP :: imposing edge (%d,%d), denying edge (%d,%d)\n\n",
		adjl[0]->data, branchingNode->data, adjl[1]->data, branchingNode->data);
	printf("branchAndBound1.c :: solveTSP :: start second branch\n");
	if (solveTSP(g2, original) == 1) return 1;

	// impose e1, e2, deny all the others
	g3->edgeList[atPosition(adjl[0]->data, branchingNode->data)]->weight -= delta;
	g3->edgeList[atPosition(adjl[1]->data, branchingNode->data)]->weight -= delta;
	printf("branchAndBound1.c :: solveTSP :: imposing edges (%d,%d), (%d,%d)\n",
		adjl[0]->data, branchingNode->data, adjl[1]->data, branchingNode->data);

	int k;
	for (k = 2; k < adjn; ++k) {
		g3->edgeList[atPosition(adjl[k]->data, branchingNode->data)]->weight += delta;
	}

	printf("branchAndBound1.c :: solveTSP :: start third branch\n");
	if (solveTSP(g3, original) == 1) return 1;

	return -1;
}

double zincumbent = DOUBLE_INFINITY;
double zopt = 0;
void *incumbent;

int main(int argc, char const *argv[]) {
	parameters *pars = getParameters();
	initializeRandom(pars->seed);

	int no_of_nodes = pars->no_of_nodes;
	matrixGraph *graph = newMatrixGraph(no_of_nodes);
	printf("matrixGraph created with seed %d\n", pars->seed);

	int i, j;

	for (i = 0; i < no_of_nodes; ++i) {
		for (j = 0; j < i; ++j) {
			printf("%f ", graph->edgeList[atPosition(i,j)]->weight);fflush(stdout);
		}
		printf("\n");
	}

	int result = solveTSP(graph, /*NULL,*/ graph);

	//printf("%f\n", zincumbent);
	//plotGraph(graph, (edge **)incumbent, pars->plotOnlyTree);

	return 0;
}
