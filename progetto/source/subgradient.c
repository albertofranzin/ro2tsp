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
			for (j = 1; j < i; j++) {
				set_edge_cost(&ONE_TREE, i, j, get_edge_cost(G, i, j));
			}
		}
		printf("poiuytre\n");
		values[k] = getLagrangeGraphCost(&ONE_TREE, &G, &l, cost, 0.);
		printf("23489732984\n");
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