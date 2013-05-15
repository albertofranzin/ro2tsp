#ifndef __CPX_CALLBACKS_H
#define __CPX_CALLBACKS_H


#include <ilcplex/cplex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpx_solver.h"
#include "cpx_mark_subtours.h"
#include "cpx_add_sec.h"
#include "cpx_table.h"

/**
 * parameters passed to the callback
 */
typedef struct _cutinfo {
  CPXLPptr   lp;
  int        numcols;
  int        num;
  double    *x;
  int       *beg;
  int       *ind;
  double    *val;
  double    *rhs;
  cpx_table  hash_table;
  int        number_of_nodes;
} cutinfo;

/**
 * [cpx_cut_callback description]
 * @param  env          CPLEX environment
 * @param  lp           problem
 * @param  cbdata       pointer to callback parameters
 * @param  wherefrom    integer reporting where the callback was called from
 * @param  cbhandle     poiter to cutinfo struct, containing parameters
 *                      to the user-written callback
 * @param  useraction_p action to be taken on callback completion
 * @return              status of the operation performed
 */
int CPXPUBLIC cpx_cut_callback(CPXCENVptr  env,
                               void       *cbdata,
                               int         wherefrom,
                               void       *cbhandle,
                               int        *useraction_p);

#endif
