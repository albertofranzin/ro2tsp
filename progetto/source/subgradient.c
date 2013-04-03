#include "subgradient.h"

double *subgradient(graph *G, lagrangian *l) {
	printf("aaaaa\t%d\n", G->n);
	int n = G->n,
		i, j, k,
		MAX = 1000;

	graph ONE_TREE;
	initGraph(&ONE_TREE, 1);

	double cost = heuristicBound(G, &ONE_TREE, 3),
		   *values = malloc(sizeof(double) * MAX);

	for (k = 0; k < MAX; ++k) {
		printf("initing\n");
		//initGraph(&ONE_TREE, 1);
		printf("skhfd\n");

		/* calcola 1-albero;
		*/
		compute_ot(G, &ONE_TREE);

		printf("boia\n");

		/* calcola z = costo dell'1-albero;
		*/
		for (i = 1; i <= n; i++) {
			for (j = i+1; j <= n; j++)
			  set_edge_cost(&ONE_TREE, i, j, get_edge_cost(G, i, j));
			}
		values[k] = getLagrangeGraphCost(&ONE_TREE, G, &l, cost, get_graph_cost(&ONE_TREE));
		printf(".. %f\n", values[k]);
		deleteGraph(&ONE_TREE);
		printf("kjdshjdkjshf\n");
		initGraph(&ONE_TREE, n);
		printf("wyeriuyweiuyr\n");
	}

	deleteGraph(&ONE_TREE);
	printf("exiting\n");
	return values;
}