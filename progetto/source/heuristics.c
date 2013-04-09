#include "heuristics.h"

/*
 * nearestNeighbour
 * - G : the original graph
 * - H : the hamiltonian path computed
 * - root : starting node
 *
 * computes a hamiltonian path in G using the Neareest Neighbour heuristic
 * According to wiki, on average it yields a path 25% longer than optimum,
 * but has an approximation factor of O(log|V|)
 *
 * return : cost of the solution
 */
double nearestNeighbour(graph *G, graph *H, int root) {
	int n = (*G).n;
	int current, j, count = 1, minIndex;
	double min, cost = 0.;

	// initialize H
	//printf("kjsdhfksdkf\n");
	deleteGraph(H);
	//printf("klshdfkjhkeshrk\n");
	initGraph(H, n);

	// declare an array of flags for recognizing visited nodes
	// probably some more sophisticated solution can save half comparisons
	short visited[n];
	memset(visited, 0, sizeof(visited));

	// mark first node as visited
	current = root;
	visited[root - 1] = 1;

	// iterate thru all the nodes, starting from node 1
	while (count < n) {
		// initialize minimum cost
		min = INF;
		minIndex = -1;

		// look for min-cost not visited neighbour
		for (j = 1; j <= n; ++j) {
			if (j != current && visited[j-1] == 0 && get_edge_cost(G, current, j) < min) {
				min = get_edge_cost(G, current, j);
				minIndex = j;
			}
		}

		// set edge cost in H, update indices
		cost += min;
		//printf("%d %d, %f | %f\n", current, minIndex, min, cost);
		set_edge_cost(H, current, minIndex, min);
		visited[minIndex-1] = 1;
		current = minIndex;
		count++;
	}

	// append last edge, for comin' home to 1
	min = get_edge_cost(G, current, root);
	set_edge_cost(H, current, root, min);
	cost += min;
	//printf("%d %d, %f | %f\n", current, 1, min, cost);

	printf("cost of the NN tour starting from node %d: %f. ", root, cost);
	// printf("May be an initial lower bound for b&b.\n");
	printf("\n");
	return cost;

}

/*
 * heuristicBound
 * - G : the original graph
 * - H : graph containing the solution given by the heuristic
 * the only heuristic tried for now is nearest neighbour
 *
 * compute a heuristic bound by calling the chosen heuristic mutiple times
 * in order to try to obtain a better value
 *
 * H is not used, I leave it hoping it will be used later
 *
 * return : bound
 */
double heuristicBound(graph *G, graph *H, int howMany) {
	int n = G->n,
		i;
	double bounds[howMany], min;

	for (i = 0; i < howMany; ++i) {
		bounds[i] = nearestNeighbour(G, H, rand() % n + 1);
	}

	min = INF;
	for (i = 0; i < howMany; ++i) {
		if (bounds[i] < min) {
			min = bounds[i];
		}
	}

	return min;

}

/*
 * nn2opt
 * - G : the original graph
 * - H : graph containing the solution given by the heuristic
 * - root : starting node
 *
 * Compute an admissible solution using the Nearest Neighbour heuristic,
 * then apply the 2-opt heuristic to swap crossing edges.
 *
 * return : cost of the solution
 */
double nn2opt(graph *G, graph *H, int root) {
	double cost = nearestNeighbour(G, H, root), delta;
	int i, j, improved = 1;

	while (improved) {
		// reset flag
		improved = 0;


		// apply algo found at : http://en.wikipedia.org/wiki/2-opt
		for (i = 0; i < G->n; ++i) {
			for (j = i+1; j < G->n - 1; ++j){
				
			}
		}
	}
}