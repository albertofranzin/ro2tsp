#include "../algos/pr_lagrange_hk.h"

int pr_lagrange_hk(environment *env, double ub, tree* t, double *best_mults, double *best_lb, int *status) {

	*status = 0;
	int integer;

	int i, j, st;
	int n 				= (env->main_graph).vrtx_num;

	double alpha		= 2.0;
	int num_step		= 0;
	int num_noimprov 	= 0;
	int max_num_step 		= (n * n) / 2 + n;
	if (n > 350) {
		max_num_step = 50000;
	}
	int max_num_noimprov 	= max_num_step;
	int alpha_ht			= (int)(ceil((double)max_num_step / (log2(alpha) - log2(0.001))) / 2.0);

	double z, step, sqnorm;


	double 	*mults		= (double*)calloc(n, sizeof(double));

	graph local_graph;
	graph_init(&local_graph);
	graph_copy(&(env->main_graph), &local_graph);

	tree curr_1t;
	tree_init(&curr_1t);
	tree_setup(&curr_1t, n);


	while (num_step < max_num_step) {

		num_step++;

		//if (num_step % 1000 == 0) printf("num step = %d\n", num_step);

	    /* compute minimum 1-tree */
		tree_empty(&curr_1t);
		pr_onetree(&local_graph, &curr_1t, &st);

		/* no spanning tree exists */
		if (st != 0) {
			*status = st;
			break;
		}

		/* compute the cost of the 1-tree */
		z = 0.0;
		for (i = 0; i < n; i++) {
			z += local_graph.edge_cost[curr_1t.edges[i]];
		}
		for (i = 1; i < n; i++) {
			z += mults[i] * 2;
		}

		/* update solution */
		if (z > *best_lb || num_step == 1) {
			//printf("update! z = %.2f\n", z);
			for (i = 0; i < n; i++) {
				best_mults[i] = mults[i];
			}
			tree_copy(&curr_1t, t);
			*best_lb		= z;
			num_noimprov	= -1;
			//printf("pr_lagrange_hk %d %.6f %f\n", num_step, z, ub);

		}
		num_noimprov++;

		/* stop condition */
		if (num_noimprov > max_num_noimprov) break;
	    if (num_noimprov > alpha_ht) {
	    	alpha /= 2.0;
	    	//alpha_ht *= 2; /* mod */
	    }

	    sqnorm = 0.0;
	    for (i = 1; i < n; i++) {
	    	sqnorm += (2 - curr_1t.vrtx_deg[i]) * (2 - curr_1t.vrtx_deg[i]);
	    }
	    if (sqnorm == 0.0) break;

	    /* update step */
	    step = alpha * (ub - z) / sqnorm;

	    /* update  multipliers */
	    for (i = 0; i < n; i++) {
	    	mults[i] += step * ((2 - curr_1t.vrtx_deg[i]) / sqrt(sqnorm));
			integer = (int)(mults[i] * 10000.0);
			mults[i] = integer / 10000.0;
	    }

	    /* update edge-costs */
	    for (i = 0; i < n; i++) {
	    	for (j = i+1; j < n; j++) {
	    		local_graph.edge_cost[get_idx(i, j)] =
						env->main_graph.edge_cost[get_idx(i, j)] - mults[i] - mults[j];
	    	}
	    }


	} /* end main for loop */

	free(mults);
	graph_delete(&local_graph);
	tree_delete(&curr_1t);

	return 0;
}
