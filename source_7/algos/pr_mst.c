#include "../algos/pr_mst.h"


int pr_mst(graph *g, tree* t, int root, int *status) {

	*status = 0;
	int i, j, k, cstr;
	int nextVertex;
	double w, wmin;

	int n = g->vrtx_num;

	int 	visited[n];	/* visited[v] = TRUE if vertex v has been visited */
						/* visited[v] = FALSE otherwise */
	int 	pred[n];	/* if vertex v has been visited, then pred[v] will */
						/* be the predecessor of v in the spanning tree; */
						/* if vertex v hasn't been visited yet, then pred[v] */
						/* is such that the cost of the edge (pred[v], v) is */
						/* minimum among the costs of all the edges between */
						/* some visited vertex and v */
	double 	cost[n];	/* cost[v] is the cost of the edge (pred[v], v) */


	visited[root]	= TRUE;
	pred[root]		= -1;
	for (i = 1; i < n; i++) {
		if (i != root) {
			visited[i] = FALSE;
			if (g->edge_cstr[get_idx(root, i)] != FORBIDDEN) {
				pred[i] = root;
				cost[i] = g->edge_cost[get_idx(root, i)];
			}
			else {
				pred[i] = -1;
			}
		}
	}


	for (k = 0; k < n-2; k++) {

		nextVertex = -1;

		/* select nextVertex among the vertices not yet visited and which are
		 * connected to the covered part of the graph by a not-forbidden edge */
		for (i = 1; i < n; i++) {
			if (i != root) {
				if (visited[i] == FALSE && pred[i] >= 0) {

					cstr	= g->edge_cstr[get_idx(pred[i], i)];
					w 		= g->edge_cost[get_idx(pred[i], i)];

					if (cstr == FORCED ) {
						nextVertex	= i;
						wmin		= w;
						break;
					}
					else if (nextVertex < 0 || w < wmin) {
						nextVertex	= i;
						wmin		= w;
					}

				}
			}
		}

		if (nextVertex < 0) break;

		visited[nextVertex] = TRUE;
		tree_insert_edge(t, get_idx(pred[nextVertex], nextVertex)); /* add a new edge to the spanning tree */
		t->vrtx_pred[nextVertex] = pred[nextVertex];


		for (i = 1; i < n; i++) {
			if (i != root && visited[i] == FALSE) {

				cstr	= g->edge_cstr[get_idx(nextVertex, i)];
				w		= g->edge_cost[get_idx(nextVertex, i)];

				if (cstr == FORCED || (cstr == FREE && pred[i] < 0)) {
					pred[i] = nextVertex;
					cost[i] = w;
				}
				else if (cstr == FREE) {
					if (w < g->edge_cost[get_idx(pred[i], i)]) {
						pred[i] = nextVertex;
						cost[i] = w;
					}
				}

			}
		}
	} /* end for (main loop)  */

	if (t->edge_num < t->vrtx_num - 2) {
		*status = 1;
	}
	return 0;
}

