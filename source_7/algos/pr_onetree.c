#include "../algos/pr_onetree.h"


int pr_onetree(graph *g, tree *min1t, int *status) {

	*status = 0;
	int i, n, cstr, v1, v2, st;
	double w, wmin;
	n = g->vrtx_num;

	v1 = v2 = -1;

	for (i = 1; i < n; i++) {
		cstr	= g->edge_cstr[get_idx(0, i)];
		w		= g->edge_cost[get_idx(0, i)];
		if (cstr == FREE && (v1 < 0 || w < wmin)) {
			v1		= i;
			wmin	= w;
		}
		else if (cstr == FORCED) {
			v1		= i;
			break;
		}
	}

	for (i = 1; i < n; i++) {
		if (i != v1) {
			cstr	= g->edge_cstr[get_idx(0, i)];
			w		= g->edge_cost[get_idx(0, i)];
			if (cstr == FREE && (v2 < 0 || w < wmin)) {
				v2		= i;
				wmin	= w;
			}
			else if (cstr == FORCED) {
				v2		= i;
				break;
			}
		}
	}


	if ((min1t->vrtx_deg[0] == 0 && (v1 < 0 || v2 < 0)) ||
		(min1t->vrtx_deg[0] == 1 &&  v1 < 0			  ) ){
		*status = 1;
		return 0;
	}

	pr_mst(g, min1t, v1, &st);
	if (st != 0) {
		*status = 1;
		return 0;
	}

	if (v1 >= 0) {
		tree_insert_edge(min1t, get_idx(0, v1));
		min1t->vrtx_pred[v1] = 0;
	}

	if (v2 >= 0) {
		tree_insert_edge(min1t, get_idx(0, v2));
		min1t->vrtx_pred[0] = v2;
	}

	return 0;
}
