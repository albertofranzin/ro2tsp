#ifndef CPX_SET_PARAMETERS_H_
#define CPX_SET_PARAMETERS_H_


#include <stdlib.h>
#include <stdio.h>
#include <ilcplex/cplex.h>
#include "../../base/global.h"
#include "../../base/utils.h"

#include "../../data/environment.h"
#include "../../data/parameters.h"

/**
 * cpx_set_parameters
 * set some parameters
 *
 * @param env   CPLEX environment
 * @param lp    CPLEX problem
 * @param ce    cpx environment
 * @return      status of the function
 */
int cpx_set_parameters(CPXENVptr	cplexenv,
					   CPXLPptr     lp,
                       environment *env,
                       parameters  *pars);

#endif /* CPX_SET_PARAMETERS_H_ */
