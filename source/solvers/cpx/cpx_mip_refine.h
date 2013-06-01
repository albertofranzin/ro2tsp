#ifndef __CPX_MIP_REFINE_H
#define __CPX_MIP_REFINE_H

#include <stdio.h>
#include <assert.h>
#include <ilcplex/cplex.h>

#include "cpx_table.h"
#include "cpx_cpool.h"
#include "cpx_create_problem.h"
#include "cpx_setup_problem.h"
#include "cpx_add_secs.h"

#include "../../data/graph.h"
#include "../../data/cycle.h"
#include "../../data/tsp_env.h"
#include "../../data/tsp_stats.h"

#include "../../base/utils.h"
#include "../../base/constants.h"

/**
 * MIP-and-refine heuristic
 * get a feasible solution and refine it
 *
 * @param  te   problem environment
 * @param  ts   problem stats
 * @param  pars user parameters
 * @return      status of the operation
 */
int cpx_mip_refine(tsp_env    *te,
                   tsp_stats  *ts,
                   parameters *pars);

#endif
