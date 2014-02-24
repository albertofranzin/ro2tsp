#include "../../solvers/cpx/cpx_setup_problem.h"

int cpx_setup_problem(CPXENVptr   	 cplexenv,
                      CPXLPptr    	 lp,
                      environment	*env,
                      parameters	*pars) {

	int status  	= 0;
	int n       	= env->main_graph.vrtx_num;
	int numedges	= n * (n - 1) / 2;
	int numcols 	= numedges;
	int my_edge, i, j, k;


#ifdef DEBUG
	if (pars->verbosity >= USEFUL) {
		printf("solvers/cpx/cpx_setup_problem.c :: "
			   "cpx_setup_problem: n = %d, numcols = %d\n", n, numcols);
	}
#endif

	/* set problem as Mixed-Integer LP */
	CPXchgprobtype(cplexenv, lp, CPXPROB_MILP);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c:\n"
						"function: cpx_setup_problem:\n"
						"CPXchgprobtype : %d\n", status);
		return status;
	}

	/* set objective function */
	double *lb;      // lower bound on variables
	double *ub;      // upper bound on variables
	char   *xtype;   // type of variables
	double *obj;     // objected value computed for the variables
	char  **colname; // name of columns (really need this?)

	lb      = (double *)calloc(numcols, sizeof(double));
	ub      = (double *)calloc(numcols, sizeof(double));
	xtype   = (char *)  calloc(numcols, sizeof(char));
	obj     = (double *)calloc(numcols, sizeof(double));
	colname = (char **) calloc(numcols, sizeof(char *));

	for (i = 0; i < numcols; i++) {
		colname[i] = (char *)calloc(100, sizeof(char));
	}

	// set variables: binary -> {0,1} -> [0,1] in the relaxated problem
	// set bounds and retrieve variable corresponding to the vertex, according to
	// its position in the table
	for (my_edge = 0; my_edge < numedges; my_edge++) {
		lb[my_edge]    	= 0.0;
		ub[my_edge]    	= 1.0;
		xtype[my_edge] 	= 'B';
		obj[my_edge] 	= env->main_graph.edge_cost[my_edge];
		sprintf(colname[my_edge], "x_%d_%d",
		  get_v1(my_edge), get_v2(my_edge));
	}

	status = CPXnewcols(cplexenv, lp, numcols, obj, lb, ub, xtype, colname);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c:\n"
						"function: cpx_setup_problem:\n"
                    	"PXnewcols : %d\n", status);
		return status;
	}

	/* set problem as a minimum problem */
	status = CPXchgobjsen(cplexenv, lp, CPX_MIN);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c:\n"
						"function: cpx_setup_problem:\n"
                    	"CPXchgobjsen : %d\n", status);
		return status;
	}

	/* set degree constraints */
	cpx_cstr c;
	cpx_cstr_init(&c);
	cpx_cstr_setup(&c, n-1);

	c.nzcnt   = n-1;
	c.rmatbeg = 0;
	c.rhs     = 2.0;
	c.sense   = 'E';

	for (i = 0; i < n; i++) {

		sprintf(c.rowname, "deg_cstr_%d", i);

		k = 0;
		for (j = 0; j < n; j++) {
			if (j != i) {
				c.rmatind[k] = get_idx(i, j);
				c.rmatval[k] = 1.0;
				k++;
			}
		}

	    status = cpx_cstr_add(cplexenv, lp, &c);
	    if (status) {
	    	fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c:\n"
	                    	"function: cpx_setup_problem:\n"
	                      	"cpx_cstr_add : %d\n", status);
	    	return status;
	    }

	}


	free(ub);
	free(lb);
	free(xtype);
	free(obj);
	free(colname);

	return 0;
}
