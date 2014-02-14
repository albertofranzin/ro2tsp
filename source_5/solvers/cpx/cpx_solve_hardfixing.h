#ifndef CPX_SOLVE_HARDFIXING_H_
#define CPX_SOLVE_HARDFIXING_H_

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

int cpx_solve_hardfixing(CPXENVptr   env,
                         CPXLPptr    lp,
                         cpx_env    *ce,
                         cpx_stats  *cs,
                         int        *zeros,
                         int        *ones,
                         double     *ret_x,
                         int         x_size,
                         int        *ret_solstat);

#endif