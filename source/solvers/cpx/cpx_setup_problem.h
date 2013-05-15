#ifndef CPX_SETUP_PROBLEM_H
#define CPX_SETUP_PROBLEM_H

#include <stdlib.h>
#include <assert.h>
#include <ilcplex/cplex.h>

#include "../../data/graph.h"
#include "cpx_table.h"

/**
 * Set up the problem, creating and inserting the objective function,
 * the variables and the first constraints.
 * @param  env        CPLEX environment
 * @param  lp         problem
 * @param  G          graph of the instance
 * @param  hash_table hash table for retrieving edges from their position
 *                    in the list, and viceversa
 * @return            status of the operation
 */
int cpx_setup_problem(CPXENVptr  env,
                      CPXLPptr   lp,
                      graph     *G,
                      cpx_table *hash_table);

#endif
