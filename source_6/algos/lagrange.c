#include "../algos/lagrange.h"

int lagrange(graph *g, double ub, int max_iter, int max_noimp_iter, int alpha_ht, tree* t, double *lb) {

	int i, j, e, num_iter, num_noimp_iter;
	int n = g->vrtx_num;
	double a, z, w, step, sqnorm;

	double* multipliers = (double*)calloc(n, sizeof(double));	/* lagrangean multipliers vector */
	double* subgradient = (double*)calloc(n, sizeof(double));	/* subgradient vector */

	graph gtemp;
	tree ttemp;
	graph_init(&gtemp);
	tree_init(&ttemp);
	graph_copy(g, &gtemp);
	num_noimp_iter	= 0;
	num_iter 		= 0;
	a 				= ALPHA;

	while (num_iter < max_iter) {

		num_iter++;
#ifdef VERBOSE
		if (verbosity == ANNOYING) {
			fprintf(stdout, "lagrange: iter num %d\n", num_iter);
		}
#endif

		e = prim_1tree(&gtemp, &ttemp);
#ifdef DEBUG
    if (e < 0) {
        fprintf(stderr, "Error in /base/lagrange.c:\n"
                        "function: lagrange:\n"
                        "lagrange : errcode %d\n", 1);
        return -1;
    }
#endif
		tree_get_cost(&ttemp, &gtemp, &z);
		for (i = 1; i < n; i++) {
			z += 2 * multipliers[i];
		}
		if (INTEGER_OPTION == TRUE)	z = rint(z); /* maybe you should perform integer rounding on lb and not on z... */
#ifdef VERBOSE
		if (verbosity == ANNOYING) {
			fprintf(stdout, "lagrange: current lb = %.2f\n", z);
		}
#endif

		if (z > *lb || num_iter == 1) {
#ifdef VERBOSE
		if (verbosity == ANNOYING) {
			fprintf(stdout, "lagrange: UPDATE!\n");
		}
#endif
			tree_copy(&ttemp, t);
			*lb				= z;
			num_noimp_iter	= -1;
		}
		num_noimp_iter++;

		if (num_noimp_iter > max_noimp_iter) break;
	    if (num_noimp_iter > alpha_ht) a /= 2.0;

	    sqnorm = 0.0;
	    for (i = 1; i < n; i++) {
	    	subgradient[i]	= 2 - ttemp.vrtx_deg[i];
	    	sqnorm			+= subgradient[i] * subgradient[i];
	    }

	    if (sqnorm == 0.0) break;

	    step = a * (ub - z) / sqnorm; /* Held-Karp */

	    /* update lagrangean multipliers */
	    for (i = 1; i < n; i++) {
	    	multipliers[i] += step * (subgradient[i] / sqrt(sqnorm));
	    }

	    /* update edge weights with respect to the new multipliers */
	    for (i = 0; i < n; i ++) {
	    	for (j = i+1; j < n; j++) {
	    		graph_get_edge_cost(g, i, j, &w);
	    		graph_set_edge_cost(&gtemp, i, j, w - multipliers[i] - multipliers[j]);
	    	}
	    }
	} /* end for loop */

	graph_delete(&gtemp);
	tree_delete(&ttemp);
	free(subgradient);
	free(multipliers);
	return 0;

}

