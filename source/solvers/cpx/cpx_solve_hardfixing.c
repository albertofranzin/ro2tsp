#include "../../solvers/cpx/cpx_solve_hardfixing.h"


int cpx_solve_hardfixing(CPXENVptr    cplexenv,
                         CPXLPptr     lp,
                         environment *env,
                         parameters  *pars,
                         statistics  *stats,
                         int         *zeros,
                         int         *ones,
                         double      *ret_x,
                         int          x_size,
                         int         *ret_solstat) {


	int i, j;
	//int k, solstat;
	int status = 0;
    int n = env->main_graph.vrtx_num;
    //double z;

    int		*startind = calloc(x_size, sizeof(int));
    int 	*beg      = calloc(1, sizeof(int));
    double 	*startx   = calloc(x_size, sizeof(double));
    char 	*lu       = calloc(x_size, sizeof(char));
    double 	*bd       = calloc(x_size, sizeof(double));

    //k = 0;

    /*for (i = 0; i < x_size; ++i) {
      if (zeros[i] == 1) {
        startind[k] = i;
        lu[k]       = 'B';
        bd[k]       = 0.0;
        k++;
      }
    }

    status = CPXchgbds(env, lp, k, startind, lu, bd);
    if (status) {
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_rins.c:\n"
                      "function: cpx_solve_rins:\n"
                      "CPXchgbds: %d\n", status);
      return status;
    }
    */

    free(startind);
    free(lu);
    free(bd);

    startind = calloc(x_size, sizeof(int));
    lu       = calloc(x_size, sizeof(char));
    bd       = calloc(x_size, sizeof(double));
    //k = 0;

    /*
    for (i = 0; i < x_size; ++i) {
      if (ones[i] == 1) {
        startind[k] = i;
        lu[k]       = 'B';
        bd[k]       = 1.0;
        k++;
      }
    }

    status = CPXchgbds(env, lp, k, startind, lu, bd);
    if (status) {
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_rins.c:\n"
                      "function: cpx_solve_rins:\n"
                      "CPXchgbds: %d\n", status);
      return status;
    }
    */

    free(startind);
    free(lu);
    free(bd);

    startind = calloc(x_size, sizeof(int));

    for (i = 0 ; i < n ; i++) {
    	j = get_idx(env->global_cycle.vrtx_idx[i], env->global_cycle.vrtx_idx[(i+1) % n]);
    	//idx_from_vrtx(&ce->T, ce->TOUR_HEUR.nodes[i], ce->TOUR_HEUR.nodes[(i+1) % n], &j);
    	startx[i]   = 1.0;
    	startind[i] = j;
    }
    qsort (startind, n, sizeof(int), compare_ints);

    status = CPXsetintparam(cplexenv, CPX_PARAM_ADVIND, CPX_ON);
    status = CPXaddmipstarts(cplexenv, lp, 1, n, beg, startind, startx, NULL, NULL);

    /*status = CPXsetintparam(env, CPX_PARAM_RINSHEUR, 50);
  	  status = CPXsetintparam(env, CPX_PARAM_POLISHAFTERNODE, 0);*/

    if (pars->callbacks_option == TRUE) {
    	status = cpx_solve_miliotis(cplexenv, lp, env, pars, stats,
    			        ret_x, n * (n - 1) / 2, ret_solstat);
    					//ret_x, n * (n - 1) / 2, &ret_solstat);????
    }
    else {
    	status = cpx_solve_iterative(cplexenv, lp, env, pars, stats,
    			         ret_x, n * (n - 1) / 2, ret_solstat);
						 //ret_x, n * (n - 1) / 2, &ret_solstat);????
    }

    free(beg);
    free(startx);
    free(startind);

    return status;

}
