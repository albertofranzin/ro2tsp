#include "../../solvers/bb/setncheck.h"


int setncheck(environment *env, arraylist *edgelist, tree *part_1t, set *part_vs, int *modedges, int *modcstrs, int *tmppreds, int *nm, int *status) {

	*status = 0;
	int n 	= env->main_graph.vrtx_num;
	int i, j, k, m, seti, setj;

	m = 0;
	for (k = 0; k < *nm; k++) {

		i = get_v1(modedges[k]);
		j = get_v2(modedges[k]);

		tmppreds[k] = edgelist->pred[modedges[k]];
		arraylist_remove(edgelist, modedges[k]);
		graph_set_edge_cstr(&(env->main_graph), modedges[k], modcstrs[k]);
		m++;

		if (modcstrs[k] == FORBIDDEN) {

			if (env->main_graph.vrtx_frb[i] > n - 3) {
				*status = 1;
				break;
			}
			if (env->main_graph.vrtx_frb[j] > n - 3) {
				*status = 1;
				break;
			}

		} /* end FORBIDDEN case */

		if (modcstrs[k] == FORCED) {

			if (env->main_graph.vrtx_frc[i] > 2) {
				*status = 1;
				break;
			}
			if (env->main_graph.vrtx_frc[j] > 2) {
				*status = 1;
				break;
			}

			if (i != 0 && j != 0) {
				set_find(part_vs, i, &seti);
				set_find(part_vs, j, &setj);

				if (seti != setj) {
					set_union(part_vs, seti, setj);
				}
				else {
					*status = 1;
					break;
				}
			}

			tree_insert_edge(part_1t, modedges[k]);

		} /* end FORCED case */

	} /* end for */

	*nm = m;

	return 0;
}
