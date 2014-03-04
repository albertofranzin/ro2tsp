#include "../../solvers/cpx/cpx_solve_miliotis.h"

int cpx_solve_miliotis(CPXENVptr   	cplexenv,
                       CPXLPptr    	lp,
                       environment *env,
                       parameters  *pars,
                       statistics  *stats,
                       double      *x,
                       int          x_size,
                       int         *solstat) {


	int status = 0;

	int n       = env->main_graph.vrtx_num;
	int numcols = n * (n - 1) / 2;
	//int vrtx_comp[n];

	//status = CPXsetintparam (env, CPX_PARAM_MIPSEARCH, CPX_MIPSEARCH_DYNAMIC);
	status = CPXsetintparam (cplexenv, CPX_PARAM_MIPSEARCH, CPX_MIPSEARCH_TRADITIONAL);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
						"function: cpx_solve_miliotis:\n"
	                    "CPXsetinparam (CPX_PARAM_MIPSEARCH) : %d\n", status);
	    return status;
	}

	//status = CPXsetintparam (env, CPX_PARAM_PRELINEAR, CPX_ON);
	status = CPXsetintparam (cplexenv, CPX_PARAM_PRELINEAR, CPX_OFF);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
	                    "function: cpx_solve_miliotis:\n"
	                    "CPXsetinparam (CPX_PARAM_PRELINEAR) : %d\n", status);
		return status;
	}

	//status = CPXsetintparam (env, CPX_PARAM_REDUCE, CPX_PREREDUCE_NOPRIMALORDUAL);
	status = CPXsetintparam (cplexenv, CPX_PARAM_REDUCE, CPX_PREREDUCE_PRIMALANDDUAL);
	//status = CPXsetintparam (env, CPX_PARAM_REDUCE, CPX_PREREDUCE_PRIMALONLY);
	//status = CPXsetintparam (env, CPX_PARAM_REDUCE, CPX_PREREDUCE_DUALONLY);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
	                    "function: cpx_solve_miliotis:\n"
	                    "CPXsetinparam (CPX_PARAM_REDUCE) : %d\n", status);
	    return status;
	}

	status = CPXsetintparam (cplexenv, CPX_PARAM_MIPCBREDLP, CPX_ON);
	//status = CPXsetintparam (env, CPX_PARAM_MIPCBREDLP, CPX_OFF);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
	                    "function: cpx_solve_miliotis:\n"
	                    "CPXsetinparam (CPX_PARAM_MIPCBREDLP) : %d\n", status);
	    return status;
	}

	/**/
	status = CPXsetintparam (cplexenv, CPX_PARAM_PREDUAL, -1);
	//status = CPXsetintparam (env, CPX_PARAM_PREDUAL, 0);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c:\n"
	                    "function: cpx_set_problem_parameters:\n"
	                    "CPXsetinparam (CPX_PARAM_PREDUAL) : %d\n", status);
	    return status;
	}

	/*
	  //status = CPXsetdblparam(env, CPX_PARAM_CUTUP, ce->init_ub);
	  if (status) {
	    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
	                    "function: cpx_solve_miliotis:\n"
	                    "CPXsetdblparam (CPX_PARAM_CUTUP) : %d\n", status);
	    return status;
	  }

	  status = CPXsetdblparam(env, CPX_PARAM_CUTLO, ce->init_lb);
	  if (status) {
	    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
	                    "function: cpx_solve_miliotis:\n"
	                    "CPXsetdblparam (CPX_PARAM_CUTLO) : %d\n", status);
	    return status;
	  }
	  */

	if (x_size != numcols) return 1;

	status = CPXsetlazyconstraintcallbackfunc(cplexenv, cpx_callback_miliotis, env);
	//status = CPXsetincumbentcallbackfunc(env, cpx_callback_miliotis, ce);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
						"function: cpx_solve_miliotis:\n"
						"CPXsetlazyconstraintcallbackfunc : %d\n", status);
		return 1;
	}
	// status = CPXsetusercutcallbackfunc(cplexenv, cpx_callback_maxflow, env);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
						"function: cpx_solve_miliotis:\n"
	                  	"CPXsetusercutcallbackfunc : %d\n", status);
		return 1;
	}


	status = CPXpresolve(cplexenv, lp, CPX_ALG_NONE);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
	                    "function: cpx_solve_miliotis:\n"
						"CPXpresolve : %d\n", status);
		return status;
	}

	status = CPXsetdblparam(cplexenv, CPX_PARAM_TILIM, 3600);
	//status = CPXsetdblparam(cplexenv, CPX_PARAM_TRELIM, 500);

	clock_t t1 = clock(), t2;
	status = CPXmipopt(cplexenv, lp);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
						"function: cpx_solve_miliotis:\n"
						"CPXmipopt : %d\n", status);
		return 1;
	}

	t2 = clock();
	long nodes_total = CPXgetnodecnt (cplexenv, lp);
	printf("nodes processed : %d\n", nodes_total);
	if ((double)(t2 - t1) / CLOCKS_PER_SEC >= 3600) {
		printf("time limit reached\n");
		return 0;
	}


	status = CPXfreepresolve(cplexenv, lp);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
	                    "function: cpx_solve_miliotis:\n"
						"CPXfreepresolve : %d\n", status);
		return 1;
	}

	status = CPXgetstat(cplexenv, lp);
	*solstat = status;
	if (status == 103) return 0;


	if (CPXgetnumcols(cplexenv, lp) != numcols) return 1;


	status = CPXgetx(cplexenv, lp, x, 0, numcols-1);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
						"function: cpx_solve_miliotis:\n"
	            		"CPXgetx : %d\n", status);
		return 1;
	}

	return 0;
}


int CPXPUBLIC cpx_callback_miliotis(CPXENVptr cplexenv,
                                    void     *cbdata,
                                    int       wherefrom,
                                    void     *cbhandle,
                                    int      *useraction_p) {


	printf("# hello callback!\n");
	//getchar();

	int status = 0;
	*useraction_p = CPX_CALLBACK_DEFAULT;

	environment* env = (environment*)cbhandle;
	int n 			 = env->main_graph.vrtx_num;
	int numcols		 = n * (n - 1) / 2;
	double *x 		 = malloc(numcols * sizeof(double));


	int prenumcols;
	CPXLPptr prelp;

	status = CPXgetcallbacklp(cplexenv, cbdata, wherefrom, &prelp);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
						"function: cpx_callback_miliotis:\n"
						"CPXgetcallbacklp : %d\n", status);
		return 1;
	}

	double currbest, timestamp;
	long nodes_done;
	status = CPXgetcallbackinfo(cplexenv, cbdata, wherefrom, CPX_CALLBACK_INFO_BEST_INTEGER, &currbest);
	status = CPXgetcallbackinfo(cplexenv, cbdata, wherefrom, CPX_CALLBACK_INFO_ENDTIME, &timestamp);
	status = CPXgetcallbackinfo(cplexenv, cbdata, wherefrom, CPX_CALLBACK_INFO_NODE_COUNT_LONG, &nodes_done);
	printf("node %d ; best current solution: %f\n", nodes_done, currbest);

	prenumcols = CPXgetnumcols(cplexenv, prelp);

	double *prex = malloc(prenumcols*sizeof(double));
	status = CPXgetcallbacknodex(cplexenv, cbdata, wherefrom, prex, 0, prenumcols-1);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
						"function: cpx_callback_miliotis:\n"
						"CPXgetcallbacknodex : %d\n", status);
		return 1;
	}

	status = CPXuncrushx(cplexenv, env->mylp, x, prex);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
						"function: cpx_callback_miliotis:\n"
						"CPXgetuncrushx : %d\n", status);
		return 1;
	}


	/*
	status = cpx_mark_components(env, x, numcols, vrtx_comp, n, &numcomp);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
						"function: cpx_callback_miliotis:\n"
						"cpx_mark_components : %d\n", status);
		return 1;
	}
	*/

	/* mark connected components */
	set vertex_set;
	set_init(&vertex_set);
	set_setup(&vertex_set, n);

	int  i, k, my_edge, my_comp, s1, s2;
	int numcomp 		= 0;
	int *components 	= (int*)malloc(n * sizeof(int));

	set_empty(&vertex_set);
	for (my_edge = 0; my_edge < numcols; my_edge++) {
		if (x[my_edge] > 0.9)  {
			set_find(&vertex_set, get_v1(my_edge), &s1);
			set_find(&vertex_set, get_v2(my_edge), &s2);
			if (s1 != s2) {
				set_union(&vertex_set, s1, s2);
			}
		}
	}

	for (k = 0; k < n; k++) components[k] = 0;
	for (k = 0; k < n; k++) components[vertex_set.list[k]] = 1;
	numcomp = 0;
	for (k = 0; k < n; k++) {
		if (components[k] == 1) numcomp++;
	}


	/* add subtour elimination constraints */
	cpx_cstr sec;
	cpx_cstr_init(&sec);
	cpx_cstr_setup(&sec, numcols);

	cpx_cstr presec;
	cpx_cstr_init(&presec);
	cpx_cstr_setup(&presec, prenumcols);

	int prenzcnt;
	double preoffset;

	if (numcomp > 1 && wherefrom == CPX_CALLBACK_MIP_CUT_FEAS) {

		//int count = (numcomp < 3) ? numcomp : 3;

		printf("# SECs added: %d\n", numcomp);
		//printf("# SECs added: %d\n", count);
		//printf("%d\n", rand());

		for (i = 0; i < n; i++) {

			if (components[i] == 1) {

				my_comp = i;


				/* ad the sec (subtour elimination constraint)
				 * associated to this component
				 */

				cpx_cstr_empty(&sec);
				status = cpx_cstr_sec(env, &vertex_set, my_comp, &sec);
				if (status) {
					fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_iterative.c:\n"
			                        "function: cpx_solve_iterative:\n"
			                        "cpx_cstr_sec : %d\n", status);
					return 1;
				}


				CPXcrushform(cplexenv, env->mylp, sec.nzcnt, sec.rmatind, sec.rmatval, &prenzcnt, &preoffset, presec.rmatind, presec.rmatval);

				presec.nzcnt = prenzcnt;
				presec.rhs = sec.rhs -  preoffset;
				//presec.rowname = sec.rowname;
				presec.rmatbeg = sec.rmatbeg;
				presec.sense = sec.sense;

				status = CPXcutcallbackadd(cplexenv, cbdata, wherefrom, presec.nzcnt, presec.rhs, presec.sense, presec.rmatind, presec.rmatval, CPX_USECUT_PURGE);
				if (status) {
					fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
									"function: cpx_callback_miliotis:\n"
									"CPXcutcallbackadd : %d\n", status);
					return 1;
				}

				/*
				status = cpx_cstr_add(cplexenv, lp, &sec);
			    if (status) {
			    	fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_iterative.c:\n"
			                        "function: cpx_solve_iterative:\n"
			                        "cpx_cstr_add : %d\n", status);
			        return 1;
			    }
			    */

			} /* end if components[i] == 1 */

		} /* end for i */

	} /* end if numcomp > 1 */




	/*
	if (numcomp > 1 && wherefrom == CPX_CALLBACK_MIP_CUT_FEAS) {

		//int count = (numcomp < 3) ? numcomp : 3;

		printf("# SECs added: %d\n", numcomp);
		//printf("# SECs added: %d\n", count);
		//printf("%d\n", rand());

		for (my_comp = 0; my_comp < numcomp; my_comp++) {
		//for (my_comp = 0; my_comp < count; my_comp++) {


			status = cpx_cstr_sec(env, vrtx_comp, n, my_comp, &sec);
			if (status) {
				fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
								"function: cpx_callback_miliotis:\n"
								"cpx_constraint_generate_sec : %d\n", status);
				return 1;
			}


			CPXcrushform(env, env->mylp, sec.nzcnt, sec.rmatind, sec.rmatval, &prenzcnt, &preoffset, presec.rmatind, presec.rmatval);

			presec.nzcnt = prenzcnt;
			presec.rhs = sec.rhs -  preoffset;
			//presec.rowname = sec.rowname;
			presec.rmatbeg = sec.rmatbeg;
			presec.sense = sec.sense;

			status = CPXcutcallbackadd(env, cbdata, wherefrom, presec.nzcnt, presec.rhs, presec.sense, presec.rmatind, presec.rmatval, CPX_USECUT_PURGE);
			if (status) {
				fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
								"function: cpx_callback_miliotis:\n"
								"CPXcutcallbackadd : %d\n", status);
				return 1;
			}

		}
	}
	*/

	cpx_cstr_delete(&sec);
	cpx_cstr_delete(&presec);
	free(x);
	free(prex);

	if (numcomp > 1) {
		*useraction_p = CPX_CALLBACK_SET;
	}

	return 0;

}

int CPXPUBLIC cpx_callback_maxflow(CPXENVptr  cplexenv,
                                    void     *cbdata,
                                    int       wherefrom,
                                    void     *cbhandle,
                                    int      *useraction_p) {

	int status = 0, i, j, k, idx;
	*useraction_p = CPX_CALLBACK_DEFAULT;

	if (rand() % 100 > 5) {
		*useraction_p = CPX_CALLBACK_SET;
		return 0;
	}

	//printf("# maxflow callback\n");

	//cpx_env* ce = (cpx_env*)cbhandle;
	//int n = ce->G.n;
	environment* env 	 = (environment*)cbhandle;
	int 		 n		 = env->main_graph.vrtx_num;
	int 		 numcols = n * (n - 1) / 2;

	double *x = calloc(numcols, sizeof(double));

	int prenumcols;
	CPXLPptr prelp;

	status = CPXgetcallbacklp(cplexenv, cbdata, wherefrom, &prelp);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
						"function: cpx_callback_maxflow:\n"
                    	"CPXgetcallbacklp : %d\n", status);
		return 1;
	}

	prenumcols = CPXgetnumcols(cplexenv, prelp);

	double *prex = malloc(prenumcols*sizeof(double));
	status = CPXgetcallbacknodex(cplexenv, cbdata, wherefrom, prex, 0, prenumcols-1);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
						"function: cpx_callback_maxflow:\n"
                  	  	"CPXgetcallbacknodex : %d\n", status);
		return 1;
	}

	status = CPXuncrushx(cplexenv, env->mylp, x, prex);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
						"function: cpx_callback_maxflow:\n"
						"CPXgetuncrushx : %d\n", status);
		return 1;
	}

	int 	*cut 	  = NULL;
	int 	 cutcount = -1;
	double	 minval;
	int  	ncomp;
	int 	*compscount,
			*comps;

	status = cpx_cstr_maxflow(env, x, &comps, &compscount, &ncomp, &cut, &cutcount, &minval);

	//printf("%d\n", cutcount);
	//getchar();

	if (ncomp == 1 && cutcount > 0) {
		/*printf("in: if cutcount > 0\n");
		for (i = 0; i < cutcount; i++) {
			printf("%d ", cut[i]);
		}
		printf("\n");*/


		int nflows = cutcount * (n - cutcount);
		flow *flist = malloc(nflows * sizeof(flow));
		/*for (i = 0 ; i < nflows ; i++) {
      	  flist[i] = malloc(sizeof(flist));
    	}*/


		int    *matind = malloc(cutcount*(n-cutcount)*sizeof(int));
		double *matval = malloc(cutcount*(n-cutcount)*sizeof(double));
		int    *mark   = calloc(n, sizeof(int));

		for (i = 0 ; i < cutcount ; i++) mark[cut[i]] = 1;

		k = 0;
		for (i = 0 ; i < cutcount ; i++) {
			for (j = 0 ; j < n ; j++) {
				if (mark[j] == 0) {
					idx = get_idx(cut[i], j);
					//idx_from_vrtx(&ce->T, cut[i], j, &idx);
					flist[k].index    = k;
					flist[k].edge     = idx;
					flist[k].capacity = x[idx];
					// printf("%d | %d %d | %d %f\n", k, cut[i], j, idx, x[idx]);
					k++;
				}
			}
		}

		/*printf ("%d =?= %d\n", k, nflows);
    	getchar();*/

		qsort(flist, nflows, sizeof(flow), compare_flows);

		for (i = 0 ; i < nflows ; i++) {
			// printf("%d %f\n", flist[i].edge, fabs(flist[i].capacity));
			matind[i] = flist[i].edge;
			matval[i] = 1.0;//fabs(flist[i].capacity);
		}

		double *pval = calloc(numcols, sizeof(double));
		int    *pind = calloc(numcols, sizeof(int));
		double  offset_p;
		int     plen_p;

		status = CPXcrushform(cplexenv, env->mylp, nflows, matind, matval, &plen_p, &offset_p, pind, pval);
		if (status) {
			fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
							"function: cpx_callback_maxflow:\n"
                      	  	"CPXcrushform : %d\n", status);
			return 1;
		}

		char   sen = 'G';
		double rhs = 2.0;
		status = CPXcutcallbackadd(cplexenv, cbdata, wherefrom, nflows, rhs, sen, pind, pval, CPX_USECUT_PURGE);
		if (status) {
			fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
							"function: cpx_callback_maxflow:\n"
                      	  	"CPXcutcallbackadd : %d\n", status);
			return 1;
		}

		free(mark);
		free(matind);
		free(matval);
		free(flist);
		free(pval);
		free(pind);
		//if (*cut != NULL) free(cut);
		if (cut != NULL) free(cut);
		//free(cut);

	} else if (ncomp > 1) {

		// add SEC constraints to break subcycles
		// first element of first connected component returned is always 0
		// or, at least, I hope so
		//int current_node = compscount[0], curr_cc;

		int    *matind = malloc(compscount[0]*(n - compscount[0])*sizeof(int));
		double *matval = malloc(compscount[0]*(n - compscount[0])*sizeof(double));
		double *pval = calloc(numcols, sizeof(double));
		int    *pind = calloc(numcols, sizeof(int));
		double  offset_p;
		int     plen_p;

		k = 0;
		for (i = 0 ; i < compscount[0] ; i++) {
			for (j = compscount[0] ; j < n ; j++) {
				idx = get_idx(comps[i], comps[j]);
				//idx_from_vrtx(&ce->T, comps[i], comps[j], &idx);
				//printf("%d %d %d\n", comps[i], comps[j], idx);
				matind[k] = idx;
				matval[k] = 1.0;
				k++;
			}
		}

		qsort(matind, k, sizeof(int), compare_ints);

		//getchar();
		status = CPXcrushform(cplexenv, env->mylp, k, matind, matval, &plen_p, &offset_p, pind, pval);
		if (status) {
			fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
							"function: cpx_callback_maxflow:\n"
							"CPXcrushform : %d\n", status);
			return 1;
		}

		char   sen = 'G';
		double rhs = 2.0;
		status = CPXcutcallbackadd(cplexenv, cbdata, wherefrom, k, rhs, sen, pind, pval, CPX_USECUT_PURGE);
		if (status) {
			fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
							"function: cpx_callback_maxflow:\n"
                      	  	"CPXcutcallbackadd : %d\n", status);
			return 1;
		}

		free(matind);
		free(matval);
		free(pval);
		free(pind);

		/*for (current_node = 0 ; current_node < n ; current_node++)
      	  printf(" %d", comps[current_node]);
    	printf("\n");*/
		//getchar();
	}


	free(x);
	free(prex);
	free(comps);
	free(compscount);

	*useraction_p = CPX_CALLBACK_SET;
	return 0;

}

int compare_flows(const void *f1, const void *f2) {

	flow *ff1 = (flow *)f1;
	flow *ff2 = (flow *)f2;
	//printf("%d %d %f | %d %d %f\n", ff1.index, ff1.edge, ff1.capacity, ff2.index, ff2.edge, ff2.capacity);
	return (ff1->edge - ff2->edge);

}
