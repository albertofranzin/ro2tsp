#ifndef CPX_SOLVE_LOCAL_BRANCHING_H_
#define CPX_SOLVE_LOCAL_BRANCHING_H_

#include <stdlib.h>
#include <ilcplex/cplex.h>

#include "../../base/constants.h"
#include "cpx_env.h"
#include "cpx_stats.h"
#include "cpx_table.h"
#include "cpx_constraint.h"
#include "cpx_components.h"
#include "cpx_solve_iterative.h"


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
int cpx_solve_local_branching(CPXENVptr   env,
                              CPXLPptr    lp,
                              cpx_env    *ce,
                              cpx_stats  *cs,
                              double     *ret_x,
                              int         x_size,
                              int        *ret_solstat);

#endif