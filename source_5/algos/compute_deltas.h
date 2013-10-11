#ifndef __COMPUTE_DELTAS_H
#define __COMPUTE_DELTAS_H

#include "../data/graph.h"
#include "../data/onetree.h"

/**
 * compute_deltas
 * check which edges are redundant and can be pruned
 * 
 * @param  G  graph
 * @param  OT 1-tree
 * @return status
 */
int compute_deltas(graph *G, onetree *OT);

#endif
