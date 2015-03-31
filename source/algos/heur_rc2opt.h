#ifndef HEUR_RC2OPT_H_
#define HEUR_RC2OPT_H_

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "../base/global.h"
#include "../base/utils.h"

#include "../data/graph.h"
#include "../data/cycle.h"


#include "../algos/heur_rc.h"
#include "../algos/heur_2opt.h"


/**
 * compute an ub using RC+2-opt heuristic
 * @param  g       graph
 * @param  c       best cycle found
 * @param  ub      uper bound computed
 * @param  ones    list of variables always found to be 1
 * @param  n_ones  length of ones
 * @param  zeros   list of variables always found to be 0
 * @param  n_zeros length of zeros
 * @return         status of the operations
 */
int heur_rc2opt(graph  *g, cycle  *best_c, double *best_ub, int *ones, int *zeros);


/**
 * compute a rc+2opt ub. Thought to be multithreading
 * @param p params
 */
void *rc_thread(void *p);


#endif /* HEUR_RC2OPT_H_ */
