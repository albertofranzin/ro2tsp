#include "../algos/kr_lagrange_vj.h"

int kr_lagrange_vj(environment *env, arraylist *edgelist, tree *part1t, set *partvs, int mode, double ub, tree *best1t, double *lb, double *gen_step, int *status) {

	*status = 0;

	int i, j, k, e, st, my_edge;
	double step, base_step, ratio, wmin, wmax, z, val;

	int n				= env->main_graph.vrtx_num;
	int	num_edges		= edgelist->size;
	int num_mstedges, num_oneedges, num_unchgedges, num_chgedges;

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
	tree temp1t;
	tree_init(&temp1t);
	tree_setup(&temp1t, n);

	/* current vertex-set */
	set	tempvs;
	set_init(&tempvs);
	set_setup(&tempvs, n);

	int max_num_step;
	if      (mode == INITASCENT) max_num_step	= (n * n) / 50 + n + 16;
	else if (mode == GENASCENT ) max_num_step	= n / 4 + 5;
	if 		(mode == GENASCENT ) base_step 		= *gen_step;
	int num_step		= 0;
	double val1			= 2.0 * (max_num_step - 1.0) * (max_num_step - 2.0);
	double val2			= max_num_step * (2.0 * max_num_step - 3.0);

	int type = INT;

	while (num_step < max_num_step) {

		num_step++;


	    /* compute minimum 1-tree */
		tree_copy(part1t, &temp1t);
		set_copy(partvs, &tempvs);
		kr_onetree(mstedges, num_mstedges, oneedges, num_oneedges, weights, &tempvs, &temp1t, &st);


		/* no spanning tree exists */
		if (st != 0) {
			*status = st;
			break;
		}


		if (num_step == 1) {
			for (k = 0; k < n; k++) {
				olddegs[k]	= temp1t.vrtx_deg[k];
				mults[k]	= 0.0;
			}
		}


		/* compute the cost of the 1-tree */
		z = 0.0;
		for (k = 0; k < n; k++) {
			my_edge = temp1t.edges[k];
			i 		= get_v1(my_edge);
			j 		= get_v2(my_edge);
			if (env->main_graph.edge_cstr[my_edge] == FREE) {
				z += weights[my_edge];
			}
			else {
				z += env->main_graph.edge_cost[my_edge] -
									mults[i] - mults[j];
			}
		}
		for (k = 1; k < n; k++) {
			z += mults[k] * 2.0;
		}


		/* update solution */
		if (z >= *lb || num_step == 1) {

			//printf("update! z = %d\n", z);
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


		/* stop condition */
		if (onetree_is_cycle(&temp1t)) {
			break;
		}

	    /* update step */
		step = base_step * ((num_step*num_step - 3.0*(max_num_step-1.0)*num_step + val2) / val1);
		//printf("step = %.2f\n", step);


		 /* switch from FLO to INT */
		if (step >= 1.0 && type == FLO) {
			type = INT;
		}
		 /* switch from INT to FLO */
		else if (step < 1.0 && type == INT) {
			type = FLO;
		}


	    /* update  multipliers */
		if (type == INT) {
			for (k = 1; k < n; k++) {

				/*
	   			val = 0.5 * step * (2 - temp1t.vrtx_deg[k]) + 0.5 * step * (2 - olddegs[k]);
    			if (val >= 2.0 * step) 	mults[k] += ceil(val);
    			else 					mults[k] += floor(val);
				*/
				if 	(temp1t.vrtx_deg[k] != 2) {
					mults[k] += rint(0.5 * step * (2 - temp1t.vrtx_deg[k]) +
	    						     0.5 * step * (2 - olddegs[k]));
				}






			}
		}
		if (type == FLO) { /* no rounding to integer! */
			for (k = 1; k < n; k++) {
				if 	(temp1t.vrtx_deg[k] != 2) {
					mults[k] += 0.5 * step * (2 - temp1t.vrtx_deg[k]) +
	    						0.5 * step * (2 - olddegs[k]);
				}
			}
		}

	    /* store degrees */
		for (k = 0; k < n; k++) {
			olddegs[k] = temp1t.vrtx_deg[k];
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

		//printf("num chg = %d\n", num_chgedges);
		//printf("ratio   = %.2f\n", num_chgedges / (double)(n * (n - 1) / 2));


		if (type == INT) {
			counting_sort(unsrt_chgedges, srt_chgedges, num_chgedges, weights, wmin, wmax);
		}
		if (type == FLO) {
			compare_sort(unsrt_chgedges, srt_chgedges, num_chgedges, weights);
		}

		merge(srt_chgedges, num_chgedges, srt_unchgedges, num_unchgedges, mstedges, weights);

	} /* end main for loop */

	free(mults);
	free(olddegs);
	free(oneedges);
	free(mstedges);
	free(srt_unchgedges);
	free(srt_chgedges);
	free(unsrt_chgedges);
	free(oldweights);
	free(weights);

	tree_delete(&temp1t);
	set_delete(&tempvs);

	return 0;
}
