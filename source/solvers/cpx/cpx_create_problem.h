#ifndef CPX_CREATE_PROBLEM_H_
#define CPX_CREATE_PROBLEM_H_

#include <assert.h>
#include <ilcplex/cplex.h>

#include "../../base/utils.h"

int cpx_create_problem(CPXENVptr   *env,
                       CPXLPptr    *lp,
                       char        *probname,
                       parameters  *pars);

#endif
