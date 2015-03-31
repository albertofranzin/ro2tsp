#ifndef CPX_PREPROCESSING_H_
#define CPX_PREPROCESSING_H_

#include <stdlib.h>
#include <stdio.h>
#include <ilcplex/cplex.h>
#include "../../base/global.h"
#include "../../base/utils.h"

#include "../../data/environment.h"
#include "../../data/statistics.h"
#include "../../data/parameters.h"
#include "../../data/graph.h"
#include "../../data/tree.h"
#include "../../data/cycle.h"

#include "../../algos/compute_ub.h"
#include "../../algos/pr_lagrange_hk.h"
#include "../../algos/reduce.h"

/**
 * preprocessing: compute initial upper-bound
 * @param env	CPLEX environment
 * @param lp    CPLEX problem
 * @param ce	cpx environment
 * @param cs	cpx environment
 * @param pars	user parameters
 * @return 		status of the function
 */
int cpx_preprocessing(CPXENVptr	 	cplexenv,
		      	  	  CPXLPptr   	lp,
		      	  	  environment  *env,
				      parameters   *pars,
		      	  	  statistics   *stats);


#endif /* CPX_PREPROCESSING_H_ */
