#ifndef CPX_SOLVER_H_
#define CPX_SOLVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ilcplex/cplex.h>

#include "../../base/constants.h"
#include "../../base/utils.h"
#include "../../data/graph.h"
#include "../../data/cycle.h"
#include "../../data/onetree.h"
#include "../../data/egraph.h"
#include "../../data/tsp_env.h"
#include "../../data/tsp_stats.h"

#include "../../algos/compute_ot.h"
#include "../../algos/compute_lagrange.h"
#include "../../algos/compute_upper_bound.h"
#include "../../algos/compute_deltas.h"

#include "cpx_table.h"
#include "cpx_create_problem.h"
#include "cpx_setup_problem.h"
#include "cpx_add_secs.h"
#include "cpx_add_kruskal_secs.h"

/**
 * [cpx_solver description]
 * @param te    problem environment
 * @param ts    problem stats
 * @param pars  user parameters
 */
void cpx_solver(tsp_env *te, tsp_stats *ts, parameters *pars);

#endif
