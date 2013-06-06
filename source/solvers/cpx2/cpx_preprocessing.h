#ifndef __CPX_PREPROCESSING_H
#define __CPX_PREPROCESSING_H


#include <stdio.h>
#include <ilcplex/cplex.h>

#include "../../base/constants.h"
#include "../../base/utils.h"

#include "../../data/graph.h"
#include "../../data/cycle.h"

#include "../../algos/compute_upper_bound.h"
#include "../../algos/compute_lagrange.h"
#include "../../algos/compute_deltas.h"

#include "cpx_env.h"
#include "cpx_stats.h"


/**
 * preprocessing: compute initial upper-bound
 * @param env	CPLEX environment
 * @param lp    CPLEX problem
 * @param ce	cpx environment
 * @param cs	cpx environment
 * @param pars	user parameters
 * @return 		status of the function
 */
int cpx_preprocessing(CPXENVptr	 env,
		      CPXLPptr   lp,
		      cpx_env	 *ce,
		      cpx_stats	 *cs,
		      parameters *pars);


#endif
