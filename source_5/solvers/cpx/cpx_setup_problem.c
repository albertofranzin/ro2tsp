#include "cpx_setup_problem.h"

int cpx_create_problem(CPXENVptr  *env,
                       CPXLPptr   *lp,
                       char       *probname) {

  int status;

  // create CPLEX problem environment
  *env = CPXopenCPLEX(&status);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c:\n"
                    "function: cpx_create_problem:\n"
                    "CPXopenCPLEX : %d\n", status);
    return status;
  }

  // create linear problem
  *lp = CPXcreateprob(*env, &status, probname);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c:\n"
                    "function: cpx_create_problem:\n"
                    "CPXcreateprob : %d\n", status);
    return status;
  }

  return 0;
}


int cpx_setup_problem(CPXENVptr   env,
                      CPXLPptr    lp,
                      cpx_env    *ce) {

  int status  = 0;
  int n       = ce->G_INPUT.n;
  int numcols = n * (n - 1) / 2;
  int idx, i, j, k;

#ifdef DEBUG
  if (ce->pars->verbosity >= USEFUL) {
    printf("solvers/cpx/cpx_setup_problem.c :: "
           "cpx_setup_problem: n = %d, numcols = %d\n", n, numcols);
  }
#endif

  // set problem as Mixed-Integer LP
  CPXchgprobtype(env, lp, CPXPROB_MILP);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c:\n"
                    "function: cpx_setup_problem:\n"
                    "CPXchgprobtype : %d\n", status);
    return status;
  }


  // set objective function
  // I try to malloc 'em, to bypass the 8MB limit
  double *lb;      // lower bound on variables
  double *ub;      // upper bound on variables
  char   *xtype;   // type of variables
  double *obj;     // objected value computed for the variables
  char  **colname; // name of columns (really need this?)

  lb      = (double *)calloc(numcols, sizeof(double));
  ub      = (double *)calloc(numcols, sizeof(double));
  xtype   = (char *)  calloc(numcols, sizeof(char));
  obj     = (double *)calloc(numcols, sizeof(double));
  colname = (char **) calloc(numcols, sizeof(char *));

  for (i = 0; i < numcols; i++) {
    colname[i] = (char *)calloc(100, sizeof(char));
  }

  // set variables: binary -> {0,1} -> [0,1] in the relaxated problem
  // set bounds and retrieve variable corresponding to the vertex, according to
  // its position in the table
  for (idx = 0; idx < numcols; idx++) {
    lb[idx]    = 0.0;
    ub[idx]    = 1.0;
    xtype[idx] = 'B';
    vrtx_from_idx(&ce->T, &i, &j, idx);
    sprintf(colname[idx], "x_%d_%d", i, j);
    obj[idx] = graph_get_edge_cost(&ce->G_INPUT, i, j); 
  }

  /*
  moved operation into previous cycle
  for (i = 0; i < n; i++) {
    for (j = i+1; j < n; j++) {
      idx_from_vrtx(&ce->T, i, j, &idx);
      obj[idx] = graph_get_edge_cost(&ce->G_INPUT, i, j); 
    }
  }
  */

  // create constraints and add them to the problem
  status = CPXnewcols(env, lp, numcols, obj, lb, ub, xtype, colname);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c:\n"
                    "function: cpx_setup_problem:\n"
                    "PXnewcols : %d\n", status);
    return status;
  }

  // set problem as a minimum problem
  status = CPXchgobjsen(env, lp, CPX_MIN);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c:\n"
                    "function: cpx_setup_problem:\n"
                    "CPXchgobjsen : %d\n", status);
    return status;
  }


  // set degree constraints //
  cpx_constraint c;
  cpx_constraint_init(&c, n-1);

  c.nzcnt   = n-1;
  c.rmatbeg = 0;
  c.rhs     = 2.0;
  c.sense   = 'E';

  for (i = 0; i < n; i++) {
    sprintf(c.rowname, "deg_cstr_%d", i);
    k = 0;
    for (j = 0; j < n; j++) {
      if (j != i) {
        idx_from_vrtx(&ce->T, i, j, &idx);
        c.rmatind[k] = idx;
        c.rmatval[k] = 1.0;
        k++;
      }
    }

    status = cpx_constraint_add(env, lp, &c);
    if (status) {
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c:\n"
                      "function: cpx_setup_problem:\n"
                      "cpx_constraint_add : %d\n", status);
      return status;
    }
  }

  free(ub);
  free(lb);
  free(xtype);
  free(obj);
  free(colname);
  cpx_constraint_delete(&c);
  return 0;

}


int cpx_set_problem_parameters(CPXENVptr  env,
                               CPXLPptr   lp,
                               cpx_env   *ce) {

  int status;

  // set cplex algo
  status = CPXsetintparam(env, CPX_PARAM_LPMETHOD, CPX_ALG_AUTOMATIC);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c:\n"
                    "function: cpx_set_problem_parameters:\n"
                    "CPXsetinparam (CPX_PARAM_LPMETHOD) : %d\n", status);
    return status;
  }

  // turn on/off CPLEX messages during execution
  if (ce->pars->verbosity >= USEFUL) {
    status = CPXsetintparam(env, CPX_PARAM_SCRIND, CPX_ON);
  } else {
    status = CPXsetintparam(env, CPX_PARAM_SCRIND, CPX_OFF);
  }
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c:\n"
                    "function: cpx_set_problem_parameters:\n"
                    "CPXsetinparam (CPX_PARAM_SCRIND) : %d\n", status);
    return status;
  }

  /*
  status = CPXsetintparam (env, CPX_PARAM_PRELINEAR, CPX_OFF);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c:\n"
                    "function: cpx_set_problem_parameters:\n"
                    "CPXsetinparam (CPX_PARAM_PRELINEAR) : %d\n", status);
    return status;
  }
  */

  /*
  status = CPXsetintparam (env, CPX_PARAM_MIPSEARCH, CPX_MIPSEARCH_TRADITIONAL);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c:\n"
                    "function: cpx_set_problem_parameters:\n"
                    "CPXsetinparam (CPX_PARAM_MIPSEARCH) : %d\n", status);
    return status;
  }
  */
  /*
  status = CPXsetintparam (env, CPX_PARAM_REDUCE, CPX_PREREDUCE_NOPRIMALORDUAL);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c:\n"
                    "function: cpx_set_problem_parameters:\n"
                    "CPXsetinparam (CPX_PARAM_REDUCE) : %d\n", status);
    return status;
  }
  */
  /*
  status = CPXsetintparam (env, CPX_PARAM_MIPCBREDLP, CPX_OFF);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c:\n"
                    "function: cpx_set_problem_parameters:\n"
                    "CPXsetinparam (CPX_PARAM_MIPCBREDLP) : %d\n", status);
    return status;
  }
  */

  return 0;

}
