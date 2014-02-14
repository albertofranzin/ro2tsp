#include "../solvers/vertexsel.h"

int vertexsel(environment *env, tree *t, int *v) {

	int i;
	int n = (env->main_graph).vrtx_num;

	*v = -1;
	for (i = 0; i < n; i++) {
		if (t->vrtx_deg[i] > 2) {
			*v = i;
			break;
		}
	}

	return 0;
}
