#ifndef CPX_SOLVE_ITERATIVE_H_
#define CPX_SOLVE_ITERATIVE_H_

#include <stdlib.h>
#include <ilcplex/cplex.h>

#include "../../base/constants.h"
#include "cpx_env.h"
#include "cpx_stats.h"
#include "cpx_constraint.h"
#include "cpx_components.h"


/**
 * solve the problem using Miliotis' approach
 * @param env		CPLEX environment
 * @param lp		CPLEX problem
 * @param ce		cpx environment
 * @param cs		cpx statistics
 * @param x		computed solution
 * @param x_size	x length
 * @param solstat	CPLEX status of the solution
 * @return 		status of the function
 */
int cpx_solve_miliotis(CPXENVptr	env, 
		       CPXLPptr	lp, 
		       cpx_env		*ce, 
		       cpx_stats	*cs, 
		       double*		x, 
		       int		x_size, 
		       int*		solstat);


/**
 * [cpx_subtour_callback description]
 * @param  env          CPLEX environment
 * @param  lp           problem
 * @param  cbdata       pointer to callback parameters
 * @param  wherefrom    integer reporting where the callback was called from
 * @param  cbhandle     poiter to cutinfo struct, containing parameters
 *                      to the user-written callback
 * @param  useraction_p action to be taken on callback completion
 * @return              status of the function
 */
int CPXPUBLIC cpx_callback_miliotis(CPXENVptr	env,
				    void	*cbdata,
				    int 	wherefrom,
				    void	*cbhandle,
				    int		*useraction_p);

#endif
