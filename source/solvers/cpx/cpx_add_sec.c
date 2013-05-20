#include "cpx_add_sec.h"

int cpx_add_sec(CPXENVptr   env,
                CPXLPptr    lp,
                void       *cbdata,
                int         wherefrom,
                int        *edge_indices,
                int        *edge_marks,
                int         n,
                int         my_mark,
                parameters *pars)
{
  int i, k, status, subtour_length;

#ifdef DEBUG
  if (pars->verbosity >= USEFUL) {
    printf("\nTour has:\n");
  }
#endif

  subtour_length = 0;
  for (i = 0; i < n; i++) {
    if (edge_marks[i] == my_mark) {
      subtour_length++;

#ifdef DEBUG
      if (pars->verbosity >= USEFUL) {
        printf("%d ", i);
      }
#endif

    }
  }

#ifdef DEBUG
  if (pars->verbosity >= USEFUL) {
    printf("\n");
  }
#endif


  int rmatind[subtour_length];
  k = 0;
  for (i = 0; i < n; i++) {
    // aggiungo al sec solo i lati con mark uguale a my_mark
    // (cioè i lati appartenenti al subtour associato a my_mark)
    if (edge_marks[i] == my_mark) {
      rmatind[k] = edge_indices[i]-1;
      // indici dei lati partono da 0 dentro cplex
      k++;
    }
  }


  double rmatval[subtour_length];
  for (i = 0; i < subtour_length; i++) {
    rmatval[i] = 1.0;
  }

  int rmatbeg[1];
  rmatbeg[0] = 0;

  double rhs[1];
  rhs[0] = subtour_length-1;

  char sense[1];
  sense[0] = 'L';

  /*status = CPXaddrows(env, lp, 0, 1, subtour_length,
                      rhs, sense, rmatbeg, rmatind, rmatval,
                      NULL, NULL);*/

#ifdef DEBUG
  if (pars->verbosity >= USEFUL) {
    printf("Adding constraint...\n");
  }
#endif

  status = CPXcutcallbackadd(env, cbdata, wherefrom, subtour_length,
                             rhs[0], sense[0], rmatind, rmatval, 1);
  // Last 1 is purgeable value. See CPXcutcallbackadd documentation.
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_add_sec.c ::\n");
    fprintf(stderr, " CPXcutcallbackadd : %d\n", status);
    fprintf(stderr, "Error while inserting a new constraint.\n");
    exit(1);
  } else {

#ifdef DEBUG
    if (pars->verbosity >= USEFUL) {
      printf("done \n");
    }
#endif

  }

  return status;
}
