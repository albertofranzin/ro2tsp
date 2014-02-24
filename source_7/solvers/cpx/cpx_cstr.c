#include "../../solvers/cpx/cpx_cstr.h"


int cpx_cstr_init(cpx_cstr *c) {

	c->max_nzcnt  = -1;
	c->rmatind    = NULL;
	c->rmatval    = NULL;
	c->rowname    = NULL;
	c->nzcnt      = -1;
	c->rmatbeg    = -1;
	c->rhs        = -1.0;
	c->sense      = -1;

	return 0;
}


int cpx_cstr_setup(cpx_cstr *c, int max_nzcnt) {

#ifdef DEBUG
	if (max_nzcnt < 1) {
		printf("error: cpx_cstr_init\n");
		exit(1);
	}
#endif

	if (c->rmatind != NULL) {
		free(c->rmatind);
	}
	if (c->rmatval != NULL) {
		free(c->rmatval);
	}
	if (c->rowname != NULL) {
		free(c->rowname);
	}

	c->max_nzcnt  = max_nzcnt;
	c->rmatind    = (int*)calloc(max_nzcnt, sizeof(int));
	c->rmatval    = (double*)calloc(max_nzcnt, sizeof(double));
	c->rowname    = (char*)calloc(100, sizeof(char));
	c->nzcnt      = 0;
	c->rmatbeg    = 0;
	c->rhs        = 0.0;
	c->sense      = 0;

	return 0;
}


int cpx_cstr_empty(cpx_cstr *c) {

	int i;

	for (i = 0; i < c->max_nzcnt; i++) {
		c->rmatind[i] = 0;
		c->rmatval[i] = 0.0;
	}
	for (i = 0; i < 100; i++) {
		c->rowname[i] = 0;
	}

	c->nzcnt      = 0;
	c->rmatbeg    = 0;
	c->rhs        = 0.0;
	c->sense      = 0;

	return 0;
}


int cpx_cstr_delete(cpx_cstr *c) {

	free(c->rmatind);
	free(c->rmatval);
	free(c->rowname);

	c->rmatind   = NULL;
	c->rmatval   = NULL;
	c->rowname   = NULL;
	c->nzcnt     = -1;
	c->rmatbeg   = -1;
	c->rhs       = -1.0;
	c->sense     = -1;
	c->max_nzcnt = -1;

	return 0;
}


int cpx_cstr_add(CPXENVptr  cplexenv,
                 CPXLPptr	lp,
                 cpx_cstr  *c) {

	int status = 0;

	double vrhs[1];
	char   vsense[1];
	int    vrmatbeg[1];
	char  *vrowname[1];

	vrhs[0]     = c->rhs;
	vsense[0]   = c->sense;
	vrmatbeg[0] = c->rmatbeg;
	vrowname[0] = c->rowname;

	status = CPXaddrows(cplexenv, lp, 0, 1, c->nzcnt, vrhs, vsense,
                 vrmatbeg, c->rmatind, c->rmatval, NULL, vrowname);
	if (status) {
		fprintf(stderr, "Fatal error in solvers/cpx/cpx_cstr.c:\n"
						"function: cpx_cstr_add:\n"
						"CPXaddrows : %d\n", status);
		return status;
	}

  return status;
}


int cpx_cstr_sec(environment	*env,
                 set            *vertex_set,
                 int             my_comp,
                 cpx_cstr	    *c) {


	int n       = env->main_graph.vrtx_num;
	int numcols = n * (n - 1) / 2;
	int i, my_edge, numvrtxs, numedges;


	if (c->max_nzcnt < n * (n - 1) / 2) return 1;


	sprintf(c->rowname, "sec_cstr");


	numedges = 0;
	for (my_edge = 0; my_edge < numcols; my_edge++) {

		if (vertex_set->list[get_v1(my_edge)] == my_comp &&
			vertex_set->list[get_v2(my_edge)] == my_comp) {

			c->rmatind[numedges] = my_edge;
			c->rmatval[numedges] = 1.0;
			numedges++;
		}
	}


	numvrtxs = 0;
	for (i = 0; i < n; i++) {
		if (vertex_set->list[i] == my_comp) {
			numvrtxs++;
		}
	}

	c->nzcnt   = numedges;
	c->rmatbeg = 0;
	c->rhs     = numvrtxs - 1;
	c->sense   = 'L';

	return 0;
}

int cpx_cstr_proximity_cutoff(environment   *env,
                              double        *x_feas,
                              int            x_feas_size,
                              double         theta,
                              cpx_cstr 		*c) {


	int    n       = env->main_graph.vrtx_num;
	int    numcols = n * (n - 1) / 2;
	int    idx;
	double z_feas;

	if (x_feas_size != numcols || c->max_nzcnt < numcols) return 1;
	/**
	 * could be replaced with
	 * assert(x_feas_size == numcols && c->max_nzcnt >= numcols);
	 */

	sprintf(c->rowname, "cutoff_cstr");

	z_feas = 0.0;
	for (idx = 0; idx < x_feas_size; idx++) {
		if (x_feas[idx] > 0.9) {
			z_feas += env->main_graph.edge_cost[idx];
			//vrtx_from_idx(&ce->T, &i, &j, idx);
			//z_feas += graph_get_edge_cost(&ce->G, i, j);
		}
	}

	for (idx = 0; idx < numcols; idx++) {
		//vrtx_from_idx(&ce->T, &i, &j, idx);
		(c->rmatind)[idx] = idx;
		(c->rmatval)[idx] = env->main_graph.edge_cost[idx];
		//(c->rmatval)[idx] = graph_get_edge_cost(&ce->G, i, j);

	}

	c->nzcnt   = numcols;
	c->rmatbeg = 0;
	c->rhs     = z_feas - theta;
	c->sense   = 'L';

	return 0;

}

int cpx_cstr_maxflow(environment 	*env,
                     double  		*x,
                     int    	   **ret_comps,
                     int    	   **ret_compscount,
                     int     		*ret_ncomps,
                     int    	   **cut_set,
                     int     		*cscount,
                     double			*minval) {

	int status = 0;
	int i, j, idx;
	int n       = env->main_graph.vrtx_num;
    int numcols = n * (n-1) / 2;

    int  ncomp;
    int *compscount,
    	*comps,
    	*elist;

    elist = malloc(2 * numcols * sizeof(int));
    for (idx = 0 ; idx < numcols ; idx++) {
    	i = get_v1(idx);
    	j = get_v2(idx);
    	//vrtx_from_idx(&ce->T, &i, &j, idx);
    	elist[2*idx]     = i;
    	elist[2*idx + 1] = j;
    }

    int retval = CCcut_connect_components(n, numcols, elist, x, &ncomp,
    											  &compscount, &comps);
    assert(!retval);

    printf("retval %d\n", retval);

    /*int current_node = 0, curr_cc;
    for (curr_cc = 0; curr_cc < ncomp; curr_cc++) {
      printf("CC #%d:", curr_cc);
      for (i = 0; i < compscount[curr_cc]; i++) {
        printf(" %d", comps[current_node]);
        current_node++;
      }
      printf("\n");
    }
    getchar();*/

    if(ncomp == 1) {
    	double cutval;
    	int   *cut,
    	  cutcount;

    	int retval = CCcut_mincut(n, numcols, elist, x, &cutval, &cut,
    													   &cutcount);

    	printf("Ret: %d, cutcount = %d\n", retval, cutcount);
    	printf("%lf\n", cutval);

        /*for (i = 0; i < cutcount; i++) {
          printf("%d ", cut[i]);
        }
        printf("\n");*/

    	if (cutval < 2.0) {
    		//printf("lakhskjsnd\n");
    		*cut_set = cut;
    		//getchar();
    		*cscount = cutcount;
    		*minval  = cutval;
    	}

    	//getchar();
    }

    *ret_comps      = comps;
    *ret_compscount = compscount;
    *ret_ncomps     = ncomp;


    return status;
}
