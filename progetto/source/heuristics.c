#include "heuristics.h"

/*
 * nearestNeighbour
 * - G : the original graph
 * - H : the hamiltonian path computed
 *
 * computes a hamiltonian path in G using the Neareest Neighbour heuristic
 * According to wiki, on average it yields a path 25% longer than optimum,
 * but has an approximation factor of O(log|V|)
 *
 * return : cost of the solution
 */
double nearestNeighbour(graph *G, graph *H) {
	int n = (*G).n;
	int current, j, count = 1, minIndex;
	double min, cost = 0.;

	// initialize H
	deleteGraph(H);
	initGraph(H, n);

	// declare an array of flags for recognizing visited nodes
	// probably some more sophisticated solution can save half comparisons
	short visited[n];
	memset(visited, 0, sizeof(visited));

	// mark first node as visited
	current = 1;
	visited[0] = 1;

	// iterate thru all the nodes, starting from node 1
	while (count < n) {
		// initialize minimum cost
		min = 2000;
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
		printf("%d %d, %f | %f\n", current, minIndex, min, cost);
		set_edge_cost(H, current, minIndex, min);
		visited[minIndex-1] = 1;
		current = minIndex;
		count++;
	}

	// append last edge, for comin' home to 1
	min = get_edge_cost(G, current, 1);
	set_edge_cost(H, current, 1, min);
	cost += min;
	printf("%d %d, %f | %f\n", current, 1, min, cost);

	printf("cost of the NN tour : %f\n", cost);
	return cost;

}