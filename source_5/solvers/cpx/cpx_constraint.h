#ifndef CPX_CONSTRAINT_H_
#define CPX_CONSTRAINT_H_


#include <stdlib.h>
#include <stdio.h>
#include <ilcplex/cplex.h>

#include "cpx_env.h"
#include "cpx_components.h"

#include "../concorde/concorde.h"

// ---------------------------------------------------------- //
//                                                            //
//                       data structures                      //
//                                                            //
// ---------------------------------------------------------- //

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
typedef struct _cpx_constraint {
  int     max_nzcnt;
  int    *rmatind;
  double *rmatval;
  int     nzcnt;
  int     rmatbeg;
  double  rhs;
  char    sense;
  char   *rowname;
} cpx_constraint;


// ---------------------------------------------------------- //
//                                                            //
//                          functions                         //
//                                                            //
// ---------------------------------------------------------- //


/**
 * cpx_constraint_init
 * initialize constraint
 * 
 * @param c         constraint
 * @param max_nzcnt maximum number of constrained variables
 */
void cpx_constraint_init(cpx_constraint *c,
                       int             max_nzcnt);


/**
 * cpx_constraint_delete
 * delete constraint
 * 
 * @param c         constraint
 * @param max_nzcnt maximum number of constrained variables
 */
void cpx_constraint_delete(cpx_constraint* c);


/**
 * cpx_constraint_add
 * add the constraint as a new row in CPLEX problem
 * 
 * @param env  CPLEX environment
 * @param lp   CPLEX problem
 * @param c    constraint
 * @return     status of the function
 */
int cpx_constraint_add(CPXENVptr       env,
                       CPXLPptr        lp,
                       cpx_constraint *c);



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
int cpx_constraint_generate_sec(cpx_env        *ce,
                                int            *vrtx_comp,
                                int             vrtx_comp_size,
                                int             my_comp,
                                cpx_constraint *c);


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
int cpx_constraint_generate_proximity_cutoff(cpx_env        *ce,
                                             double         *x_feas,
                                             int             x_feas_size,
                                             double          theta,
                                             cpx_constraint *c);


#endif

/**
 * create max-flow constraints on the fractional solution
 * @param  ce         cpx environment
 * @param  x          the fractional solution
 * @param  cut_set    list of nodes in separated set
 * @return            status of the function
 */
int cpx_maxflow_constraints(cpx_env *ce,
                            double  *x,
                            int    **cut_set,
                            int     *cscount,
                            double  *minval);
