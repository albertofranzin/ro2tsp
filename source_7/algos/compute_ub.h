#ifndef COMPUTE_UB_H_
#define COMPUTE_UB_H_

#include <stdlib.h>
#include <stdio.h>

#include "../base/global.h"
#include "../base/utils.h"

#include "../data/graph.h"
#include "../data/cycle.h"

#include "../algos/heur_nn.h"
#include "../algos/heur_rc.h"
#include "../algos/heur_2opt.h"
#include "../algos/heur_3opt.h"
#include "../algos/heur_nn2opt.h"
#include "../algos/heur_rc2opt.h"

int compute_ub(graph *g, int algo, cycle *best_c, double *best_ub, int *ones, int *zeros);

#endif /* COMPUTE_UB_H_ */
