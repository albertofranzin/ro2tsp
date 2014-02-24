#ifndef CPX_SOLVER_H_
#define CPX_SOLVER_H_


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

#include "../../solvers/cpx/cpx_solve_iterative.h"
#include "../../solvers/cpx/cpx_solve_miliotis.h"
#include "../../solvers/cpx/cpx_solve_rins.h"
#include "../../solvers/cpx/cpx_solve_hardfixing.h"
#include "../../solvers/cpx/cpx_solve_proximity.h"
#include "../../solvers/cpx/cpx_solve_local_branching.h"


/**
 * cpx_solver
 * cplex-based tsp solver: just a "proxy" method
 *
 * @param ce	cpx environment
 * @param cs	cpx environment
 * @param pars	user parameters
 */
int cpx_solver(environment *env, statistics *stats, parameters *pars);


#endif /* CPX_SOLVER_H_ */
