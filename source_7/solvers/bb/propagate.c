#include "../../solvers/bb/propagate.h"


int propagate(environment *env, int v, int *modedges, int *modcstrs, int *nm) {

	int n 	= env->main_graph.vrtx_num;
	int k;

	if (env->main_graph.vrtx_frc[v] == 2) {
		for (k = 0; k < n; k++) {
			if (k != v			&&
				env->main_graph.edge_cstr[get_idx(v, k)] == FREE) {
				graph_set_edge_cstr(&(env->main_graph), get_idx(v, k), FORBIDDEN);
				modedges[*nm] = get_idx(v, k);
				modcstrs[*nm] = FORBIDDEN;
				*nm 		 += 1;
				propagate(env, k, modedges, modcstrs, nm);
			}
		}
	}
	else if (env->main_graph.vrtx_frb[v] == n - 3) {
		for (k = 0; k < n; k++) {
			if (k != v			&&
				env->main_graph.edge_cstr[get_idx(v, k)] == FREE) {
				graph_set_edge_cstr(&(env->main_graph), get_idx(v, k), FORCED);
				modedges[*nm] = get_idx(v, k);
				modcstrs[*nm] = FORCED;
				*nm 		 += 1;
				propagate(env, k, modedges, modcstrs, nm);
			}
		}
	}

	return 0;
}
