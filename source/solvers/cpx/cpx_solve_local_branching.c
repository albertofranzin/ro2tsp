#include "../../solvers/cpx/cpx_solve_local_branching.h"


int cpx_solve_local_branching(CPXENVptr   	 cplexenv,
                              CPXLPptr    	 lp,
                              environment	*env,
                              parameters 	*pars,
                              statistics  	*stats,
                              double     	*ret_x,
                              int         	 x_size,
                              int        	*ret_solstat) {


	int n 		= env->main_graph.vrtx_num;
	int numcols = n * (n - 1) / 2;
	int lbc_counter = 0;
	int i, j, idx, status;
	//int k, iter, cid_counter;
	clock_t node_time_limit;
	//clock_t total_time_limit, elapsed_time, start_time, now;

	double rhs = 10;//ce->init_ub;

	int 	*ms_indices	= malloc(numcols * sizeof(int));
	int 	*const_inds = malloc(numcols * sizeof(int));
	double 	*x_feas 	= malloc(numcols * sizeof(double));
	double 	*startx 	= calloc(n, sizeof(double));
	int    	*stindx 	= calloc(n, sizeof(int));

	if (x_size != numcols) return 1;


	for (idx = 0; idx < numcols; idx++) {
		x_feas[idx]     = 0.0;
	    const_inds[idx] = 0;
	    ms_indices[idx] = idx; // originale:  ms_indices[idx] = i; ma i NON INIZIALIZZATA!
	}

	for (i = 0; i < n; i++) {
		idx = get_idx(env->global_cycle.vrtx_idx[i], env->global_cycle.vrtx_idx[(i+1) % n]);
		//idx_from_vrtx(&ce->T, ce->TOUR_HEUR.nodes[i], ce->TOUR_HEUR.nodes[(i+1) % n], &idx);
	    x_feas[idx]	= 1.0;
	    startx[i] 	= 1.0;
	    stindx[i] 	= idx;
	    //printf("%d %d %d %d\n", i, ce->TOUR_HEUR.nodes[i], ce->TOUR_HEUR.nodes[(i+1) % n], idx);
	}

	qsort (stindx, n, sizeof(int), compare_ints);

	/*
	for (i = 0; i < n; i++) {
	   	printf("%d\n", stindx[i]);
	}
	*/

	int beg[1];
	beg[0] = 0;
	status = CPXsetintparam(cplexenv, CPX_PARAM_ADVIND, CPX_ON);
	status = CPXaddmipstarts(cplexenv, lp, 1, n, beg, stindx, startx, NULL, NULL);

	// --------------------------------------------------------------
	//
	// solver : main cycle
	//
	// --------------------------------------------------------------

	// leave callbacks out for now
	// leave kruskal secs out for now

	// repeat following cycle until problem is solved:
	// * try to solve the problem:
	//   + solve the relaxated problem
	//   + look for subtours, add relative constraints
	//   + iterate until problem is solved, unfeasible or timed-out
	// * add local branching constraint
	// * iterate until problem is solved, timed-out, or infeasible
	// * based on the solution status, add/remove local branching constraints,
	//   and iterate
	int 	termination = FALSE; // cycle control
	int 	solstat;             // solution status code
	                             // (see appendix B for explanation)
	double 	x[numcols];          // solution coefficients
	double 	objval;              // solution value (cost)

	// parameters for directing local branching
	//int opt       = TRUE;
	int first     = TRUE;
	int diversify = FALSE;
	int dv        = 0;
	//int dv_max    = 100000; // ????
	int lcr;                // last constraint row


	// radius of the neighbourhood - the k = 10, 20 of the paper
	int    radius = 20;

	//double rhs        = BIG;
	//double tl         	= BIG;
	//double ub         	= env->global_ub;
	double best_ub    	= env->global_ub;
	double *x_opt		= malloc(sizeof(double)*numcols);
	double *cur_x_opt 	= malloc(sizeof(double)*numcols);

	memset(x_opt, 0., sizeof(double)*numcols);
	memcpy(cur_x_opt, x_feas, sizeof(double)*numcols);
	// set time controls
	//start_time = clock();


	// cycle begins!
	clock_t t1, t2;
	double maxtime = 200;
	while (!termination) {

		if (rhs < BIG) {
			// printf("creating constraint\n");
			// cpx_constraint *cpxc = cpx_create_lb_constraint(cur_x_opt,
			//                                                 numcols,
			//                                                 cid_counter++,
			//                                                 'L',
			//                                                 rhs);
			// printf("done\n");
			// status = cpx_add_lb_constraint(env, lp, cpxc, pars);
			// assert(status == 0);
			// printf("asserted!\n");
		}

		//tl = fmin(tl, total_time_limit - elapsed_time);

		//printf("Set node time limit to CPLEX: %f\n", node_time_limit); // commentato: node_time_limit NON INIZIALIZZATO
		status = CPXsetdblparam(cplexenv, CPX_PARAM_TILIM, 500);
		if (status) {
			fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_local_branching.c :: ");
		    fprintf(stderr, "failed to set the timeout, error %d.\n", status);
		    exit(1);
		}
		t1 = clock();
	    // status = cpx_solve_iterative(cplexenv, lp, env, pars, stats,
	    // 		                     x_opt, numcols, &solstat);
	    status = cpx_solve_miliotis(cplexenv, lp, env, pars, stats,
	    		                     x_opt, numcols, &solstat);
	    t2 = clock();
	    maxtime = maxtime - (double)(t2 - t1) / CLOCKS_PER_SEC;
	    if (maxtime <=0) {
	    	printf("time limit reached\n");
	    	termination = TRUE;
	    	break;
	    }
	    //getchar();

	    //int numsubtrs = 0;
	    //while (numsubtrs != 1) {
	    /*
		#ifdef DEBUG
	    if (pars->verbosity >= ESSENTIAL) {
	    	printf("about to solve the problem\n");
	    }
		#endif
		*/

	    // solve the MIP problem
	    /*
	    status = CPXmipopt(env, lp);
	    if (status) {
	        fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_local_branching.c :: ");
	        fprintf(stderr, "CPXmipopt : %d\n", status);
	        fprintf(stderr, "Error while solving the problem.\n");
	        exit(1);
	    }
	    */


	    // retrieve solution status: can we continue or not?
	    //solstat = CPXgetstat(env, lp);
	    /*
		#ifdef DEBUG
	    	if (pars->verbosity >= ESSENTIAL) {
	        // getchar();
	    }
		#endif
		*/
	    // printf("solution status is %d\n", solstat);
	    // if no suitable solution is found, then stop here
	    // else, retrieve current solution and follow on adding SECs
	    if (solstat == 107 ||  // timed out, integer solution exists
	    	solstat == 108 ||  // timed out, integer solution does not exist
	    	solstat == 103  ) {// problem infeasible

	    	// terminate cycle
	    	//  numsubtrs = 1;
	    }
	    else if (solstat == 101 ||
	             solstat == 102  ) {

	    	// retrieve solution coefficients
	        status = CPXgetx(cplexenv, lp, x, 0, numcols-1);
	        if (status) {
	        	fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
	        	fprintf(stderr, "CPXgetx : %d\n", status);
	        	fprintf(stderr, "Failed to get node solution.\n");
	        	exit(1);
	        }
	        // add SECs for the current solution
	        // numsubtrs = cpx_add_secs(env, lp, x, numcols, &ce->T, pars);
	        //cpx_constraint sec;
	        //cpx_constraint_init(&sec, numcols);
	    }

	    //}  // end while num of subtours != 1

	    //tl = node_time_limit;

	    switch(solstat) {

	    case 101: // optimal solution found
	    case 102: // optimal solution found within tolerance
	    	status = CPXgetobjval(cplexenv, lp, &objval);
	    	if (status) {
	    		fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
	    		fprintf(stderr, "case %d :: CPXgetobjval : %d\n", solstat, status);
	    		fprintf(stderr, "Failed to obtain objective value.\n");
	    		exit(1);
	    	}
	    	printf("objval = %f (vs %f)\n", objval, best_ub);
	    	if (objval < best_ub) {
	    		best_ub = objval;
	    		//printf("a %d\n", sizeof(x));
	    		memcpy(x_opt, x, sizeof(x));
	    		//printf("b\n");
	    	}
	    	if (rhs >= BIG) {
	    		//printf("101/102: rhs >= BIG\n");
	    		termination = TRUE;
	    		break;
	    	}
	    	diversify = FALSE;
	    	first     = FALSE;
	    	//ub        = objval;
	    	rhs       = radius;
	    	memcpy(cur_x_opt, x, sizeof(x));
	    	// reverse last constraint
	    	//printf("101/102: before getnumrows\n");
	    	lcr = CPXgetnumrows(cplexenv, lp);
	    	char asense[1];
	    	int inds[1];
	    	asense[0]	= 'G';
	    	inds[0] 	= lcr-1;
	    	//printf("101/102: before chgsense\n");
	    	status = CPXchgsense(cplexenv, lp, 1, inds, asense);
	    	if (status) {
	    		fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
	    		fprintf(stderr, "case 101 :: CPXchgsense : %d\n", status);
	    		fprintf(stderr, "Failed to invert constraint.\n");
	    		exit(1);
	    	}
	    	double arhs[1];
	    	arhs[0] = rhs + 1;
	    	//printf("101/102: before chgrhs\n");
	    	status = CPXchgrhs(cplexenv, lp, 1, inds, arhs);
	    	if (status) {
	    		fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
	    		fprintf(stderr, "case 101 :: CPXchgrhs : %d\n", status);
	    		fprintf(stderr, "Failed to invert constraint.\n");
	    		exit(1);
	    	}
	    	//printf("101/102 : well done\n");
	    break;

	    case 103: // problem is infeasible
	    	if (rhs >= BIG) {
	    		termination = TRUE;
	    		break;
	    	}
	    	// reverse last local branching constraint
	    	if (lbc_counter > 0) {
	    		/*cpx_constraint *outc = cpx_cpool_pop_last(round1_cpool);
	              outc->rhs[0] = outc->rhs[0]+1;
	              if (outc->sense[0] == 'L') {
	              outc->sense[0] = 'G';
	              }
	              else {
	              	  outc->sense[0] = 'L';
	                  }
	                  cpx_cpool_insert(round2_cpool, outc);*/
	    		lcr = CPXgetnumrows(cplexenv, lp);
	    		char asense[1];
	    		int inds[1];
	    		asense[0]	= 'G';
	    		inds[0] 	= lcr-1;
	    		status 		= CPXchgsense(cplexenv, lp, 1, inds, asense);
	    		if (status) {
	    			fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
	    			fprintf(stderr, "case 103 :: CPXchgsense : %d\n", status);
	    			fprintf(stderr, "Failed to invert constraint.\n");
	    			exit(1);
	    		}
	    		double arhs[1];
	    		arhs[0] = rhs + 1;
	    		status 	= CPXchgrhs(cplexenv, lp, 1, inds, arhs);
	    		if (status) {
	    			fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
	    			fprintf(stderr, "case 103 :: CPXchgrhs : %d\n", status);
	    			fprintf(stderr, "Failed to invert constraint.\n");
	    			exit(1);
	    		}
	    	}
	    	if (diversify) {
	    		//ub = BIG;
	    		//tl = BIG;
	    		dv++;
	    		first = TRUE;
	    	}
	    	rhs = rhs + ceil(radius / 2);
	    	diversify = TRUE;
	    break;

	    case 107: // problem has timed out, a feasible solution has been found
	    	if (rhs < BIG) {
	    		/*printf("rhs < BIG\n");
	                 if (round1_cpool->size > 0) {
	                    cpx_constraint *outc = cpx_cpool_pop_last(round1_cpool);
	                  }
	                  printf("last popped\n");*/
	    		lcr = CPXgetnumrows(cplexenv, lp);
	                  // printf("# of rows: %d\n", lcr);
	    		if (first) {
	                    // printf("ciao\n");
	    			status = CPXdelrows(cplexenv, lp, lcr-1, lcr-1);
	    			if (status) {
	    				fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
	    				fprintf(stderr, "case 107 :: CPXdelrows : %d\n", status);
	    				fprintf(stderr, "Failed to delete constraint.\n");
	    				exit(1);
	    			}
	    		}
	    		else {
	    			// don't know what that 'refine' is,
	    			// so I follow the advice and do NOT execute
	    			// the following 'else' branch
	    			/*char asense[1];
	                    asense[0] = 'G';
	                    int inds[1];
	                    inds[0] = lcr-1;
	                    status = CPXchgsense(env, lp, 1, inds, asense);
	                    if (status) {
	                      fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
	                      fprintf(stderr, "case 107 :: CPXchgsense : %d\n", status);
	                      fprintf(stderr, "Failed to invert constraint.\n");
	                      exit(1);
	                    }
	                    int arhs[1];
	                    arhs[0] = rhs + 1;
	                    status = CPXchgrhs(env, lp, 1, inds, arhs);
	                    if (status) {
	                      fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
	                      fprintf(stderr, "case 107 :: CPXchgrhs : %d\n", status);
	                      fprintf(stderr, "Failed to invert constraint.\n");
	                      exit(1);
	                    }*/
	    		} // end if !first
	    	} // end if rhs < BIG
	    	// refine ?
	    	printf("try to get value\n");
	    	status = CPXgetobjval(cplexenv, lp, &objval);
	    	if (status) {
	    		fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
	    		fprintf(stderr, "case 107 :: CPXgetobjval : %d\n", status);
	    		fprintf(stderr, "Failed to obtain objective value.\n");
	    		exit(1);
	    	}
	    	// printf("eh?\n");
	    	if (objval < best_ub) {
	    		best_ub = objval;
	    		memcpy(x_opt, x, sizeof(x));
	    	}
	    	// printf("mah\n");
	    	first      = FALSE;
	    	diversify  = FALSE;
	    	//ub  = objval;
	    	rhs = radius;
	    	// printf("boh\n");
	    	memcpy(cur_x_opt, x, sizeof(x));
	    	// printf("già\n");
	    break;

	    case 108: // problem infeasible, no feasible solutions found
	    	printf("diversify = %d\n", diversify);
	    	if (diversify) {
	    		/*if (round1_cpool->size > 0) {
	                    cpx_constraint *outc = cpx_cpool_pop_last(round1_cpool);
	                    outc->rhs[0] = outc->rhs[0]+1;
	                    if (outc->sense[0] == 'L') {
	                      outc->sense[0] = 'G';
	                    } else {
	                      outc->sense[0] = 'L';
	                    }
	                    cpx_cpool_insert(round2_cpool, outc);*/
	    		lcr = CPXgetnumrows(cplexenv, lp);
	    		char asense[1];
	    		int inds[1];
	    		asense[0]	= 'G';
	    		inds[0]	 	= lcr-1;
	    		status 		= CPXchgsense(cplexenv, lp, 1, inds, asense);
	    		if (status) {
	    			fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
	    			fprintf(stderr, "case 108 :: CPXchgsense : %d\n", status);
	    			fprintf(stderr, "Failed to invert constraint.\n");
	    			exit(1);
	    		}
	    		double arhs[1];
	    		arhs[0] 	= rhs + 1;
	    		status		= CPXchgrhs(cplexenv, lp, 1, inds, arhs);
	    		if (status) {
	    			fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
	    			fprintf(stderr, "case 108 :: CPXchgrhs : %d\n", status);
	    			fprintf(stderr, "Failed to invert constraint.\n");
	    			exit(1);
	    		}
	    		//}
	    		//ub = BIG;
	    		//tl = BIG;
	    		dv++;
	    		rhs = rhs + ceil(radius / 2);
	    		first = TRUE;
	    	}
	    	else {
	                  /*if (round1_cpool->size > 0) {
	                    cpx_constraint *outc = cpx_cpool_pop_last(round1_cpool);
	                  }*/
	    		lcr = CPXgetnumrows(cplexenv, lp);
	    		status = CPXdelrows(cplexenv, lp, lcr-1, lcr-1);
	    		if (status) {
	    			fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
	    			fprintf(stderr, "case 108 :: CPXdelrows : %d\n", status);
	    			fprintf(stderr, "Failed to delete constraint.\n");
	    			exit(1);
	    		}
	    		rhs = rhs - ceil(radius / 2);
	    	}
	    	diversify = TRUE;
	    break;

	    } // end switch

	    // check time
	    //now = clock();
	    //printf("before time_elapsed()\n");
	   //elapsed_time = time_elapsed(start_time, now);

	    /*
	    if (elapsed_time > total_time_limit || dv > dv_max) { // commentato: total_time_limit NON INIZIALIZZATO
	    	termination = TRUE;
	    }
	    */

	    //printf("before second addmipstarts\n");
	    j = 0;
	    for (i = 0; i < numcols; i++) {
	    	if (x_opt[i] >= 0.99) {
	    		startx[j] = 1.0;
	    		stindx[j++] = i;
	    		//printf("%d %d %d %d\n", i, ce->TOUR_HEUR.nodes[i], ce->TOUR_HEUR.nodes[(i+1) % n], idx);
	    	}
	    }
	    //int mipstartindices[1] = {0};
	    // status = CPXchgmipstarts(env, lp, 1, mipstartindices, n, beg, stindx, startx, NULL);
	    status = CPXaddmipstarts(cplexenv, lp, 1, n, beg, stindx, startx, NULL, NULL);
	    //printf("after second addmipstarts\n");

	} // end while termination

	//printf("Set node time limit to CPLEX: %f\n", node_time_limit); // commentato: node_time_limit type clock_t non double
	node_time_limit = 100; // non presente nell'originale: nvalore a caso, altrimenti node_time_limit NON INIZIALIZZATO
	status = CPXsetdblparam(cplexenv, CPX_PARAM_TILIM, node_time_limit);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_local_branching.c :: ");
	    fprintf(stderr, "failed to set the timeout, error %d.\n", status);
	    exit(1);
	}


	/*  status = CPXmipopt(env, lp);
	  if (status) {
	    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_local_branching.c :: ");
	    fprintf(stderr, "CPXmipopt : %d\n", status);
	    fprintf(stderr, "Error while solving the problem.\n");
	    exit(1);
	  }*/

	  // Retrieve objective function final value.
	status = CPXgetobjval(cplexenv, lp, &objval);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_local_branching.c :: ");
	    fprintf(stderr, "CPXgetobjval : %d\n", status);
	    fprintf(stderr, "Failed to obtain objective value.\n");
	    //exit(1);
	}

	//cs->z_opt = objval;
	printf("# problem solved!\n");

	assert(numcols == CPXgetnumcols(env, lp));

	solstat = CPXgetstat(cplexenv, lp);
	if (solstat == 101 ||
		solstat == 102 ||
		solstat == 103) {
		//opt = TRUE;
	}
	else {
		//opt = FALSE;
	}

	if (solstat == 101 ||
		solstat == 102) {
	    	status = CPXgetx(cplexenv, lp, x_opt, 0, numcols-1);
	    	if (status) {
	    		fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
	    		fprintf(stderr, "CPXgetx : %d\n", status);
	    		fprintf(stderr, "Failed to get node solution.\n");
	    		exit(1);
	    	}
	}

	// Save tour...
	//graph_delete(&ce->G_OUTPUT); graph_init(&ce->G_OUTPUT, n);


	// OCCHIO: COMMENTATO, DECOMMENTA E DECIDI SE E DOVE SALVARE IL TOUR OTTIMO
	/*
	for (k = 0; k < numcols; k++) {

		if (x_opt[k] > 0.9) {

			vrtx_from_idx(&ce->T, &i, &j, k);
			//printf("%d %d\n", i,j);

	//#ifdef DEBUG
	//      if (pars->verbosity >= USEFUL) {
	//        printf("(%d,%d)\n", i, j);
	//      }
	//#endif
	      graph_insert_edge(&ce->G_OUTPUT, i, j, graph_get_edge_cost(&ce->G_INPUT, i, j), FREE);

	    }
	  }
	*/

	*ret_x = *x_opt;
	*ret_solstat = 101;

	return SUCCESS;

}
