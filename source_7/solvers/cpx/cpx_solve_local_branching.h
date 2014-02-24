#ifndef CPX_SOLVE_LOCAL_BRANCHING_H_
#define CPX_SOLVE_LOCAL_BRANCHING_H_


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
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
 * solve the problem using local branching
 * @param env           CPLEX environment
 * @param lp            CPLEX problem
 * @param ce            cpx environment
 * @param cs            cpx statistics
 * @param x             computed solution
 * @param x_size        x length
 * @param solstat       CPLEX status of the solution
 * @return              status of the function
 */
int cpx_solve_local_branching(CPXENVptr   	 cplexenv,
                              CPXLPptr    	 lp,
                              environment	*env,
                              parameters 	*pars,
                              statistics  	*stats,
                              double     	*ret_x,
                              int         	 x_size,
                              int        	*ret_solstat);


#endif /* CPX_SOLVE_LOCAL_BRANCHING_H_ */
