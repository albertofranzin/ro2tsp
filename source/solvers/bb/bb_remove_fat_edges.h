#ifndef BB_REMOVE_FAT_EDGES_H_
#define BB_REMOVE_FAT_EDGES_H_

#include "../../base/constants.h"
#include "../../data/onetree.h"
#include "../../data/graph.h"
#include "../../data/clist.h"
#include "../../data/cnstr.h"

#include "bb_env.h"
#include "bb_stats.h"
#include "../../algos/compute_deltas.h"

void bb_remove_fat_edges(bb_env* env, bb_stats* stats, double ub);

#endif
