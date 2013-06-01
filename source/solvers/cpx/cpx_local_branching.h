#ifndef __LOCAL_BRANCHING_H
#define __LOCAL_BRANCHING_H

#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <ilcplex/cplex.h>

#include "cpx_table.h"
#include "cpx_cpool.h"
#include "cpx_create_problem.h"
#include "cpx_setup_problem.h"
#include "cpx_add_secs.h"

#include "../../base/constants.h"
#include "../../base/utils.h"

#include "../../data/graph.h"
#include "../../data/cycle.h"
#include "../../data/onetree.h"
#include "../../data/egraph.h"
#include "../../data/tsp_env.h"
#include "../../data/tsp_stats.h"

/**
 * [cpx_local_branching description]
 * @param  te   [description]
 * @param  ts   [description]
 * @param  pars [description]
 * @return      [description]
 */
int cpx_local_branching(tsp_env    *te,
                        tsp_stats  *ts,
                        parameters *pars);


#endif
