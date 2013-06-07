#ifndef CPX_SETUP_PROBLEM_H_
#define CPX_SETUP_PROBLEM_H_

#include <stdlib.h>
#include <ilcplex/cplex.h>

#include "../../base/constants.h"
#include "../../data/graph.h"
#include "cpx_table.h"
#include "cpx_env.h"
#include "cpx_constraint.h"

/**
 * create cplex problem and fill it with TSP data
 * @param env		CPLEX environment
 * @param lp		CPLEX problem
 * @param probname	problem name
 * @return 		status of the function
 */
int cpx_create_problem(CPXENVptr	*env,
                       CPXLPptr		*lp,
                       char		*probname);


/**
 * fill the problem with TSP data
 * @param env		CPLEX environment
 * @param lp		CPLEX problem
 * @param ce		cpx environment
 * @return 		status of the function
 */
int cpx_setup_problem(CPXENVptr       	env,
                      CPXLPptr		lp,
		      cpx_env		*ce);


/**
 * set some parameters
 * @param env		CPLEX environment
 * @param lp		CPLEX problem
 * @param ce		cpx environment
 * @return 		status of the function
 */
int cpx_set_problem_parameters(CPXENVptr	env,
               	               CPXLPptr		lp,
		               cpx_env		*ce);


#endif
