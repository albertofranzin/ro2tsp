#include "../algos/pr_lagrange_vj.h"


int pr_lagrange_vj(environment *env, int mode, double ub, double *lb, double *gen_step, tree *best1t, int *status) {

	*status = 0;

	int i, j, k, e, st;
	double z, step, base_step;
	int n 		= env->main_graph.vrtx_num;

	int max_num_step;
	if      (mode == INITASCENT) max_num_step	= (n * n) / 50 + n + 16;
	else if (mode == GENASCENT ) max_num_step	= n / 4 + 5;

	if 		(mode == GENASCENT ) base_step 		= *gen_step;

	int num_step		= 0;
	double val1			= 2.0 * (max_num_step - 1.0) * (max_num_step - 2.0);
	double val2			= max_num_step * (2.0 * max_num_step - 3.0);

	/* multipliers */
	double *mults	= (double*)malloc(n * sizeof(double));
	/* vertex-degrees in the previous step */
	int *olddegs	= (int*)malloc(n * sizeof(int));

	graph local_graph;
	graph_init(&local_graph);
	graph_copy(&(env->main_graph), &local_graph);

	tree temp1t;
	tree_init(&temp1t);
	tree_setup(&temp1t, n);

	while (num_step < max_num_step) {

		num_step++;

	    /* compute minimum 1-tree */
		tree_empty(&temp1t);
		e = pr_onetree(&local_graph, &temp1t, &st);

		/* no spanning tree exists */
		if (st != 0) {
			*status = st;
			break;
		}

		if (num_step == 1) {
			for (k = 0; k < n; k++) {
				olddegs[k] = temp1t.vrtx_deg[k];
				mults[k]	= 0.0;
			}
		}

		/* compute the cost of the 1-tree */
		tree_get_cost(&temp1t, &local_graph, &z);
		for (k = 1; k < n; k++) {
			z += mults[k] * 2;
		}

		/* update solution */
		if (z > *lb || num_step == 1) {

			//printf("update! z = %.2f\n", z);
			/* update best 1-tree */
			tree_copy(&temp1t, best1t);
			*lb	= z;

			if 		(mode == INITASCENT) {
				/* update "base" step */
				base_step = 0.01 * z;
				/* update "base" step that will be used in general ascent */
				*gen_step = 0.0;
				for (i = 0; i < n; i++) *gen_step += fabs(mults[i]);
				*gen_step = (0.5 * *gen_step) / n;
			}
			else if (mode == GENASCENT) {
				if (z > ub) break;
			}

		}

		//break;

		/* stop condition */
		if (onetree_is_cycle(&temp1t)) {
			break;
		}

	    /* update step */
		step = base_step * ((num_step*num_step - 3.0*(max_num_step-1.0)*num_step + val2) / val1);
		//printf("step = %.2f\n", step);

	    /* update  multipliers */
	    for (k = 1; k < n; k++) {
	    	if 	(temp1t.vrtx_deg[k] != 2) {
	    		mults[k] += 0.5 * step * (2 - temp1t.vrtx_deg[k]) +
	    					0.5 * step * (2 - olddegs[k]);
	    	}
	    }

	    /* store degrees */
		for (k = 0; k < n; k++) {
			olddegs[k] = temp1t.vrtx_deg[k];
		}



	    /* update edge-costs */
		for (i = 0; i < n; i++) {
	    	for (j = i+1; j < n; j++) {
    			local_graph.edge_cost[get_idx(i, j)] =
    					env->main_graph.edge_cost[get_idx(i, j)] - mults[i] - mults[j];
	    	}
	    }


	} /* end main for loop */

	graph_delete(&local_graph);
	tree_delete(&temp1t);
	free(mults);

	return 0;
}
