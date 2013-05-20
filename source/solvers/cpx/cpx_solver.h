#ifndef CPX_SOLVER_H_
#define CPX_SOLVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ilcplex/cplex.h>

#include "../../data/graph.h"
#include "../../data/egraph.h"
#include "../../data/tsp_env.h"
#include "../../data/tsp_stats.h"

#include "../../base/utils.h"

#include "cpx_table.h"
#include "cpx_create_problem.h"
#include "cpx_setup_problem.h"
#include "cpx_add_sec.h"
#include "cpx_mark_subtours.h"
#include "cpx_callbacks.h"

/**
 * [cpx_solver description]
 *
 * @param te    problem environment
 * @param ts    problem stats
 * @param pars  user parameters
 */
void cpx_solver(tsp_env *te, tsp_stats *ts, parameters *pars);

#endif
