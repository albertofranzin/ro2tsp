#include "cpx_add_sec.h"

int cpx_add_sec(CPXENVptr env, CPXLPptr lp, int* edge_indexes, int* edge_marks, int n, int my_mark) {
  int i, k, status, subtour_length;

  subtour_length = 0;
  for (i = 0; i < n; i++) {
    if (edge_marks[i] == my_mark) {
      subtour_length++;
    }
  }

  int rmatind[subtour_length];
  k = 0;
  for (i = 0; i < n; i++) {
    if (edge_marks[i] == my_mark) { // aggiungo al sec solo i lati con mark uguale a my_mark (cioè i lati appartenenti al subtour associato a my_mark)
      rmatind[k] = edge_indexes[i]-1; // indici dei lati partono da 0 dentro cplex
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

  status = CPXaddrows(env, lp, 0, 1, subtour_length, rhs, sense, rmatbeg, rmatind, rmatval, NULL, NULL); // NULL: aggiungere nome constraint?
  return status;
}
