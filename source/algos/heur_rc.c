#include "../algos/heur_rc.h"

int heur_rc(graph *g, cycle* c, double *ub) {

	int n = g->vrtx_num;
	int i, remaining, target, tmp;

	cycle_delete(c);
	cycle_setup(c, n);

	for (i = 0; i < n; i++) {
		c->vrtx_idx[i] = i;
	}

	remaining = n;

	/* generate a permutation of the vertices */
	for (i = 0; i < n; i++) {
		tmp = c->vrtx_idx[i];
	    target					= rand() % remaining;
	    c->vrtx_idx[i]			= c->vrtx_idx[target + i];
	    c->vrtx_idx[target + i] = tmp;
	    remaining--;
	}


	*ub = 0.0;
	for (i = 0; i < n; i++) {
		*ub += g->edge_cost[ get_idx(c->vrtx_idx[i], c->vrtx_idx[(i+1)%n]) ];
	}

	return 0;
}


