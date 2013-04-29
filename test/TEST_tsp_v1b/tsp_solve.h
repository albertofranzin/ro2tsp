#ifndef TSP_SOLVE_H_
#define TSP_SOLVE_H_

#include "graph.h"
#include "onetree.h"
#include "compute_ot.h"
#include "compute_lagrange.h"
#include "../../progetto3/source/compute_upper_bound.h"
#include "tsp_input.h"
#include "tsp_output.h"
#include "tsp_stats.h"
#include "tsp_status.h"
#include "tsp_backup.h"
#include "tsp_select_node.h"
#include "tsp_select_edges.h"
#include "tsp_constraints.h"

void tsp_solve(tsp_input* input, tsp_output* output, tsp_status* status, tsp_stats* stats);

#endif
