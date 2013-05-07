#ifndef BB_SOLVER_H_
#define BB_SOLVER_H_

#include "../../data/graph.h"
#include "../../data/onetree.h"
#include "../../algos/compute_ot.h"
#include "../../algos/compute_lagrange.h"
#include "../../algos/compute_upper_bound.h"
#include "bb_input.h"
#include "bb_output.h"
#include "bb_stats.h"
#include "bb_status.h"
#include "bb_backup.h"
#include "bb_select_node.h"
#include "bb_select_edges.h"
#include "bb_constraints.h"

void bb_solver(bb_input* input, bb_output* output, bb_status* status, bb_stats* stats);

#endif
