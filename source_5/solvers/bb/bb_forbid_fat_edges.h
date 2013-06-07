#ifndef BB_FORBID_FAT_EDGES_H_
#define BB_FORBID_FAT_EDGES_H_

#include "../../base/constants.h"
#include "../../data/onetree.h"
#include "../../data/graph.h"
#include "../../data/tsp_env.h"
#include "../../data/tsp_stats.h"

#include "../../algos/compute_deltas.h"

void bb_forbid_fat_edges(tsp_env* te, tsp_stats* ts, double ub);

#endif
