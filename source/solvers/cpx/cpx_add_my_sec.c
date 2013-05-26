#include "cpx_add_my_sec.h"

int cpx_add_my_sec(CPXENVptr   env,
                   CPXLPptr    lp,
                   cpx_table  *hash_table,
                   int        *vrtx_mrks,
                   int         n,
                   int         my_mark,
                   parameters *pars)
{

  int i, j, numedgs, numvrtx, numcols, ind, status;

  numcols = CPXgetnumcols(env, lp);
  int rmatind[numcols];
  double rmatval[numcols];

  numvrtx = 0;
  for (i = 0; i < n; i++) {
    if (vrtx_mrks[i] == my_mark) numvrtx++;
  }

  numedgs = 0;
  for (i = 1; i <= n; i++) {
    for (j = i+1; j <= n; j++) {

      if (vrtx_mrks[i-1] == my_mark && vrtx_mrks[j-1] == my_mark) {

        indx_from_vertices(hash_table, i, j, &ind);

        rmatind[numedgs] = ind-1;
        rmatval[numedgs] = 1.0;
        numedgs++;

      }
    
    }
  }

  if (numedgs <= 2) return status;

  int rmatbeg[1];
  rmatbeg[0] = 0;

  double rhs[1];
  rhs[0] = numvrtx-1;

  char sense[1];
  sense[0] = 'L';

#ifdef DEBUG
  if (pars->verbosity >= USEFUL) {
    printf("Adding constraint...\n");
  }
#endif

  status = CPXaddrows(env, lp, 0, 1, numedgs,
                      rhs, sense, rmatbeg, rmatind, rmatval,
                      NULL, NULL);

  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_add_my_sec.c :: ");
    fprintf(stderr, "CPXaddrows : %d\n", status);
    fprintf(stderr, "Error while inserting a new constraint.\n");
    exit(1);
  } else {

#ifdef DEBUG
    if (pars->verbosity >= USEFUL) {
      printf("done \n");
    }
#endif

  }

  return SUCCESS;
}
