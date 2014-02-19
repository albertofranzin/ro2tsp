#include "../algos/kr_lagrange_vj.h"

int kr_lagrange_vj(environment *env, int mode, double ub, arraylist *edgelist, tree *part_1t, set *part_vs, tree *best_1t, double *best_mults, double *best_lb, int *status) {

	*status = 0;

	int i, j, k, st, my_edge;
	double step, base_step, wmin, wmax, z;

	int n				= env->main_graph.vrtx_num;
	int	num_edges		= edgelist->size;
	int num_mstedges, num_oneedges, num_unchgedges, num_chgedges;

	double beta;
	int b, sorting_mode;

	double f, modwmin, modwmax;

	int integer;

	/* multipliers */
	double	*mults	= (double*)malloc(n * sizeof(double));
	/* vertex-degrees in the previous step */
	int *olddegs	= (int*)malloc(n * sizeof(int));
	/* set of edges incident to 0 */
	int *oneedges	= (int*)malloc(n * sizeof(int));
	/* set of edges used for construction of the minimum spanning tree */
	int *mstedges	= (int*)malloc(num_edges * sizeof(int));

	int *srt_unchgedges	= (int*)malloc(num_edges * sizeof(int));
	int *srt_chgedges	= (int*)malloc(num_edges * sizeof(int));
	int *unsrt_chgedges	= (int*)malloc(num_edges * sizeof(int));

	/* weights of (possibly all) the edges */
	double *weights		= (double*)malloc(((n * (n - 1)) / 2) * sizeof(double));
	double *modweights	= (double*)malloc(((n * (n - 1)) / 2) * sizeof(double));
	double *oldweights	= (double*)malloc(((n * (n - 1)) / 2) * sizeof(double));

	num_mstedges = num_oneedges = 0;
	my_edge = edgelist->next[edgelist->capacity];
	for (i = 0; i < num_edges; i++) {

		if (get_v1(my_edge) != 0 && get_v2(my_edge) != 0) {
			mstedges[num_mstedges++] = my_edge;
		}
		else {
			oneedges[num_oneedges++] = my_edge;
		}
		weights[my_edge] = env->main_graph.edge_cost[my_edge];
		my_edge = edgelist->next[my_edge];

	}

	/* current 1-tree */
	tree curr_1t;
	tree_init(&curr_1t);
	tree_setup(&curr_1t, n);

	/* current vertex-set */
	set	curr_vs;
	set_init(&curr_vs);
	set_setup(&curr_vs, n);

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

	printf("start loop\n");
	while (num_step < max_num_step) {

		num_step++;
		printf("num step = %d\n", num_step);

	    /* compute minimum 1-tree */
		tree_copy(part_1t, &curr_1t);
		set_copy(part_vs, &curr_vs);
		kr_onetree(mstedges, num_mstedges, oneedges, num_oneedges, weights, &curr_vs, &curr_1t, &st);

		/* no spanning tree exists */
		if (st != 0) {
			*status = st;
			break;
		}

		if (num_step == 1) {
			for (k = 0; k < n; k++) {
				olddegs[k]	= curr_1t.vrtx_deg[k];
				mults[k]	= 0.0;
			}
		}

		/* compute the cost of the 1-tree */
		z = 0.0;
		for (k = 0; k < n; k++) {
			my_edge = curr_1t.edges[k];
			z += env->main_graph.edge_cost[my_edge]
						   - mults[get_v1(my_edge)]
						   - mults[get_v2(my_edge)];
		    /* why not z += weights[my_edge]??? FORCED
		     * edges in curr_1t are not stored in
		     * mstedges and their weights are not
		     * updated at each iteration.
		     */
		}
		for (k = 1; k < n; k++) {
			z += mults[k] * 2.0;
		}

		/* update solution */
		if (z > *best_lb || num_step == 1) {
			//printf("update! z = %d\n", z);

			/* update best 1-tree */
			tree_copy(&curr_1t, best_1t);
			*best_lb	= z;

			for (i = 0; i < n; i++) {
				best_mults[i] = mults[i];
			}

			if 	(mode == INITASCENT) {
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
		//printf("step = %.20f\n", step);

		if 		(step >= 1.0					)	{ sorting_mode = 0; 			}
		else if (step >= 0.1 &&	step < 1.0		) 	{ sorting_mode = 1; f = 10; 	}
		else if (step >= 0.01 && step < 0.1		)	{ sorting_mode = 1; f = 100; 	}
		else if (step >= 0.001 && step < 0.01	)	{ sorting_mode = 1; f = 1000; 	}
		//else if (step >= 0.0001 && step < 0.001	) 	{ sorting_mode = 1; f = 10000; 	}
		else if (step < 0.001					) 	{ sorting_mode = 1; f = 10000; 	}
		//else if (step < 1.0					)	{ sorting_mode = 2; 			}

		if (step == 0.0) break;

	    /* update  multipliers */
		if (sorting_mode == 0) {
			for (k = 1; k < n; k++) {
				if 	(curr_1t.vrtx_deg[k] != 2) {
					mults[k] += 0.5 * step * (2 - curr_1t.vrtx_deg[k]) +
								0.5 * step * (2 - olddegs[k]);
					mults[k]   = (int)rint(mults[k]);
				}
			}
		}
		if (sorting_mode == 1) {
			for (k = 1; k < n; k++) {
				if 	(curr_1t.vrtx_deg[k] != 2) {
					mults[k] += 0.5 * step * (2 - curr_1t.vrtx_deg[k]) +
	    						0.5 * step * (2 - olddegs[k]);
					integer   = (int)rint(mults[k] * f);
					mults[k]  = integer / f;
				}
			}
		}
		if (sorting_mode == 2) {
			for (k = 1; k < n; k++) {
				if 	(curr_1t.vrtx_deg[k] != 2) {
					mults[k] += 0.5 * step * (2 - curr_1t.vrtx_deg[k]) +
	    						0.5 * step * (2 - olddegs[k]);
				}
			}
		}

	    /* store degrees */
		for (k = 0; k < n; k++) {
			olddegs[k] = curr_1t.vrtx_deg[k];
		}

		for (k = 0; k < num_mstedges; k++) {
			my_edge = mstedges[k];
			oldweights[my_edge] = weights[my_edge];
		}

	    /* update edge-costs */
		for (k = 0; k < num_oneedges; k++) {
			my_edge = oneedges[k];
			i 		= get_v1(my_edge);
			j 		= get_v2(my_edge);
	    	weights[my_edge] = env->main_graph.edge_cost[my_edge] -
	    			                          mults[i] - mults[j];
		}

		num_chgedges = num_unchgedges = 0;

		for (k = 0; k < num_mstedges; k++) {
			my_edge = mstedges[k];
			i 		= get_v1(my_edge);
			j 		= get_v2(my_edge);
		    weights[my_edge] = env->main_graph.edge_cost[my_edge] -
		    		                          mults[i] - mults[j];
			if (k == 0 || weights[my_edge] > wmax) wmax = weights[my_edge];
			if (k == 0 || weights[my_edge] < wmin) wmin = weights[my_edge];

		    if (weights[my_edge] != oldweights[my_edge]) {
		    	unsrt_chgedges[num_chgedges++]		= my_edge;
		    }
		    else {
		    	srt_unchgedges[num_unchgedges++]	= my_edge;
		    }
		}

		printf("start sorting\n");
		if (sorting_mode == 1) {
			for (k = 0; k < num_mstedges; k++) {
				modweights[mstedges[k]] = (int)(weights[mstedges[k]] * f);
			}
			modwmin = wmin * f;
			modwmax = wmax * f;
		}

		if (sorting_mode == 0) {
			printf("start sorting 0\n");
			if (num_chgedges >= n)	b = num_chgedges;
			else					b = n;
			beta = (wmax - wmin + 1) / ((n * (n - 1)) / 2);

			if (wmax - wmin + 1 < n || beta < 4 * log2(beta) / log2(b) + 1.0) { /* counting or radix? */
				printf("counting sort\n");
				counting_sort(unsrt_chgedges, srt_chgedges, num_chgedges, weights, wmin, wmax);
			}
			else {
				printf("radix sort\n");
				radix_sort(unsrt_chgedges, srt_chgedges, num_chgedges, weights, wmin, wmax, b);
			}
		}
		if (sorting_mode == 1) {
			printf("start sorting 1\n");
			if (num_chgedges >= n)	b = num_chgedges;
			else					b = n;
			beta = (modwmax - modwmin + 1) / ((n * (n - 1)) / 2);

			if (modwmax - modwmin + 1 < n || beta < 4 * log2(beta) / log2(b) + 1.0) {
				printf("counting sort\n");
				printf("num chgedges = %d\n", num_chgedges);
				printf("modwmin = %.2f\n", modwmin);
				printf("modwmax = %.2f\n", modwmax);
				counting_sort(unsrt_chgedges, srt_chgedges, num_chgedges, modweights, modwmin, modwmax);
			}
			else {
				printf("radix sort\n");
				radix_sort(unsrt_chgedges, srt_chgedges, num_chgedges, modweights, modwmin, modwmax, b);
			}
		}
		if (sorting_mode == 2) {
			printf("start sorting 2\n");
			compare_sort(unsrt_chgedges, srt_chgedges, num_chgedges, weights);
		}

		printf("start merging\n");
		merge(srt_chgedges, num_chgedges, srt_unchgedges, num_unchgedges, mstedges, weights);

		printf("end sorting\n");

	} /* end main for loop */

	printf("end loop\n");

	free(mults);
	free(olddegs);
	free(oneedges);
	free(mstedges);
	free(srt_unchgedges);
	free(srt_chgedges);
	free(unsrt_chgedges);
	free(weights);
	free(oldweights);
	free(modweights);
	tree_delete(&curr_1t);
	set_delete(&curr_vs);

	return 0;
}
