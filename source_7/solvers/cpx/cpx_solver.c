#include "../../solvers/cpx/cpx_solver.h"

int cpx_solver(environment *env, statistics *stats, parameters *pars) {


	CPXENVptr cplexenv;
	CPXLPptr  lp;

	int status = 0;

	/* create problem */
	status = cpx_create_problem(&cplexenv, &lp, "problem");
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c:\n"
	                    "function: cpx_solver:\n"
	                    "cpx_create_problem : %d\n", status);
	    exit(1);
	}
	#ifdef DEBUG
	if (pars->verbosity >= ANNOYING) {
		printf("solvers/cpx/cpx_solver.c :: before entering cpx_setup_problem\n");
	}
	#endif

	status = cpx_setup_problem(cplexenv, lp, env, pars);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c:\n"
	                    "function: cpx_solver:\n"
	                    "cpx_setup_problem : %d\n", status);
		exit(1);
	}
	#ifdef DEBUG
	if (pars->verbosity >= ANNOYING) {
		printf("solvers/cpx/cpx_solver.c :: before entering cpx_set_parameters\n");
	}
	#endif

	status = cpx_set_parameters(cplexenv, lp, env, pars);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c:\n"
	                    "function: cpx_solver:\n"
	                    "cpx_set_parameters : %d\n", status);
	    exit(1);
	}
	#ifdef DEBUG
	if (pars->verbosity >= ANNOYING) {
		printf("solvers/cpx/cpx_solver.c :: before entering cpx_preprocessing\n");
	}
	#endif


	/* preprocessing */
	status = cpx_preprocessing(cplexenv, lp, env, pars, stats);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c:\n"
	                    "function: cpx_solver:\n"
	                    "preprocessing : %d\n", status);
	    exit(1);
	}


	/* write problem to file */
	status = CPXwriteprob(cplexenv, lp, "myprob.lp", "LP");
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c:\n"
	                    "function: cpx_solver:\n"
	                    "CPXwriteprob : %d\n", status);
	    exit(1);
	}


	/* ------------------------------------------------------------------------------------ */

	clock_t start, end;
	double time_interval;

	int my_edge, solstat;
	int n 		= env->main_graph.vrtx_num;
	int numcols = (n * (n - 1)) / 2;
	double *x	= (double *)calloc(numcols, sizeof(double));
	env->mylp = lp;

	/* call the chosen solver, among
	 * - iterative cplex
	 * - cplex with callbacks
	 * - matheuristic
	 */

	start = clock();

	if (pars->rinspolishing_option == TRUE) {
		status = cpx_solve_rins(cplexenv, lp, env, pars, stats, NULL, NULL,
											 x, n * (n - 1) / 2, &solstat);
	}
	else {

		if (pars->callbacks_option == TRUE) {
			status = cpx_solve_miliotis(cplexenv, lp, env, pars, stats,
	    		                         x, n * (n - 1) / 2, &solstat);
	    }
		else {
			status = cpx_solve_iterative(cplexenv, lp, env, pars, stats,
					                      x, n * (n - 1) / 2, &solstat);
	    }

	}

	end = clock();

	/* setup output, retrieving edges whose corresponding variable
	 * is close enough to 1 (tolerance should be much higher, but...)
	 */
	double opt = 0.0;
	tree_delete(&(env->global_1t));
	tree_setup(&(env->global_1t), n);
	for (my_edge = 0; my_edge < numcols; my_edge++) {
		if (x[my_edge] > 0.9) {
			tree_insert_edge(&(env->global_1t), my_edge);
			opt += env->main_graph.edge_cost[my_edge];
		}
	}

	time_interval = (end - start) / (double)CLOCKS_PER_SEC;
	printf("opt                 = %.2f\n", opt);
	printf("time                = %.2f\n", time_interval);
	free(x);

	/* ------------------------------------------------------------------------------------ */

	return status;
}
