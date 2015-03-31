#ifndef HEUR_NN2OPT_H_
#define HEUR_NN2OPT_H_

#include <stdlib.h>
#include <stdio.h>

#include "../base/global.h"
#include "../base/utils.h"

#include "../data/graph.h"
#include "../data/cycle.h"

#include "../algos/heur_nn.h"
#include "../algos/heur_2opt.h"

/**
 * compute NN heuristic starting from each node. Also applies 2-opt
 * @param  g  graph
 * @param  c  best cycle found
 * @param  ub upper bound computed
 * @return    status of the operations
 */
int heur_nn2opt(graph *g, cycle *best_c, double *best_ub);

#endif /* HEUR_NN2OPT_H_ */
