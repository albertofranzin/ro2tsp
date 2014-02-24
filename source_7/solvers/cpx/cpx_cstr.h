#ifndef CPX_CSTR_H_
#define CPX_CSTR_H_

#include <stdlib.h>
#include <stdio.h>
#include <ilcplex/cplex.h>
#include "../../base/global.h"
#include "../../base/utils.h"

#include "../../data/environment.h"
#include "../../data/set.h"


#include "../concorde/concorde.h"

/**
 * cpx_constraint
 * infos needed for defining a constraint in CPLEX
 *
 * max_nzcnt : maximum number of non-zero elements (constrained variables)
 * rmatind   : list of indices of the positions in rmatval containing
 *             the non-zero elements
 * rmatval   : list of coefficients of the variables
 * nzcnt     : number of non-zero elements
 * rmatbeg   : index of the first non-zero coefficient
 * rhs       : right-hand-side term of the (in)equality
 * sense     : less than, equal, greater than
 * rowname   : name of the constraint
 */
typedef struct _cpx_cstr {

	int     max_nzcnt;
	int    *rmatind;
	double *rmatval;
	int     nzcnt;
	int     rmatbeg;
	double  rhs;
	char    sense;
	char   *rowname;

} cpx_cstr;



int cpx_cstr_init(cpx_cstr *c);

int cpx_cstr_setup(cpx_cstr *c, int max_nzcnt);

int cpx_cstr_empty(cpx_cstr *c);


/**
 * cpx_constraint_delete
 * delete constraint
 *
 * @param c         constraint
 * @param max_nzcnt maximum number of constrained variables
 */
int cpx_cstr_delete(cpx_cstr* c);


/**
 * cpx_constraint_add
 * add the constraint as a new row in CPLEX problem
 *
 * @param env  CPLEX environment
 * @param lp   CPLEX problem
 * @param c    constraint
 * @return     status of the function
 */
int cpx_cstr_add(CPXENVptr cplexenv,
                 CPXLPptr  lp,
                 cpx_cstr  *c);


/**
 * cpx_constraint_generate_sec
 * generate the sec associated to the component with label my_comp
 *
 * @param ce              cpx environment
 * @param vrtx_comp       vertex component-labels
 * @param vrtx_comp_size  vrtx_comp length
 * @param my_comp         label of the component
 * @param c               constraint
 * @return                status of the function
 */

int cpx_cstr_sec(environment	*env,
                 set            *vertex_set,
                 int             my_comp,
                 cpx_cstr	    *c);


/**
 * cpx_constraint_generate_proximity_cutoff
 * generate proximity-search cutoff-constraint
 *
 * @param ce          cpx environment
 * @param x_feas      feasible solution
 * @param x_feas_size x_feas length
 * @param theta       proximity-search parameter
 * @param c           constraint
 * @return            status of the function
 */
int cpx_cstr_proximity_cutoff(environment	*env,
                              double      	*x_feas,
                              int         	 x_feas_size,
                              double      	 theta,
                              cpx_cstr 		*c);


/**
 * create max-flow constraints on the fractional solution
 * @param  ce         cpx environment
 * @param  x          the fractional solution
 * @param  cut_set    list of nodes in separated set
 * @return            status of the function
 */
int cpx_cstr_maxflow(environment	*ce,
                     double			*x,
                     int    	   **ret_comps,
                     int    	   **ret_compscount,
                     int     		*ret_ncomps,
                     int    	   **cut_set,
                     int     		*cscount,
                     double  		*minval);

#endif /* CPX_CSTR_H_ */
