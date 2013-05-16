#ifndef BB_SOLVER_H_
#define BB_SOLVER_H_

#include <math.h>
#include <stdio.h>
#include "../../base/constants.h"
#include "../../data/graph.h"
#include "../../data/onetree.h"
#include "../../data/cycle.h"
#include "../../algos/compute_deltas.h"
#include "../../algos/compute_ot.h"
#include "../../algos/compute_lagrange.h"
#include "../../algos/compute_upper_bound.h"
#include "bb_stats.h"
#include "bb_env.h"
#include "bb_bkp.h"
#include "bb_select_node.h"
#include "bb_select_edges.h"
#include "bb_propagate.h"
#include "bb_remove_fat_edges.h"

void bb_solver(bb_env* env, bb_stats* stats);
int bb_compute_current_ot(bb_env* env, bb_stats* stats);

#endif
