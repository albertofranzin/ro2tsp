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

