#include "../algos/pr_lagrange_vj.h"


int pr_lagrange_vj(environment *env, int mode, double ub, tree *best_1t, double *best_mults, double *best_lb, int *status) {

	*status = 0;

	int i, j, k, st;
	double z, step, base_step;
	int n 		= env->main_graph.vrtx_num;

	int max_num_step;
	if      (mode == INITASCENT) max_num_step	= (n * n) / 50 + n + 16;
	else if (mode == GENASCENT ) max_num_step	= n / 4 + 5;
	if 		(mode == GENASCENT) {
		base_step = 0.0;
		for (i = 0; i < n; i++) {
			base_step += fabs(best_mults[i]);
		}
		base_step = (0.5 * base_step) / n;
	}

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

	tree curr_1t;
	tree_init(&curr_1t);
	tree_setup(&curr_1t, n);

	while (num_step < max_num_step) {

		num_step++;

	    /* compute minimum 1-tree */
		tree_empty(&curr_1t);
		pr_onetree(&local_graph, &curr_1t, &st);

		/* no spanning tree exists */
		if (st != 0) {
			*status = st;
			break;
		}

		if (num_step == 1) {
			for (k = 0; k < n; k++) {
				olddegs[k] = curr_1t.vrtx_deg[k];
				mults[k]	= 0.0;
			}
		}

		/* compute the cost of the 1-tree */
		tree_get_cost(&curr_1t, &local_graph, &z);
		for (k = 1; k < n; k++) {
			z += mults[k] * 2;
		}

		/* update solution */
		if (z > *best_lb || num_step == 1) {
			//printf("%d %.4f %f\n", num_step, z, ub);

			/* update best 1-tree */
			tree_copy(&curr_1t, best_1t);
			*best_lb	= z;

			if 		(mode == INITASCENT) {
				base_step = 0.01 * z;
			}

			if (z > ub) break;

		}

		/* stop condition */
		if (onetree_is_cycle(&curr_1t)) {
			break;
		}

	    /* update step */
		step = base_step * ((num_step*num_step - 3.0*(max_num_step-1.0)*num_step + val2) / val1);
		//printf("step = %.2f\n", step);

	    /* update  multipliers */
	    for (k = 1; k < n; k++) {
	    	if 	(curr_1t.vrtx_deg[k] != 2) {
	    		mults[k] += 0.5 * step * (2 - curr_1t.vrtx_deg[k]) +
	    					0.5 * step * (2 - olddegs[k]);
	    	}
	    }

	    /* store degrees */
		for (k = 0; k < n; k++) {
			olddegs[k] = curr_1t.vrtx_deg[k];
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
	tree_delete(&curr_1t);
	free(mults);
	free(olddegs);

	return 0;
}
