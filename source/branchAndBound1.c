#include <stdio.h>
#include "ds/branchBoundUtils.h"
#include "ds/matrixGraph.h"
#include "ds/utils.h"

//edge **
void solveTSP(matrixGraph *graph, double cost_delta, branchingInfo *bbi) {

	printf("\n\nbeginning solveTSP\n\n");
	printf("branchAndBound1.c :: solveTSP :: ");
	printf("Computing the 1-tree\n");
	edge **el = malloc(sizeof(edge) * graph->no_of_nodes);
	memset(el, 0, sizeof(edge)*graph->no_of_nodes);

	int i;
	double lowerBound = matrixGraphOneTree(graph, &el);
	//lowerBound += cost_delta;

	printf("branchAndBound1.c :: solveTSP :: ");
	printf("total cost of the 1-tree : %f (incumbent is %f)\n", lowerBound, zincumbent);

	/*if (zincumbent <= lowerBound) {
		printf("discarding current branch\n");
		return;
	}

	if (zincumbent > lowerBound || isHamilton(graph, el)) {
		for (i = 0; i < graph->no_of_nodes; ++i) {
			printf("%d %d - %f\n", el[i]->u->data, el[i]->v->data, el[i]->weight);
		}
		zincumbent = lowerBound;
		incumbent = (void *)el;
		//return;
	}

	if(isHamilton(graph, el)) {
		/*for (i = 0; i < graph->no_of_nodes; ++i) {
			printf("%d %d - %f\n", el[i]->u->data, el[i]->v->data, el[i]->weight);
		}* /
		printf("HAMILTON\n\n\n");
		char ch; scanf("%c\n",&ch);
		return ;//el;
	}*/

	branchingInfo *bi;
	printf("branchAndBound1.c :: solveTSP :: checking branchingInfo :: ");fflush(stdout);
	if (bbi == NULL) {
		printf("create new one\n");
		bi = createBranchingInfo(NULL, NULL, NULL, 0);
	} else {
		printf("clone old one\n");
		bi = cloneBranchingInfo(bbi);
	}

	i = 0;
	int no_of_nodes = graph->no_of_nodes;

	printf("branchAndBound1.c :: solveTSP :: looking for stopping criteria\n");
	switch(stoppingCriteria(graph, el, bi, lowerBound, &zincumbent)) {
		case -1 : printf("male\n");
				  return;
		case  1 : printf("bene\n");
				  return;
		default : printf("nay\n");
				  break;
	}

	// choose branching node:
	// best policy to choose branching node is not a problem addressed now
	// anyway, the correct place to (try to) solve is inside this
	// chooseBranchingNode() method.

	node *branchingNode = (node *)chooseBranchingNode(graph, el);
	bi->branchNode = branchingNode;
	bi->numEditedEdges++;

	printf("branch on node %d : %d : %d\n", i, branchingNode->data, branchingNode->deg);

	// get adjacent nodes if node i in the 1-tree
	// I have to get the number first, and then malloc and fill in the list
	// so 2 scans, which sucks, but I can't do better now
	int adjn = getNumberOfAdjacentNodesInOneTree(graph, el, branchingNode);
	node **adjl = malloc(sizeof(node *) * adjn);

	getAdjacentNodesInOneTree(graph, el, branchingNode, &adjl, adjn);
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

	double delta = 100.0 * 2;

	graph->edgeList[atPosition(0, branchingNode->data)]->weight += delta;

	// deny first neighbour
	/*branchingInfo *bi1 = createBranchingInfo(branchingNode,
				NULL, NULL, bi->numEditedEdges + 1);
	bi1->editedEdges = malloc(sizeof(edge *) * bi1->numEditedEdges);
	bi1->deltas = malloc(sizeof(double) * bi1->numEditedEdges);
	for (i = 0; i < bi1->numEditedEdges - 1; ++i) {
		bi1->editedEdges[i] = bi->editedEdges[i];
		bi1->deltas[i] = bi->deltas[i];
	}
	bi1->editedEdges[i] = graph->edgeList[atPosition(0, branchingNode->data)];
	printf("branchAndBound1.c :: solveTSP :: start branch\n");*/
	solveTSP(graph, -2*delta, bi);

	graph->edgeList[atPosition(0, branchingNode->data)]->weight -= 2*delta;
	graph->edgeList[atPosition(1, branchingNode->data)]->weight += delta;

	//branchingInfo *bi2 = cloneBranchingInfo(bi);
	//bi2->delta = 0
	solveTSP(graph, 0, bi);

	graph->edgeList[atPosition(1, branchingNode->data)]->weight -= 2*delta;

	for (j = 2; j < adjn; ++j) {
		graph->edgeList[atPosition(j, branchingNode->data)]->weight += delta;
	}

	delta = delta * (adjn - 2);
	//branchingInfo *bi3 = cloneBranchingInfo(bi);
	//bi3->delta = -2*delta
	solveTSP(graph, -delta, bi);

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
	solveTSP(graph, 0.0, NULL);

	printf("%f\n", zincumbent);
	//plotGraph(graph, (edge **)incumbent, pars->plotOnlyTree);

	return 0;
}
