#include "cpx_solve_hardfixing.h"

int cpx_solve_hardfixing(CPXENVptr   env,
                         CPXLPptr    lp,
                         cpx_env    *ce,
                         cpx_stats  *cs,
                         int        *zeros,
                         int        *ones,
                         double     *ret_x,
                         int         x_size,
                         int        *ret_solstat) {

  int i, j, k, solstat, status = 0,
      n = ce->G.n;
  double z;

  int    *startind,
         *beg      = calloc(1, sizeof(int));
  double *startx   = calloc(x_size, sizeof(double)),
         *bd;
  char   *lu;

  startind = calloc(x_size, sizeof(int));
  lu       = calloc(x_size, sizeof(char));
  bd       = calloc(x_size, sizeof(double));
  k = 0;
  /*for (i = 0; i < x_size; ++i) {
    if (zeros[i] == 1) {
      startind[k] = i;
      lu[k]       = 'B';
      bd[k]       = 0.0;
      k++;
    }
  }

  status = CPXchgbds(env, lp, k, startind, lu, bd);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_rins.c:\n"
                    "function: cpx_solve_rins:\n"
                    "CPXchgbds: %d\n", status);
    return status;
  }*/

  free(startind);
  free(lu);
  free(bd);

  startind = calloc(x_size, sizeof(int));
  lu       = calloc(x_size, sizeof(char));
  bd       = calloc(x_size, sizeof(double));
  k = 0;
  /*for (i = 0; i < x_size; ++i) {
    if (ones[i] == 1) {
      startind[k] = i;
      lu[k]       = 'B';
      bd[k]       = 1.0;
      k++;
    }
  }

  status = CPXchgbds(env, lp, k, startind, lu, bd);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_rins.c:\n"
                    "function: cpx_solve_rins:\n"
                    "CPXchgbds: %d\n", status);
    return status;
  }*/

  free(startind);
  free(lu);
  free(bd);

  startind = calloc(x_size, sizeof(int));

  for (i = 0 ; i < n ; i++) {
    idx_from_vrtx(&ce->T, ce->TOUR_HEUR.nodes[i], ce->TOUR_HEUR.nodes[(i+1) % n], &j);
    startx[i]   = 1.0;
    startind[i] = j;
  }
  qsort (startind, n, sizeof(int), comp_int);

  status = CPXsetintparam(env, CPX_PARAM_ADVIND, CPX_ON);
  status = CPXaddmipstarts(env, lp, 1, n, beg, startind, startx, NULL, NULL);

  /*status = CPXsetintparam(env, CPX_PARAM_RINSHEUR, 50);
  status = CPXsetintparam(env, CPX_PARAM_POLISHAFTERNODE, 0);*/

  if (ce->pars->cplex_callbacks == TRUE) {
    status = cpx_solve_miliotis(env, lp, ce, cs, ret_x, n * (n - 1) / 2, &ret_solstat);
  } else {
    status = cpx_solve_iterative(env, lp, ce, cs, ret_x, n * (n - 1) / 2, &ret_solstat);
  }

  free(beg);
  free(startx);
  free(startind);

  return status;

}