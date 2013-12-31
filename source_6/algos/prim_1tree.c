#include "../algos/prim_1tree.h"


int prim_1tree(graph *g, tree *t) {

	int e, i, imin;
	double w, wmin;

	prim_mst(g, 0, t);

	imin = -1;

	for (i = 1; i < g->vrtx_num; i++) {
		if (!tree_is_edge(t, 0, i)) {
			graph_get_edge_cost(g, 0, i, &w);
			if (imin < 0 || w < wmin) {
				imin = i;
				wmin = w;
			}
		}
	}

	e = tree_insert_edge(t, imin, 0);
#ifdef DEBUG
	if (e < 0) {
        fprintf(stderr, "Error in /data/prim_1tree.c:\n"
        		"function: prim_1tree:\n"
        		"prim_1tree : errcode %d\n", 1);
        return -1;
    }
#endif
	return 0;
}
