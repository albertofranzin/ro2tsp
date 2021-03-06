#include "../../solvers/cpx/cpx_solve_proximity.h"


int cpx_solve_proximity(CPXENVptr    cplexenv,
                        CPXLPptr   	 lp,
                        environment *env,
                        parameters 	*pars,
                        statistics	*stats,
                        double    	*x,
                        int       	 x_size,
                        int       	*solstat) {


	int n 			= env->main_graph.vrtx_num;
	int numcols		= n * (n - 1) / 2;
	double theta 	= 1.0;

	double maxtime = 500;

	clock_t t1, t2;

	double *x_feas 	= malloc(numcols * sizeof(double));
	cpx_cstr cutoff;
	cpx_cstr_init(&cutoff);
	cpx_cstr_setup(&cutoff, numcols);

	int i, idx, cutoff_idx, status;
	//int j;

	if (x_size != numcols) return 1;


	for (idx = 0; idx < numcols; idx++) {
		x_feas[idx] = 0.0;
	}

	for (i = 0; i < n; i++) {
		idx = get_idx(env->global_cycle.vrtx_idx[i], env->global_cycle.vrtx_idx[(i+1) % n]);
		//idx_from_vrtx(&ce->T, ce->TOUR_HEUR.nodes[i], ce->TOUR_HEUR.nodes[(i+1) % n], &idx);
		x_feas[idx] = 1.0;
	}

	status = cpx_recenter_obj(cplexenv, lp, x_feas, numcols);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_proximity.c:\n"
						"function: cpx_solve_proximity:\n"
                    	"cpx_recenter_obj: %d\n", status);
		return 1;
	}


	status = cpx_cstr_proximity_cutoff(env, x_feas, numcols, theta, &cutoff);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_proximity.c:\n"
						"function: cpx_solve_proximity:\n"
                    	"cpx_constraint_generate_proximity_cutoff: %d\n", status);
		return 1;
	}


	status = cpx_cstr_add(cplexenv, lp, &cutoff);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_proximity.c:\n"
						"function: cpx_solve_proximity:\n"
                    	"cpx_constraint_add: %d\n", status);
		return 1;
	}

	cutoff_idx = CPXgetnumrows(cplexenv, lp) - 1;
	//printf("CUTOFF IDX = %d\n", cutoff_idx);

	int cnt;
	int    indices[numcols];
	char   lu[numcols];
	double bd[numcols];



	int iter = 0;
	double z_feas;
	int begin, end;
	while (iter < 1) {
		printf("iter = %d\n", iter);
		iter++;
		/*
	    printf("numcols = %d\n", CPXgetnumcols(env, lp));
	    printf("numrows = %d\n", CPXgetnumrows(env, lp));
	    */
	   	
	   	status = CPXsetdblparam(cplexenv, CPX_PARAM_TILIM, maxtime);

	   	t1 = clock();
	   	if (pars->callbacks_option == TRUE) {
	   		cpx_solve_miliotis(cplexenv, lp, env, pars, stats,
	   								x_feas, numcols, &status);
	   	} else {
			cpx_solve_iterative(cplexenv, lp, env, pars, stats,
					                x_feas, numcols, &status);
	   	}
	    //printf("NUMCOLS = %d\n", CPXgetnumcols(env, lp));
	    /*
	    printf("numcols = %d\n", CPXgetnumcols(env, lp));
	    printf("numrows = %d\n", CPXgetnumrows(env, lp));
	    printf("============================\n");
	    */
	   	t2 = clock();
	   	maxtime = maxtime - (double)(t2 - t1) / CLOCKS_PER_SEC;
	   	if (maxtime <= 0) {
	   		iter = 1;
	   		printf("time limit reached\n");
	   	}

		if (status == 103) {printf("ERROR!\n"); break; }
		printf("SOLSTAT = %d\n", status);
		//getchar();

		for (idx = 0; idx < numcols; idx++) {
			x[idx] = x_feas[idx];
		}
		*solstat = status;

		z_feas = 0.0;
		for (idx = 0; idx < numcols; idx++) {
			if (x_feas[idx] > 0.9) {
				//i = get_v1(idx);
				//j = get_v2(idx);
				//vrtx_from_idx(&ce->T, &i, &j, idx);
				z_feas += env->main_graph.edge_cost[idx];
				//z_feas += graph_get_edge_cost(&ce->G, i, j);
			}
		}
		printf("feas. sol = %f\n", z_feas);

		status = cpx_recenter_obj(cplexenv, lp, x_feas, numcols);
		if (status) {
			fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_proximity.c:\n"
							"function: cpx_solve_proximity:\n"
                        	"cpx_recenter_obj: %d\n", status);
			return 1;
		}

		status = cpx_update_cutoff(cplexenv, lp, env, x_feas, numcols, theta, cutoff_idx);
		if (status) {
			fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_proximity.c:\n"
							"function: cpx_solve_proximity:\n"
                        	"cpx_update_cutoff: %d\n", status);
			return 1;
		}

		begin = cutoff_idx + 1;
		end = CPXgetnumrows(cplexenv, lp) - 1;
		if (begin < end) {
			status = CPXdelrows(cplexenv, lp, begin, end);
			if (status) {
				fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_proximity.c:\n"
                                "function: cpx_solve_proximity:\n"
                                "CPXdelrows: %d\n", status);
				return 1;
			}
		}


		cnt = 0;
		for (idx =  0; idx < numcols; idx++) {
			//vrtx_from_idx(&ce->T, &i, &j, idx);
			if (env->global_lb + env->main_graph.edge_delta[idx] > z_feas + 1.0) {
			//if (ce->init_lb + graph_get_edge_delta(&ce->G, i, j) > z_feas+1) {
				indices[cnt] = idx;
				lu[cnt]      = 'B';
				bd[cnt]      = 0.0;
				cnt++;
			}
		}


		status = CPXchgbds(cplexenv, lp, cnt, indices, lu, bd);
		if (status) {
			fprintf(stderr, "Fatal error in solvers/cpx/cpx_preprocessing.c:\n"
                        	"function: cpx_preprocessing:\n"
							"CPXchgbds: %d\n", status);
			return status;
		}

		printf("# discarded fat edges = %d\n", cnt);


	} // end while (iter)

	return 0;

}


int cpx_recenter_obj(CPXENVptr cplexenv, CPXLPptr lp, double* x_feas, int x_feas_size) {

	if (x_feas_size != CPXgetnumcols(cplexenv, lp)) return 1;

	int i;
	int status;
	int rmatind[x_feas_size];
	double rmatval[x_feas_size];

	for (i = 0; i < x_feas_size; i++) {
		rmatind[i] = i;
		if (x_feas[i] > 0.9) rmatval[i] = -1.0;
		else                 rmatval[i] =  1.0;
	}

	status = CPXchgobj(cplexenv, lp, x_feas_size, rmatind, rmatval);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_proximity.c:\n"
						"function: cpx_recenter_obj:\n"
                    	"CPXchgobj: %d\n", status);
		return 1;
	}

	return 0;

}



int cpx_update_cutoff(CPXENVptr cplexenv, CPXLPptr lp, environment *env,
		double *x_feas, int x_feas_size, double theta, int cutoff_idx) {

	if (x_feas_size != CPXgetnumcols(cplexenv, lp)) return 1;


	//int i, j;
	int idx, status;
	double z_feas;

	z_feas = 0.0;
	for (idx = 0; idx < x_feas_size; idx++) {
		if (x_feas[idx] > 0.9) {
			//vrtx_from_idx(&ce->T, &i, &j, idx);
			//z_feas += graph_get_edge_cost(&ce->G, i, j);
			z_feas += env->main_graph.edge_cost[idx];
		}
	}

	int rmatind[1];
	rmatind[0] = cutoff_idx;

	double rmatval[1];
	rmatval[0] = z_feas - theta;

	status = CPXchgrhs(cplexenv, lp, 1, rmatind, rmatval);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_proximity.c:\n"
						"function: cpx_update_cutoff:\n"
                    	"CPXchgrhs: %d\n", status);
		return 1;
	}

	return  0;

}
