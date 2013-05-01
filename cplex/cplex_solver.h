#ifndef __CPLEX_SOLVER_H
#define __CPLEX_SOLVER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ilcplex/cplex.h>
#include <assert.h>
#include "utils.h"

/*
 * cplex_solve_problem
 *
 * - CPXENVptr : pointer to the CPLEX environment
 * - CPXLPptr : pointer to the CPLEX LP problem
 *
 * solve a LP problem using CPLEX
 *
 */
void cplex_solve_problem(CPXENVptr, CPXLPptr);

#endif