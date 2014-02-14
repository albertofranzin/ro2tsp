#include "cpx_solve_iterative.h"

int cpx_solve_iterative(CPXENVptr   env, 
                        CPXLPptr    lp, 
                        cpx_env    *ce, 
                        cpx_stats  *cs, 
                        double     *x, 
                        int         x_size, 
                        int        *solstat) {


  int n       = ce->G.n;
  int numcols = n * (n - 1) / 2;
  int vrtx_comp[n];
  int status, numcomp, my_comp;

  cpx_constraint sec;
  cpx_constraint_init(&sec, numcols);


  
  status = CPXsetintparam (env, CPX_PARAM_MIPSEARCH, CPX_MIPSEARCH_DYNAMIC);
  //status = CPXsetintparam (env, CPX_PARAM_MIPSEARCH, CPX_MIPSEARCH_TRADITIONAL);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_iterative.c:\n"
                    "function: cpx_solve_iterative:\n"
                    "CPXsetinparam (CPX_PARAM_MIPSEARCH) : %d\n", status);
    return status;
  }

  // each variable in the original model can be expressed as
  // a linear form of variables in the presolved model
  //status = CPXsetintparam (env, CPX_PARAM_PRELINEAR, CPX_ON);
  status = CPXsetintparam (env, CPX_PARAM_PRELINEAR, CPX_OFF);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_iterative.c:\n"
                    "function: cpx_solve_iterative:\n"
                    "CPXsetinparam (CPX_PARAM_PRELINEAR) : %d\n", status);
    return status;
  }

  //status = CPXsetintparam (env, CPX_PARAM_REDUCE, CPX_PREREDUCE_NOPRIMALORDUAL);
  status = CPXsetintparam (env, CPX_PARAM_REDUCE, CPX_PREREDUCE_PRIMALANDDUAL);
  //status = CPXsetintparam (env, CPX_PARAM_REDUCE, CPX_PREREDUCE_PRIMALONLY);
  //status = CPXsetintparam (env, CPX_PARAM_REDUCE, CPX_PREREDUCE_DUALONLY);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_iterative.c:\n"
                    "function: cpx_solve_iterative:\n"
                    "CPXsetinparam (CPX_PARAM_REDUCE) : %d\n", status);
    return status;
  }

  // use reduced (on) or original (off)?
  status = CPXsetintparam (env, CPX_PARAM_MIPCBREDLP, CPX_ON);
  //status = CPXsetintparam (env, CPX_PARAM_MIPCBREDLP, CPX_OFF);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_iterative.c:\n"
                    "function: cpx_solve_iterative:\n"
                    "CPXsetinparam (CPX_PARAM_MIPCBREDLP) : %d\n", status);
    return status;
  }
  

  /*
  status = CPXsetintparam (env, CPX_PARAM_PREDUAL, -1);
  //status = CPXsetintparam (env, CPX_PARAM_PREDUAL, 0);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_iterative.c:\n"
                    "function: cpx_solve_iterative:\n"
                    "CPXsetinparam (CPX_PARAM_PREDUAL) : %d\n", status);
    return status;
  }
  */
 
  /*status = CPXsetdblparam(env, CPX_PARAM_CUTUP, ce->init_ub);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_iterative.c:\n"
                    "function: cpx_solve_iterative:\n"
                    "CPXsetdblparam (CPX_PARAM_CUTUP) : %d\n", status);
    return status;
  }

  status = CPXsetdblparam(env, CPX_PARAM_CUTLO, ce->init_lb);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_iterative.c:\n"
                    "function: cpx_solve_iterative:\n"
                    "CPXsetdblparam (CPX_PARAM_CUTLO) : %d\n", status);
    return status;
  }*/

  if (x_size != numcols) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_iterative.c:\n"
                    "function: cpx_solve_iterative:\n"
                    "x_size != numcols\n");
    return status;
  }

  // WHY THIS???? don't remember
  status = CPXpresolve(env, lp, CPX_ALG_NONE);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_iterative.c:\n"
                    "function: cpx_solve_iterative:\n"
                    "CPXpresolve : %d\n", status);
    return status;
  }


  // Actually solve the problem, iterating while there are more than one
  // connected components (aka subtours)
  numcomp = 0;
  while (numcomp != 1) {
  //*solstat = 0;
  //while (*solstat != 101 && *solstat != 102) {//<----------------- mod, proximity
    
    // optimize (= solve)
    status = CPXmipopt(env, lp);
    if (status) {
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_iterative.c:\n"
                      "function: cpx_solve_iteraitve:\n"
                      "CPXmipopt : %d\n", status);
      return 1;
    }
  
    //getchar();

    // retrieve solution status. Exit if CPXMIP_INFEASIBLE
    *solstat = CPXgetstat(env, lp);
    if (*solstat == 103) break;


    if (CPXgetnumcols(env, lp) != numcols) {
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_iterative.c:\n"
                      "function: cpx_solve_iterative:\n"
                      "unexpected number of columns in the current solution\n");
      return status;
    }

    // retrieve solution coefficients
    status = CPXgetx(env, lp, x, 0, numcols-1);
    if (status) {
        fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_iterative.c:\n"
                        "function: cpx_solve_iteraitve:\n"
                        "CPXgetx : %d\n", status);
        return 1;
    }

    /*
    int i;
    double origx[numcols];
    CPXuncrushx(env, lp, origx, x);

    for (i = 0; i < numcols; i++) {
      printf("%.3f | %.3f\n", x[i], origx[i]);
    }
    getchar();
    */
    
    // don't remember what this does
    status = CPXfreepresolve(env, lp);
    if (status) {
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_iterative.c:\n"
                      "function: cpx_solve_iteraitve:\n"
                      "CPXfreepresolve : %d\n", status);
      return 1;
    }
    
    // mark the connected components, in order to count them
    status = cpx_mark_components(ce, x, numcols, vrtx_comp, n, &numcomp);
    if (status) {
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_iterative.c:\n"
                      "function: cpx_solve_iteraitve:\n"
                      "cpx_mark_components : %d\n", status);
      return 1;
    }

    // if there are more than one connected components, and therefore subtours,
    // generate the relative constraints and add them to the model,
    // before another iteration of CPLEX
    if (numcomp > 1) {
      for (my_comp = 0; my_comp < numcomp; my_comp++) {
        status = cpx_constraint_generate_sec(ce, vrtx_comp, n, my_comp, &sec);
        if (status) {
          fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_iterative.c:\n"
                          "function: cpx_solve_iteraitve:\n"
                          "cpx_constraint_sec : %d\n", status);
          return 1;
        }

        status = cpx_constraint_add(env, lp, &sec);
        if (status) {
          fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_iterative.c:\n"
                          "function: cpx_solve_iteraitve:\n"
                          "cpx_constraint_add : %d\n", status);
          return 1;
        }
      }

      if (ce->pars->verbosity >= ESSENTIAL) {
        printf("# SECs added: %d\n", numcomp);
      }
    }

  }

  cpx_constraint_delete(&sec);
  return 0;
}
