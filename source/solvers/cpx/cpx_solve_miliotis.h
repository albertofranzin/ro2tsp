#ifndef CPX_SOLVE_MILIOTIS_H_
#define CPX_SOLVE_MILIOTIS_H_

#include <stdlib.h>
#include <stdio.h>
#include <ilcplex/cplex.h>
#include "../../base/global.h"
#include "../../base/utils.h"

#include "../../data/environment.h"
#include "../../data/statistics.h"
#include "../../data/parameters.h"

#include "../../solvers/cpx/cpx_cstr.h"
#include "../../solvers/cpx/cpx_create_problem.h"
#include "../../solvers/cpx/cpx_setup_problem.h"
#include "../../solvers/cpx/cpx_set_parameters.h"
#include "../../solvers/cpx/cpx_preprocessing.h"


typedef struct _flow {

	int    index;
	int    edge;
	double capacity;

} flow;

/**
 * solve the problem using Miliotis' approach
 * @param env   CPLEX environment
 * @param lp    CPLEX problem
 * @param ce    cpx environment
 * @param cs    cpx statistics
 * @param x   computed solution
 * @param x_size  x length
 * @param solstat CPLEX status of the solution
 * @return    status of the function
 */
int cpx_solve_miliotis(CPXENVptr   	cplexenv,
                       CPXLPptr    	lp,
                       environment *env,
                       parameters  *pars,
                       statistics  *stats,
                       double      *x,
                       int          x_size,
                       int         *solstat);

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
int CPXPUBLIC cpx_callback_miliotis(CPXENVptr cplexenv,
                                    void     *cbdata,
                                    int       wherefrom,
                                    void     *cbhandle,
                                    int      *useraction_p);


int CPXPUBLIC cpx_callback_maxflow(CPXENVptr  env,
                                    void     *cbdata,
                                    int       wherefrom,
                                    void     *cbhandle,
                                    int      *useraction_p);

int compare_flows(const void *f1, const void *f2);


#endif /* CPX_SOLVE_MILIOTIS_H_ */
