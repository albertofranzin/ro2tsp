#include <stdio.h>
#include "ds/branchBoundUtils.h"
//#include "ds/matrixGraph.h"
//#include "ds/utils.h"

//edge **
short solveTSP(matrixGraph *graph, matrixGraph *original) { //, branchingInfo *bri

	/*printf("\n\nbeginning solveTSP\n");
	printf("branchAndBound1.c :: solveTSP :: ");
	printf("Computing the 1-tree\n");*/
	edge **el = malloc(sizeof(edge) * graph->no_of_nodes);
	memset(el, 0, sizeof(edge)*graph->no_of_nodes);

	int i, j;
	double delta = 100.0;

	double lowerBound = matrixGraphOneTree(graph, &el);

	//printf("branchAndBound1.c :: solveTSP :: ");
	//printf("total cost of the 1-tree : %f (incumbent is %f)\n", lowerBound, zincumbent);

	/*printf("branchAndBound1.c :: solveTSP :: looking for stopping criteria\n");
	switch(stoppingCriteria(original, el, &zopt, &zincumbent)) {
		case  1 : //printf("bene\n");
				  return 1;
		case -1 : //printf("male\n");
				  return -1;
				  break;
		default : //printf("nay\n");
				  break;
	}*/

	/////////////////////////////////////////////////////////////////////

	double cost = 0.0;
	for (i = 0; i < graph->no_of_nodes; ++i) {
		if (el[i]->weight >= 2.0) {
			//printf("branchBoundUtils.c :: stoppingCriteria :: solution has a forbidden edge\n");
			return -1;
		}
		cost += original->edgeList[atPosition(el[i]->u->data, el[i]->v->data)]->weight;
	}

	/*printf("branchBoundUtils.c :: stoppingCriteria :: lower bound computed :: %f\n", cost);

	printf("branchBoundUtils.c :: stoppingCriteria :: ");fflush(stdout);*/

	// current branch cannot improve the bound
	if (cost >= zincumbent || cost < 0) {
		//printf("current branch cannot improve the bound\n");
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

		for (i = 0; i < graph->no_of_nodes; ++i) {
			printf("(%d,%d), %f\n", el[i]->u->data, el[i]->v->data,
				original->edgeList[atPosition(el[i]->u->data, el[i]->v->data)]->weight);
		}
		printf("total cost : %f\n", cost);
		//*opt = cost;
		//if (cost < *z) {
		zincumbent = cost;
		plotGraph(graph, el, 0);
		char c = getchar();
		//return 1;
		//}
	}

	///////////////////////////////////////////////////////////////////////

	matrixGraph *g1 = cloneGraph(graph),
				*g2 = cloneGraph(graph),
				*g3 = cloneGraph(graph);

	// choose branching node:
	// best policy to choose branching node is not a problem addressed now
	// anyway, the correct place to (try to) solve is inside the
	// chooseBranchingNode() method.

	node *branchingNode = (node *)chooseBranchingNode(graph, el);

	if (branchingNode == NULL) {
		//printf("branchAndBound1.c :: solveTSP :: no nodes to branch on\n");
		return 0;
	}

	// get adjacent nodes if node i in the 1-tree
	// I have to get the number first, and then malloc and fill in the list
	// so 2 scans, which sucks, but I can't do better now
	int adjn = getNumberOfAdjacentNodesInOneTree(graph, el, branchingNode);
	if (adjn < 3) {
		//printf("branchAndBound1.c :: solveTSP :: chosen node has no enough choice\n");
		return 0;
	}

	node **adjl = malloc(sizeof(node *) * adjn);

	getAdjacentNodesInOneTree(graph, el, branchingNode, &adjl, adjn);
	/*for (i = 0; i < adjn; ++i) {
		printf("adjacent : %d, %f\n", adjl[i]->data, graph->edgeList[atPosition(adjl[i]->data,branchingNode->data)]->weight);
	}*/

	// selected edges (meaning: they are ok)
	int s1 = -1, s2 = -1;

	i = 0;
	while(i < adjn && s1 < 0) {
		if (graph->edgeList[atPosition(adjl[i]->data, branchingNode->data)]->weight > 0 &&
			graph->edgeList[atPosition(adjl[i]->data, branchingNode->data)]->weight < delta) {
			s1 = i;
		}
		i++;
	}
	while(i < adjn && s2 < 0) {
		if (graph->edgeList[atPosition(adjl[i]->data, branchingNode->data)]->weight > 0 &&
			graph->edgeList[atPosition(adjl[i]->data, branchingNode->data)]->weight < delta) {
			s2 = i;
		}
		i++;
	}

	/*printf("... s1 = %d\n", s1);
	printf("... s2 = %d\n", s2);*/
	// not enough good edges to branch on
	if (s1 < 0) {
		//printf("branchAndBound1.c :: solveTSP :: not enough good edges on chosen node\n");
		return -1;
	}

	//printf("branch on node %d , with deg %d\n", branchingNode->data, branchingNode->deg);

	if (s2 >= 0) {
		
		// branch: for now, just consider edges in the bovine way
		// (1 out, 1 in-2 out, 1,2 in, others out)

		// let e1, e2, ... en the n edged to the n neighours

		// forbid e1
		//printf("branchAndBound1.c :: solveTSP :: forbidding edge (%d,%d)\n",
		//		adjl[s1]->data, branchingNode->data);
		g1->edgeList[atPosition(adjl[s1]->data, branchingNode->data)]->weight += delta;

		//printf("branchAndBound1.c :: solveTSP :: start first branch\n");
		if(solveTSP(g1, original) == -1) return -1;

		// impose e1, forbid e2
		g2->edgeList[atPosition(adjl[s1]->data, branchingNode->data)]->weight = 0;
		g2->edgeList[atPosition(adjl[s2]->data, branchingNode->data)]->weight += delta;

		//printf("branchAndBound1.c :: solveTSP :: imposing edge (%d,%d), forbidding edge (%d,%d)\n\n",
		//	adjl[s1]->data, branchingNode->data, adjl[s2]->data, branchingNode->data);
		//printf("branchAndBound1.c :: solveTSP :: start second branch\n");
		if (solveTSP(g2, original) == -1) return -1;

		// impose e1, e2, forbid all the others
		g3->edgeList[atPosition(adjl[s1]->data, branchingNode->data)]->weight = 0;
		g3->edgeList[atPosition(adjl[s2]->data, branchingNode->data)]->weight = 0;
		//printf("branchAndBound1.c :: solveTSP :: imposing edges (%d,%d), (%d,%d)\n",
		//	adjl[s1]->data, branchingNode->data, adjl[s2]->data, branchingNode->data);

		int k;
		for (k = s2+1; k < adjn; ++k) {
			if (graph->edgeList[atPosition(adjl[k]->data, branchingNode->data)]->weight > 0 &&
				graph->edgeList[atPosition(adjl[k]->data, branchingNode->data)]->weight < delta) {
				g3->edgeList[atPosition(adjl[k]->data, branchingNode->data)]->weight += delta;
			}
		}

		//printf("branchAndBound1.c :: solveTSP :: start third branch\n");
		if (solveTSP(g3, original) == -1) return -1;

	} else {

		// forbid e1
		//printf("branchAndBound1.c :: solveTSP :: forbidding edge (%d,%d)\n",
		//		adjl[s1]->data, branchingNode->data);
		g1->edgeList[atPosition(adjl[s1]->data, branchingNode->data)]->weight += delta;

		//printf("branchAndBound1.c :: solveTSP :: start first branch\n");
		if(solveTSP(g1, original) == -1) return -1;

		// impose e1
		g2->edgeList[atPosition(adjl[s1]->data, branchingNode->data)]->weight = 0;

		//printf("branchAndBound1.c :: solveTSP :: imposing edge (%d,%d)\n\n",
		//	adjl[s1]->data, branchingNode->data);
		//printf("branchAndBound1.c :: solveTSP :: start second branch\n");
		if (solveTSP(g2, original) == -1) return -1;

	}

	return 0;//-1;
}

double zincumbent = DOUBLE_INFINITY;
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

	int result = solveTSP(graph, graph);

	//printf("%f\n", zincumbent);
	//plotGraph(graph, (edge **)incumbent, pars->plotOnlyTree);

	return 0;
}
