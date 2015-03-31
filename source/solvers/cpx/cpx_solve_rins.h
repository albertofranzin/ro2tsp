#ifndef CPX_SOLVE_RINS_H_
#define CPX_SOLVE_RINS_H_

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
#include "../../solvers/cpx/cpx_solve_miliotis.h"


/**
 * solve the problem casting RINS+polishing over a solution computed using an heuristic
 * @param env           CPLEX environment
 * @param lp            CPLEX problem
 * @param ce            cpx environment
 * @param cs            cpx statistics
 * @param zeros         contains variables to be set to zero
 * @param ones          contains variables to be set to one
 * @param x             computed solution
 * @param x_size        x length
 * @param solstat       CPLEX status of the solution
 * @return              status of the function
 */
int cpx_solve_rins(CPXENVptr   	 cplexenv,
                   CPXLPptr    	 lp,
                   environment	*env,
                   parameters  	*pars,
                   statistics 	*stats,
                   int       	*zeros,
                   int        	*ones,
                   double     	*ret_x,
                   int         	 x_size,
                   int        *ret_solstat);

#endif /* CPX_SOLVE_RINS_H_ */
