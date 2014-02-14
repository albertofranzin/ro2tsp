#ifndef CPX_SOLVE_MILIOTIS_H_
#define CPX_SOLVE_MILIOTIS_H_

#include <stdlib.h>
#include <ilcplex/cplex.h>
#include <assert.h>

#include "../../base/constants.h"
#include "cpx_env.h"
#include "cpx_stats.h"
#include "cpx_constraint.h"
#include "cpx_components.h"


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
int cpx_solve_miliotis(CPXENVptr   env, 
                       CPXLPptr    lp, 
                       cpx_env    *ce, 
                       cpx_stats  *cs, 
                       double     *x, 
                       int         x_size, 
                       int        *solstat);


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
int CPXPUBLIC cpx_callback_miliotis(CPXENVptr env,
                                    void     *cbdata,
                                    int       wherefrom,
                                    void     *cbhandle,
                                    int      *useraction_p);



int CPXPUBLIC cpx_callback_maxflow(CPXENVptr  env,
                                    void     *cbdata,
                                    int       wherefrom,
                                    void     *cbhandle,
                                    int      *useraction_p);

/**
 * compute mincut
 * @param  ce [description]
 * @param  x  [description]
 * @param  s  start
 * @param  t  end
 * @return    [description]
 */
int mincut(cpx_env  *ce,
           double   *x,
           int       s,
           int       t);


int comp_flows(const void * f1, const void *f2);

#endif