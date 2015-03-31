#ifndef CPX_SETUP_PROBLEM_H_
#define CPX_SETUP_PROBLEM_H_

#include <stdlib.h>
#include <stdio.h>
#include <ilcplex/cplex.h>
#include "../../base/global.h"
#include "../../base/utils.h"

#include "../../data/environment.h"
#include "../../data/parameters.h"

#include "../../solvers/cpx/cpx_cstr.h"


/**
 * cpx_setup_problem
 * fill the problem with TSP data
 *
 * @param env   CPLEX environment
 * @param lp    CPLEX problem
 * @param ce    cpx environment
 * @return      status of the function
 */
int cpx_setup_problem(CPXENVptr		cplexenv,
                      CPXLPptr    	lp,
                      environment  *env,
                      parameters   *pars);

#endif /* CPX_SETUP_PROBLEM_H_ */
