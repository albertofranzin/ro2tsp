#include "../../solvers/cpx/cpx_set_parameters.h"

int cpx_set_parameters(CPXENVptr	cplexenv,
                       CPXLPptr  	lp,
                       environment *env,
                       parameters  *pars) {

  int status;

  /* set cplex algo */
  status = CPXsetintparam(cplexenv, CPX_PARAM_LPMETHOD, CPX_ALG_AUTOMATIC);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c:\n"
                    "function: cpx_set_problem_parameters:\n"
                    "CPXsetinparam (CPX_PARAM_LPMETHOD) : %d\n", status);
    return status;
  }

  /* turn on/off CPLEX messages during execution */
  if (pars->verbosity >= USEFUL) {
    status = CPXsetintparam(cplexenv, CPX_PARAM_SCRIND, CPX_ON);
  } else {
    status = CPXsetintparam(cplexenv, CPX_PARAM_SCRIND, CPX_OFF);
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
