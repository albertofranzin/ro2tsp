#include "cpx_create_problem.h"

int cpx_create_problem(CPXENVptr *env, CPXLPptr *lp, char *probname) {
  int status;

  *env = CPXopenCPLEX(&status);

  *lp = CPXcreateprob(*env, &status, probname);

  status = CPXsetintparam(*env, CPX_PARAM_LPMETHOD, CPX_ALG_AUTOMATIC);

  return status;
}
