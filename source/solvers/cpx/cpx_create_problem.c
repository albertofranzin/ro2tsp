#include "cpx_create_problem.h"

int cpx_create_problem(CPXENVptr *env, CPXLPptr *lp, char *probname) {
  int status;

  *env = CPXopenCPLEX(&status);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_create_problem.c:\n");
    fprintf(stderr, " CPXopenCPLEX : %d\n", status);
    fprintf(stderr, "Error while opening CPLEX environment, exiting.\n");
    exit(1);
  }

  *lp = CPXcreateprob(*env, &status, probname);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_create_problem.c:\n");
    fprintf(stderr, " CPXcreateprob : %d\n", status);
    fprintf(stderr, "Error while creating the problem, exiting.\n");
    exit(1);
  }

  status = CPXsetintparam(*env, CPX_PARAM_LPMETHOD, CPX_ALG_AUTOMATIC);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_create_problem.c:\n");
    fprintf(stderr, "Error while setting lp algorithm, exiting.\n");
    exit(1);
  }

  return status;
}
