#ifndef CPX_CREATE_PROBLEM_H_
#define CPX_CREATE_PROBLEM_H_

#include <stdlib.h>
#include <stdio.h>
#include <ilcplex/cplex.h>
#include "../../base/global.h"
#include "../../base/utils.h"

/**
 * cpx_create_proble
 * create cplex problem
 *
 * @param env       CPLEX environment, to be created here
 * @param lp        CPLEX problem, to be created here
 * @param probname  problem name
 * @return          status of the function
 */
int cpx_create_problem(CPXENVptr  *cplexenv,
                       CPXLPptr   *lp,
                       char       *probname);


#endif /* CPX_CREATE_PROBLEM_H_ */
