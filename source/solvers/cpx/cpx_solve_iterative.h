#ifndef CPX_SOLVE_ITERATIVE_H_
#define CPX_SOLVE_ITERATIVE_H_


#include <stdlib.h>
#include <stdio.h>
#include <ilcplex/cplex.h>
#include "../../base/global.h"
#include "../../base/utils.h"

#include "../../data/environment.h"
#include "../../data/statistics.h"
#include "../../data/parameters.h"
#include "../../data/set.h"

#include "../../solvers/cpx/cpx_cstr.h"
#include "../../solvers/cpx/cpx_create_problem.h"
#include "../../solvers/cpx/cpx_setup_problem.h"
#include "../../solvers/cpx/cpx_set_parameters.h"
#include "../../solvers/cpx/cpx_preprocessing.h"

/**
 * solve the problem using the iterative method
 * @param env       CPLEX environment
 * @param lp        CPLEX problem
 * @param ce        cpx environment
 * @param cs        cpx statistics
 * @param x         computed solution
 * @param x_size    x length
 * @param solstat   CPLEX status of the solution
 * @return          status of the function
 */
int cpx_solve_iterative(CPXENVptr    cplexenv,
                        CPXLPptr     lp,
                        environment	*env,
                        parameters  *pars,
                        statistics  *stats,
                        double      *x,
                        int          x_size,
                        int         *solstat);

#endif /* CPX_SOLVE_ITERATIVE_H_ */
