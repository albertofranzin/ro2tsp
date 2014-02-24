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

int cpx_solve_iterative(environment *env, parameters *pars, statistics *stats);

#endif /* CPX_SOLVE_ITERATIVE_H_ */
