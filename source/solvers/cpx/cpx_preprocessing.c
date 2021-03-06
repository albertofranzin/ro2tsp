#include "../../solvers/cpx/cpx_preprocessing.h"

int cpx_preprocessing(CPXENVptr	 	cplexenv,
		      	  	  CPXLPptr   	lp,
		      	  	  environment  *env,
				      parameters   *pars,
		      	  	  statistics   *stats) {

	int n 		= env->main_graph.vrtx_num;
	int *zeros 	= malloc((n * (n - 1)) / 2 *sizeof(int));
	int *ones  	= malloc((n * (n - 1)) / 2 *sizeof(int));

	int status, i;
	double best_ub;


	status = CPXsetdblparam(cplexenv, CPX_PARAM_TRELIM, 2048);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_preprocessing.c:\n"
	                    "function: cpx_preprocessing:\n"
	                    "CPXsetdblparam (CPX_PARAM_TRELIM): %d\n", status);
	    return status;
	}


	/* compute initial upper-bound */
	printf("computing upper-bound...\n");
	cycle best_cycle;
	cycle_init(&best_cycle);
	cycle_setup(&best_cycle, n);

	compute_ub(&(env->main_graph), RC23OPT, &best_cycle, &best_ub, ones, zeros);
	env->global_ub = best_ub;
	cycle_copy(&best_cycle, &(env->global_cycle));
	printf("# preprocessing : initial upper-bound         = %.2f\n", env->global_ub);


	status = CPXsetdblparam(cplexenv, CPX_PARAM_CUTUP, env->global_ub);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_preprocessing.c:\n"
	                    "function: cpx_preprocessing:\n"
	                    "CPXsetdblparam (CPX_PARAM_CUTUP): %d\n", status);
	    return status;
	}

	cycle_delete(&best_cycle);





	/* compute initial lower-bound */
	printf("computing lower-bound...\n");
	tree best_1t;
	tree_init(&best_1t);
	tree_setup(&best_1t, n);

	double *best_mults = (double*)malloc(n * sizeof(double));
	double best_lb;

	pr_lagrange_hk(env, env->global_ub, &best_1t, best_mults, &best_lb, &status);
	if (status) {
	    fprintf(stderr, "Fatal error in solvers/cpx/cpx_preprocessing.c:\n"
	                    "function: cpx_preprocessing:\n"
	                    "pr_lagrange_hk: %d\n", status);
	    return status;
	}

	if (best_lb - floor(best_lb) > EPSILON) {
		env->global_lb = ceil(best_lb);
	}
	else {
		env->global_lb = best_lb;
	}
	env->global_lb = ceil(best_lb);
	printf("# preprocessing : initial lower-bound         = %.2f\n", env->global_lb);





	/* reduce */
	printf("applying reduction...\n");
	int num_rmvedges;
	int *rmvedges  	= (int*)malloc((n * (n - 1)) / 2 * sizeof(int));

	tree_rooting(&best_1t, 0);
	reduce(&(env->main_graph), &best_1t, best_mults, best_lb,
			       env->global_ub, rmvedges, &num_rmvedges);

	printf("# preprocessing : removed edges               = %d\n",
			          	  	  	 	 	 	 	 	num_rmvedges);
	printf("# preprocessing : ratio removed edges         = %.2f\n",
			    num_rmvedges / (double)((n * (n - 1)) / 2) * 100.0);

	for (i = 0; i < num_rmvedges; i++) {
		graph_set_edge_cstr(&(env->main_graph), rmvedges[i], FORBIDDEN);
	}

	char   *lu	= (char *)calloc(num_rmvedges, sizeof(char));
	double *bd	= (double *)calloc(num_rmvedges, sizeof(double));

	for (i =  0; i < num_rmvedges; i++) {
		lu[i] = 'B';
		bd[i] = 0.0;
	}

	status = CPXchgbds(cplexenv, lp, num_rmvedges, rmvedges, lu, bd);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_preprocessing.c:\n"
	                    "function: cpx_preprocessing:\n"
	                    "CPXchgbds: %d\n", status);
	    return status;
	}

	free(best_mults);
	free(rmvedges);
	free(lu);
	free(bd);


	/* hard fixing */
	int numcols = (n * (n - 1)) / 2;
	if (pars->hardfixing_option == TRUE) {

		int    *indicesz = (int *)calloc(numcols, sizeof(int));
	    char   *luz      = (char *)calloc(numcols, sizeof(char));
	    double *bdz      = (double *)calloc(numcols, sizeof(double));
	    int    *indiceso = (int *)calloc(numcols, sizeof(int));
	    char   *luo      = (char *)calloc(numcols, sizeof(char));
	    double *bdo      = (double *)calloc(numcols, sizeof(double));
	    
	    int    *notfixed = (int *)calloc(numcols, sizeof(int));
	    int    *ind_fa   = (int *)calloc(numcols, sizeof(int));
	    char   *lu_fa    = (char *)calloc(numcols, sizeof(char));
	    double *bd_fa    = (double *)calloc(numcols, sizeof(double));

	    double *tmpvrtx  = (double *)calloc(numcols, sizeof(double));

	    int cnto = 0, cntz = 0, nf = 0, v1, v2, j;

	    for (i = 0 ; i < n ; i++) {
    		j = get_idx(env->global_cycle.vrtx_idx[i], env->global_cycle.vrtx_idx[(i+1) % n]);
    		tmpvrtx[j] = 1.0;
	    }

	    for (i = 0; i < numcols; ++i) {

	    	if (ones[i] == 1) {
	    		indiceso[cnto] = i;
	    		luo[cnto]      = 'B';
	    		bdo[cnto]      = tmpvrtx[i];//1.0;
	    		cnto++;
	        }
	    	else if (zeros[i] == 1) {
	    		indicesz[cntz] = i;
	    		luz[cntz]      = 'B';
	    		bdz[cntz]      = tmpvrtx[i];//0.0;
	    		cntz++;
	    	}
	    	else {
	    		notfixed[nf++] = i;
	    	}

	    }

	    printf("hardfixing\n");

	    int max_delta = 1000, remaining = nf, tmp, target;
	    if (nf > max_delta) {
	    	/*for (i = 0; i < nf; i++) {
	    		tmp = notfixed[i];
	    		target = rand() % remaining;
	    		notfixed[i] = notfixed[target + i];
	    		notfixed[target + i] = tmp;
	    		remaining--;
	    	}*/

	    	for (i = 0; i < nf - max_delta ; i++) {
	    		ind_fa[i]     = notfixed[i];
	    		lu_fa[i]      = 'B';
	    		bd_fa[i]      = tmpvrtx[notfixed[i]];
	    	}
	    	//getchar();
		    status = CPXchgbds(cplexenv, lp, nf - max_delta, ind_fa, lu_fa, bd_fa);
		    if (status) {
		    	fprintf(stderr, "Fatal error in solvers/cpx/cpx_preprocessing.c:\n"
		    					"function: cpx_preprocessing:\n"
		                    	"CPXchgbds: %d\n", status);
		    	return status;
		    }
	    }


	    status = CPXchgbds(cplexenv, lp, cntz, indicesz, luz, bdz);
	    if (status) {
	    	fprintf(stderr, "Fatal error in solvers/cpx/cpx_preprocessing.c:\n"
	    					"function: cpx_preprocessing:\n"
	                    	"CPXchgbds: %d\n", status);
	    	return status;
	    }

	    //printf("Additionally, %d variables are set to 0\n", cntz);
	    //printf("Additionally, %d variables are set to 1\n", cnto);

	    status = CPXchgbds(cplexenv, lp, cnto, indiceso, luo, bdo);
	    if (status) {
	    	fprintf(stderr, "Fatal error in solvers/cpx/cpx_preprocessing.c:\n"
	                      	"function: cpx_preprocessing:\n"
	    					"CPXchgbds: %d\n", status);
	    	return status;
	    }

	    free(indicesz);
	    free(luz);
	    free(bdz);
	    free(indiceso);
	    free(luo);
	    free(bdo);
	    free(notfixed);
	    free(ind_fa);
	    free(lu_fa);
	    free(bd_fa);
	    free(tmpvrtx);

	}

	return 0;
}
