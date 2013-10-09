#ifndef CPX_SOLVER_H_
#define CPX_SOLVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <ilcplex/cplex.h>

#include "../../base/constants.h"
#include "../../base/utils.h"

#include "../../data/graph.h"
#include "../../data/egraph.h"

#include "cpx_table.h"
#include "cpx_env.h"
#include "cpx_stats.h"
#include "cpx_setup_problem.h"
#include "cpx_solve_miliotis.h"
#include "cpx_solve_iterative.h"
#include "cpx_solve_proximity.h"
#include "cpx_preprocessing.h"

/**
 * cpx_solver
 * cplex-based tsp solver: just a "proxy" method
 * 
 * @param ce	cpx environment
 * @param cs	cpx environment
 * @param pars	user parameters
 */
void cpx_solver(cpx_env    *ce,
	            cpx_stats  *cs,
	            parameters *pars);


#endif
