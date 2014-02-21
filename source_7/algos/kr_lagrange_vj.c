#include "../algos/kr_lagrange_vj.h"

int kr_lagrange_vj(environment *env, int mode, double ub, arraylist *edgelist, tree *part_1t, set *part_vs, tree *best_1t, double *best_mults, double *best_lb, int *status) {

	*status = 0;

	/*
	int cns_count = 0;
	int rxs_count = 0;
	int qks_count = 0;
	*/

	int i, j, k, st, my_edge, flag;
	double step, base_step;
	int wmin, wmax;
	double z;
	int K, M;
	int time_qks, time_rxs, time_cns;

	int n				= env->main_graph.vrtx_num;
	int	num_edges		= edgelist->size;
	int num_mstedges, num_oneedges, num_unchgedges, num_chgedges;

	int b, d, f, old_f;

	/* multipliers */
	int	*mults	= (int*)malloc(n * sizeof(int));
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
	int *weights	= (int*)malloc(((n * (n - 1)) / 2) * sizeof(int));
	int *modweights	= (int*)malloc(((n * (n - 1)) / 2) * sizeof(int));
	//int *oldweights	= (int*)malloc(((n * (n - 1)) / 2) * sizeof(int));
	int oldweight;

	num_mstedges = num_oneedges = 0;
	my_edge = edgelist->next[edgelist->capacity];
	for (i = 0; i < num_edges; i++) {

		if (get_v1(my_edge) != 0 && get_v2(my_edge) != 0) {
			mstedges[num_mstedges++] = my_edge;
		}
		else {
			oneedges[num_oneedges++] = my_edge;
		}
		weights[my_edge] = (int)rint(env->main_graph.edge_cost[my_edge]);
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

	while (num_step < max_num_step) {

		num_step++;

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
				mults[k]	= 0;
				f 			= 1;
				old_f		= 1;
			}
		}

		/* compute the cost of the 1-tree */
		z = 0.0;
		for (k = 0; k < n; k++) {
			my_edge = curr_1t.edges[k];
			z += env->main_graph.edge_cost[my_edge]
			   - (double)mults[get_v1(my_edge)] / f
			   - (double)mults[get_v2(my_edge)] / f;
		    /* why not z += weights[my_edge]??? FORCED
		     * edges in curr_1t are not stored in
		     * mstedges and their weights are not
		     * updated at each iteration.
		     */
		}
		for (k = 1; k < n; k++) {
			z += ((double)mults[k] / f) * 2;
		}

		/* update solution */
		if (z > *best_lb || num_step == 1) {
			//printf("update! z = %.2f\n", z);

			/* update best 1-tree */
			tree_copy(&curr_1t, best_1t);
			*best_lb	= z;

			for (i = 0; i < n; i++) {
				best_mults[i] = (double)mults[i] / f;
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
		//printf("step = %.8f\n", step);

		old_f = f;


		if 		(step >= 1.0					) f = 10;
		else if (step >= 0.1 &&	step < 1.0		) f = 100;
		else if (step >= 0.01 && step < 0.1		) f = 1000;
		else if (step  < 0.01					) f = 10000;

		if (step == 0.0) break;


		if (f < old_f) f = old_f;

		for (k = 1; k < n; k++) {
			if 	(curr_1t.vrtx_deg[k] != 2) {
				mults[k] *= (f / old_f);
				mults[k] += (int)rint(
				(0.5 * step * (2 - curr_1t.vrtx_deg[k]) +
				 0.5 * step * (2 - olddegs[k]))   	* f);
			}
		}

	    /* store degrees */
		for (k = 0; k < n; k++) {
			olddegs[k] = curr_1t.vrtx_deg[k];
		}

		/*
		for (k = 0; k < num_mstedges; k++) {
			oldweights[mstedges[k]] =
			weights[mstedges[k]] * (f / old_f);
		}
		*/

	    /* update edge-costs */
		for (k = 0; k < num_oneedges; k++) {
			my_edge = oneedges[k];
			i 		= get_v1(my_edge);
			j 		= get_v2(my_edge);
	    	weights[my_edge] = (int)rint(env->main_graph.edge_cost[my_edge]) * f
	    												   - mults[i] - mults[j];
		}

		num_chgedges = num_unchgedges = 0; flag = FALSE;
		for (k = 0; k < num_mstedges; k++) {

			my_edge = mstedges[k];
			i 		= get_v1(my_edge);
			j 		= get_v2(my_edge);
			oldweight 		 = weights[mstedges[k]] * (f / old_f);
		    weights[my_edge] = (int)rint(env->main_graph.edge_cost[my_edge]) * f
		    		                             	 	   - mults[i] - mults[j];

		    if (weights[my_edge] != oldweight) {
		    	unsrt_chgedges[num_chgedges++] = my_edge;

		    	if (flag == FALSE) {
					wmin = weights[my_edge];
					wmax = weights[my_edge];
					flag = TRUE;
		    	}
		    	else if (weights[my_edge] < wmin) {
		    		wmin = weights[my_edge];
		    	}
		    	else if (weights[my_edge] > wmax) {
		    		wmax = weights[my_edge];
		    	}

		    }
		    else {
		    	srt_unchgedges[num_unchgedges++] = my_edge;
		    }
		}


		M 	= num_chgedges;						/* number of edges to be sorted */
		K 	= wmax - wmin + 1;					/* number of keys */
		b 		= M;							/* base used by radix sort -> b = M good choice */
		d 		= (int)ceil(log2(K) / log2(b));	/* iteration of radix sort main loop */

		//printf("num chg = %d : %.2f\n", M, (double)num_chgedges / ((n * (n - 1)) / 2));

		time_qks = M * M;		/* quick-sort worst case time complexity */
		time_cns = 2 * M + K;	/* counting sort */
		time_rxs = 3 * M * ceil(log2(K) / log(b));


		if (d % 2 == 0) {

			if (time_qks < time_cns && time_qks < time_rxs + M) {
				//qks_count++;
				compare_sort(unsrt_chgedges, srt_chgedges, num_chgedges, weights);
			}

			else {
				if (time_rxs + M < time_cns) {
					//rxs_count++;
					radix_sort(unsrt_chgedges, srt_chgedges, num_chgedges, weights, wmin, wmax, b);
				}
				else {
					//cns_count++;
					counting_sort(unsrt_chgedges, srt_chgedges, num_chgedges, weights, wmin, wmax);
				}
			}
		}

		else {

			if (time_qks < time_cns && time_qks < time_rxs) {
				//qks_count++;
				compare_sort(unsrt_chgedges, srt_chgedges, num_chgedges, weights);
			}

			else {
				if (time_rxs < time_cns) {
					//rxs_count++;
					radix_sort(unsrt_chgedges, srt_chgedges, num_chgedges, weights, wmin, wmax, b);
				}
				else {
					//cns_count++;
					counting_sort(unsrt_chgedges, srt_chgedges, num_chgedges, weights, wmin, wmax);
				}
			}
		}

		merge(srt_chgedges, num_chgedges, srt_unchgedges, num_unchgedges, mstedges, weights);


	} /* end main for loop */

	//printf("qks = %d\n", qks_count);
	//printf("rxs = %d\n", rxs_count);
	//printf("cns = %d\n", cns_count);

	free(mults);
	free(olddegs);
	free(oneedges);
	free(mstedges);
	free(srt_unchgedges);
	free(srt_chgedges);
	free(unsrt_chgedges);
	free(weights);
	free(modweights);
	tree_delete(&curr_1t);
	set_delete(&curr_vs);

	return 0;
}
