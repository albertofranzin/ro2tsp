#include "../solvers/edgesel.h"

int edgesel(environment *env, tree *t, int v, int *e1, int *e2) {


	int i;
	int my_edge;
	int n = (env->main_graph).vrtx_num;

	*e1 = -1;
	*e2 = -1;
	for (i = 0; i < n+1; i++) {
		my_edge = t->edges[i];
		if ((get_v1(my_edge) == v || get_v2(my_edge) == v) && (env->main_graph).edge_cstr[my_edge] == FREE) {
			if(*e1 < 0) {
				*e1 = my_edge;
			}
			else {
				*e2 = my_edge;
				break;
			}
		}
	}

	return 0;
}
