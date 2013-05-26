#ifndef __PREPROCESSING_H
#define __PREPROCESSING_H

#include <stdio.h>
#include <assert.h>

#include "../base/constants.h"
#include "../base/utils.h"

#include "../data/graph.h"
#include "../data/tsp_env.h"
#include "../data/tsp_stats.h"

#include "../algos/compute_upper_bound.h"
#include "../algos/compute_lagrange.h"

/**
 * preprocess the tsp instance, computing upper and lower bounds
 * on the solution, and initializing problem statistics
 * @param G    the graph
 * @param pars parameters of the problem
 * @param te   environment structure
 * @param ts   problem stats
 * @return     status of preprocessing
 */
int preprocessing(graph      *G,
                  parameters *pars,
                  tsp_env    *te,
                  tsp_stats  *ts);

#endif
