#ifndef CPX_CREATE_PROBLEM_H_
#define CPX_CREATE_PROBLEM_H_

#include <assert.h>
#include <ilcplex/cplex.h>

int cpx_create_problem(CPXENVptr *env, CPXLPptr *lp, char *probname);

#endif
