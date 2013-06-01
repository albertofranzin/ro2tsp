#ifndef cpx_cpool_H_
#define cpx_cpool_H_



#include <stdlib.h>
#include <stdio.h>
#include <ilcplex/cplex.h>

#include "../../base/utils.h"


// ---------------------------------------------------------- //
//                                                            //
//                       data structures                      //
//                                                            //
// ---------------------------------------------------------- //

// generic constraint
typedef struct _cpx_constraint {
  int     cid; // constraint ID - easier to manage than a char *name
  int     num;
  int    *rmatind;
  double *rmatval;
  int     rmatbeg[1];
  double  rhs[1];
  char    sense[1];
} cpx_constraint;

// constraint pool

// pool node (constraint container)
typedef struct _cpx_cpool_node {
  cpx_constraint constraint;
  struct _cpx_cpool_node *prev;
  struct _cpx_cpool_node *next;
} cpx_cpool_node;

// pool
typedef struct _cpx_cpool {
  int size;
  cpx_cpool_node head;
  cpx_cpool_node tail;
} cpx_cpool;


// ---------------------------------------------------------- //
//                                                            //
//                 constraint pool operations                 //
//                                                            //
// ---------------------------------------------------------- //


void cpx_cpool_init(cpx_cpool *cl);


void cpx_cpool_delete(cpx_cpool *cl);


void cpx_cpool_copy(cpx_cpool *FROM, cpx_cpool *TO);


void cpx_cpool_push_last(cpx_cpool *cl,
                         int        cid,
                         int        num,
                         int       *rmatind,
                         double    *rmatval,
                         int        rmatbeg,
                         double     rhs,
                         char       sense);


cpx_constraint *cpx_cpool_pop_first(cpx_cpool *cl);


cpx_constraint *cpx_cpool_pop_last(cpx_cpool *cl);

/**
 * insert a constraint
 * @param cl constraint pool
 * @param c  constraint
 */
void cpx_cpool_insert(cpx_cpool      *cl,
                      cpx_constraint *c);


void cpx_cpool_remove(cpx_cpool *cl, cpx_cpool_node *node);



// ---------------------------------------------------------- //
//                                                            //
//                single constraint operations                //
//                                                            //
// ---------------------------------------------------------- //

/**
 * create local branching constraint (see Fischetti-Lodi paper)
 * @param  x       coefficients of objective function
 * @param  numcols number of columns in the model
 * @param  cid     ID associated to the constraint
 * @param  sense   sense of the inequality (<=, =, >=)
 * @param  rhs     right-hand-side coefficient of the inequality
 * @return         newly created constraint
 */
cpx_constraint *cpx_create_lb_constraint(double  *x,
                                         int      numcols,
                                         int      cid,
                                         char     sense,
                                         double   rhs);

/**
 * add a cpx_constraint to the problem
 * @param  env CPLEX environment
 * @param  lp  problem model
 * @param  c   constraint
 * @return     status of the operation
 */
int cpx_add_constraint(CPXENVptr       env,
                       CPXLPptr        lp,
                       cpx_constraint *c);

/**
 * insert the local branching constraint into the model
 * @param  env  CPLEX environment
 * @param  lp   problem
 * @param  c    local branching constraint
 * @param  pars user parameters
 * @return      status of the operations
 */
int cpx_add_lb_constraint(CPXENVptr       env,
                          CPXLPptr        lp,
                          cpx_constraint *c,
                          parameters     *pars);

#endif
