#include "branchBoundUtils.h"

branchingInfo *createBranchingInfo(){
	branchingInfo *bi = malloc(sizeof(branchingInfo));

	bi->numEditedEdges = 0;
	bi->deniedEdges = 0;
	bi->imposedEdges = 0;
	bi->branchNode = NULL;
	bi->editedEdges = NULL;
	bi->deltas = NULL;

	return bi;
}

void deleteBranchingInfo(branchingInfo *bi) {
	//free(bi->branchNode);
	//free(bi->editedNodes);
	free(bi->deltas);
	free(bi);
}

branchingInfo *cloneBranchingInfo(branchingInfo *original) {
	return createBranchingInfo(original->branchNode,
							   original->editedEdges,
							   original->deltas,
							   original->numEditedEdges);
}

node *chooseBranchingNode(matrixGraph *graph, edge **el) {
	int i, j, chosenData;

	// copy nodes to order them in degree order
	node **nl = malloc(sizeof(node *)*(graph->no_of_nodes));
	for (i = 0; i < graph->no_of_nodes; ++i) {
		nl[i] = malloc(sizeof(node));
		nl[i]->data = graph->nodeList[i]->data;
		nl[i]->deg = 0;
		nl[i]->x = graph->nodeList[i]->x;
		nl[i]->y = graph->nodeList[i]->y;
	}

	// fill in the right degree
	// (remember that here we are not dealing with the complete graph -
	// which is, you know, /complete/- but only with the 1-tree)
	for (i = 0; i < graph->no_of_nodes; ++i) {
		//printf("%d\n", i);
		//printf("%d\n", el[i]->u->data);
		nl[el[i]->u->data]->deg++;
		nl[el[i]->v->data]->deg++;
	}

	// order the list of nodes according to their degree
	// qsort is from the standard libs, snbdComp is in utils.h
	qsort(nl, graph->no_of_nodes, sizeof(node *), snbdComp);

	/*for (i = 0; i < graph->no_of_nodes; ++i) {
		printf("%d : deg : %d\n", i, nl[i]->deg);
	}/**/

	i = 0;
	while(i < graph->no_of_nodes && nl[i]->deg < 3) i++;

	// have we gone too far away?
	// probably redundant, but...
	if (i < graph->no_of_nodes) {
		chosenData = nl[i]->data;

		/*printf("BranchBoundUtils.c :: chooseBranchingNode :: ");
		printf("branch on node %d : %d\n", nl[i]->data, nl[i]->deg);*/

		deleteNodeList(nl, graph->no_of_nodes);

		return graph->nodeList[chosenData];
	} else {
		deleteNodeList(nl, graph->no_of_nodes);
		return NULL;
	}
}

short isHamilton(matrixGraph *graph, edge **el) {
	// no need to check for subtours
	// I hope, Prim-Dijkstra should shelter us against this

	int i;
	int visited[graph->no_of_nodes];
	memset(visited, 0, sizeof(visited));

	for (i = 0; i < graph->no_of_nodes; ++i) {
		visited[el[i]->u->data]++;
		visited[el[i]->v->data]++;
	}

	for (i = 0; i < graph->no_of_nodes; ++i) {
		if (visited[i] != 2) {
			return 0;
		}
	}

	return 1;
}

double sumDeltas(branchingInfo *bi) {
	int i;
	double sum = 0.;
	printf("branchBoundUtils.c :: sumDeltas :: # of edited edges : %d\n", bi->numEditedEdges);
	for (i = 0; i < bi->numEditedEdges; ++i) {
		printf("\t %d %d has delta :: %f\n", bi->editedEdges[i]->u->data,
											bi->editedEdges[i]->v->data,
											bi->deltas[i]);
		sum += bi->deltas[i];
	}
	return sum;
}

short stoppingCriteria(matrixGraph *graph, edge **el, double *opt, double *incumbent) {

	printf("branchBoundUtils.c :: stoppingCriteria :: entering method\n");

	int i;
	double cost = 0.0;
	for (i = 0; i < graph->no_of_nodes; ++i) {
		if (el[i]->weight >= 2.0) {
			printf("branchBoundUtils.c :: stoppingCriteria :: solution has a forbidden edge\n");
			return -1;
		}
		cost += graph->edgeList[atPosition(el[i]->u->data, el[i]->v->data)]->weight;
	}

	printf("branchBoundUtils.c :: stoppingCriteria :: lower bound computed :: %f\n", cost);

	printf("branchBoundUtils.c :: stoppingCriteria :: ");fflush(stdout);

	// current branch cannot improve the bound
	if (cost >= *incumbent) {
		printf("current branch cannot improve the bound\n");
		return -1;
	} /*else {
		printf("found a better bound\n");
//		if (cost < *opt) {
		*z = cost;
		//plotGraph(graph, el, 1);
		//char c = getchar();
//		}
	}*/

	// current branch contains a hamiltonian path
	if (isHamilton(graph, el) == 1) {
		printf("Hamiltonian path found!!\nExiting successfully\n");
		//*opt = cost;
		//if (cost < *z) {
		*incumbent = cost;
		plotGraph(graph, el, 1);
		char c = getchar();
		return 1;
		//}
	}

	// no reasons to stop
	printf("continuing\n");
	return 0;
}
