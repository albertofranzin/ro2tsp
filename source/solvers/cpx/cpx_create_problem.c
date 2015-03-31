#include "../../solvers/cpx/cpx_create_problem.h"


int cpx_create_problem(CPXENVptr  *cplexenv,
                       CPXLPptr   *lp,
                       char       *probname) {

  int status;

  /* create CPLEX problem environment */
  *cplexenv = CPXopenCPLEX(&status);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c:\n"
                    "function: cpx_create_problem:\n"
                    "CPXopenCPLEX : %d\n", status);
    return status;
  }

  /* create linear problem */
  *lp = CPXcreateprob(*cplexenv, &status, probname);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c:\n"
                    "function: cpx_create_problem:\n"
                    "CPXcreateprob : %d\n", status);
    return status;
  }

  return 0;
}
