#ifndef BB_SOLVER_H_
#define BB_SOLVER_H_

#include <math.h>
#include <stdio.h>
#include "../../base/constants.h"
#include "../../data/egraph.h"
#include "../../data/graph.h"
#include "../../data/onetree.h"
#include "../../data/cycle.h"
#include "../../data/tsp_env.h"
#include "../../data/tsp_stats.h"

#include "../../algos/compute_min_ot.h"
#include "../../algos/compute_lagrange.h"
#include "../../algos/compute_upper_bound.h"

#include "bb_bkp.h"
#include "bb_select_node.h"
#include "bb_select_edges.h"
#include "bb_propagate.h"

void bb_solver(tsp_env* te, tsp_stats* ts);
int bb_compute_current_ot(tsp_env* te, tsp_stats* ts);

#endif

