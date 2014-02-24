#ifndef CPX_SOLVE_PROXIMITY_H_
#define CPX_SOLVE_PROXIMITY_H_


#include <stdlib.h>
#include <stdio.h>
#include <ilcplex/cplex.h>
#include "../../base/global.h"
#include "../../base/utils.h"

#include "../../data/environment.h"
#include "../../data/statistics.h"
#include "../../data/parameters.h"

#include "../../solvers/cpx/cpx_cstr.h"
#include "../../solvers/cpx/cpx_create_problem.h"
#include "../../solvers/cpx/cpx_setup_problem.h"
#include "../../solvers/cpx/cpx_set_parameters.h"
#include "../../solvers/cpx/cpx_preprocessing.h"

#include "../../solvers/cpx/cpx_solve_iterative.h"


/**
 * solve the problem using proximity-search approach
 * @param env           CPLEX environment
 * @param lp            CPLEX problem
 * @param ce            cpx environment
 * @param cs            cpx statistics
 * @param x             computed solution
 * @param x_size        x length
 * @param solstat       CPLEX status of the solution
 * @return              status of the function
 */
int cpx_solve_proximity(CPXENVptr    cplexenv,
                       CPXLPptr      lp,
                       environment  *env,
                       parameters	*pars,
                       statistics   *stats,
                       double*       x,
                       int           x_size,
                       int*          solstat);

/**
 * recenter the objective function with respect to the
 * new feasible solution
 * @param env           CPLEX environment
 * @param lp            CPLEX problem
 * @param x_feas        feasible solution
 * @param x_feas_sixe   size of x_feas
 * @return              status of the function
 */
int cpx_recenter_obj(CPXENVptr   cplexenv,
                     CPXLPptr    lp,
                     double     *x_feas,
                     int         x_feas_size);

/**
 * update the cutoff constraint
 * @param env           CPLEX environment
 * @param lp            CPLEX problem
 * @param x_feas        feasible solution
 * @param x_feas_sixe   size of x_feas
 * @param theta         proximity-search parameter
 * @param cutoff_idx    index of the row corresponding
 * to the cutoff constraint
 * @return              status of the function
 */
int cpx_update_cutoff(CPXENVptr		 cplexenv,
                      CPXLPptr  	 lp,
                      environment	*env,
                      double    	*x_feas,
                      int      		 x_feas_size,
                      double    	 theta,
                      int       	 cutoff_idx);



#endif /* CPX_SOLVE_PROXIMITY_H_ */
