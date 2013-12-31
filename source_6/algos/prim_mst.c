#include "../algos/prim_mst.h"


int prim_mst(graph *g, int root, tree* t) {

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

	tree_delete(t);
	tree_setup(t, n, root);

	for (i = 0; i < n; i++) {
		if (i != root) {
			visited[i] 	= FALSE;
			pred[i]		= -1;
			graph_get_edge_cstr(g, root, i, &cstr);
			if (cstr != FORBIDDEN) {
				graph_get_edge_cost(g, root, i, &w);
				pred[i] = root;
				cost[i] = w;
			}
		}
	}
	visited[root]	= TRUE;
	pred[root]		= -1;

	for (k = 0; k < n-1; k++) {

		nextVertex = -1;

		for (i = 0; i < n; i++) {
			if (visited[i] == FALSE && pred[i] >= 0) { 	/* select nextVertex among the vertices not yet visited, /*
			 	 	 	 	 	 	 	 	 	 	 	/* and which are connected to the covered part of */
														/* the graph by a not-forbidden edge */
				graph_get_edge_cstr(g, pred[i], i, &cstr);
				graph_get_edge_cost(g, pred[i], i, &w);
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


		if (nextVertex < 0) break;

		visited[nextVertex] = TRUE;
		tree_insert_edge(t, pred[nextVertex], nextVertex); /* add a new edge to the spanning tree */

		if (k < n - 2) {	/* there is at least another vertex not yet visited */
			for (i = 0; i < n; i++) {
				if (visited[i] == FALSE) {
					graph_get_edge_cstr(g, nextVertex, i, &cstr);
					graph_get_edge_cost(g, nextVertex, i, &w);
					if (cstr == FORCED || pred[i] < 0) {
						pred[i] = nextVertex;
						cost[i] = w;
					}
					else if (cstr == FREE) {
						graph_get_edge_cost(g, pred[i], i, &wmin);
						if (w < wmin) {
							pred[i] = nextVertex;
							cost[i] = w;
						}
					}
				}
			}
		}

	} /* end for (main loop)  */


#ifdef DEBUG
	if (k < n - 1) {
        fprintf(stderr, "Error in /data/prim_mst.c:\n"
        		"function: prim_mst:\n"
        		"prim_mst : errcode %d\n", 1);
        fprintf(stderr, "Info:\n"
        		"k = %d\n", k);
        return -1;
    }
#endif

	return 0;
}

