#ifndef CPX_SOLVE_RINS_H
#define CPX_SOLVE_RINS_H

#include <stdlib.h>
#include <ilcplex/cplex.h>

#include "../../base/constants.h"
#include "cpx_env.h"
#include "cpx_stats.h"
#include "cpx_table.h"
#include "cpx_constraint.h"
#include "cpx_components.h"
#include "cpx_solve_iterative.h"
#include "cpx_solve_miliotis.h"

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
int cpx_solve_rins(CPXENVptr   env,
                   CPXLPptr    lp,
                   cpx_env    *ce,
                   cpx_stats  *cs,
                   int        *zeros,
                   int        *ones,
                   double     *ret_x,
                   int         x_size,
                   int        *ret_solstat);

#endif