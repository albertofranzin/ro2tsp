#ifndef CPX_SOLVE_PROXIMITY_H_
#define CPX_SOLVE_PROXIMITY_H_

#include <stdlib.h>
#include <ilcplex/cplex.h>

#include "../../base/constants.h"
#include "cpx_env.h"
#include "cpx_stats.h"
#include "cpx_constraint.h"
#include "cpx_solve_iterative.h"

#include "../../algos/compute_deltas.h"

/**
 * solve the problem using proximity-search approach
 * @param env		CPLEX environment
 * @param lp		CPLEX problem
 * @param ce		cpx environment
 * @param cs		cpx statistics
 * @param x		computed solution
 * @param x_size	x length
 * @param solstat	CPLEX status of the solution
 * @return 		status of the function
 */
int cpx_solve_proximity(CPXENVptr	env, 
		       CPXLPptr		lp, 
		       cpx_env		*ce, 
		       cpx_stats	*cs, 
		       double*		x, 
		       int		x_size, 
		       int*		solstat);

/**
 * recenter the objective function with respect to the
 * new feasible solution
 * @param env		CPLEX environment
 * @param lp		CPLEX problem
 * @param x_feas	feasible solution
 * @param x_feas_sixe	size of x_feas
 * @return 		status of the function
 */
int cpx_recenter_obj(CPXENVptr	env, 
		     CPXLPptr	lp, 
		     double	*x_feas,
		     int	x_feas_size);

/**
 * update the cutoff constraint
 * @param env		CPLEX environment
 * @param lp		CPLEX problem
 * @param x_feas	feasible solution
 * @param x_feas_sixe	size of x_feas
 * @param theta		proximity-search parameter
 * @param cutoff_idx	index of the row corresponding
 * to the cutoff constraint
 * @return 		status of the function
 */
int cpx_update_cutoff(CPXENVptr	env, 
		      CPXLPptr	lp,
		      cpx_env   *ce,
		      double	*x_feas,
		      int	x_feas_size,
		      double	theta,
		      int	cutoff_idx);


#endif
