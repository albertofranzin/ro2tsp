#include "../algos/heur_nn.h"

int heur_nn(graph *g, cycle* c, int start, double *ub) {

	int n = g->vrtx_num;
	int i, k, curr, next;
	double min;

	cycle_delete(c);
	cycle_setup(c, n);

	int* visited = (int*)calloc(n, sizeof(int));

	// Main loop: at each step a new vertex is visited and a new edge
	// is added to the solution.
	c->vrtx_idx[0]	= start;
	curr 			= start;
	visited[start] 	= 1;

	for (k = 1; k < n; k++) {
		min = -1.0;

		for (i = 0; i < n; i++) {
			if (visited[i] == 0 &&
			   (min < 0.0 || g->edge_cost[get_idx(curr, i)] < min)){
				next 	= i;
				min 	= g->edge_cost[get_idx(curr, i)];
			}
		}

		visited[next]		= 1;
		c->vrtx_idx[k]	= next;
		curr 				= next;

	} /* end for loop */

	*ub = 0.0;
	for (i = 0; i < n; i++) {
		*ub += g->edge_cost[ get_idx(c->vrtx_idx[i], c->vrtx_idx[(i+1)%n]) ];
	}

	return 0;
}
