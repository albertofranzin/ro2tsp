#include "../../solvers/bb/edgesel.h"

int edgesel(environment *env, tree *t, int v, int *e1, int *e2) {

	int i;
	int my_edge;
	int n = (env->main_graph).vrtx_num;

	*e1 = -1;
	*e2 = -1;
	for (i = 0; i < n; i++) {
		my_edge = t->edges[i];

		if (get_v1(my_edge) == v || get_v2(my_edge) == v) {

			if ((env->main_graph).edge_cstr[my_edge] == FREE) {

				*e1 = my_edge;
				break;

			}
		}
	}
	for (i = 0; i < n; i++) {
		my_edge = t->edges[i];

		if (get_v1(my_edge) == v || get_v2(my_edge) == v) {

			if (my_edge != *e1 && (env->main_graph).edge_cstr[my_edge] == FREE) {

				*e2 = my_edge;
				break;

			}
		}
	}

	return 0;
}
