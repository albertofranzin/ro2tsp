#include "cpx_table.h"


/**
 *  triangular matrix:
 *  (i, j) : (0, 1) - (0, 2) - (1, 2) - (0, 3) - (1, 3) - (2, 3) - (0, 4) - (1, 4) - (2, 4) - (3, 4) ...
 *  idx    :  0        1        2        3        4        5        6        7        8        9     ...
 *
 */
void cpx_table_init(cpx_table* T, graph* G) {

  if (G == NULL) {
    T->n = 0;
    T->entries = NULL;
    return;
  }

  int i, j, idx;
  int n = (*G).n;

  T->n = n;
  T->entries = (cpx_table_entry*)calloc(n * (n - 1) / 2, sizeof(cpx_table_entry));

  for (j = 1; j < n; j++) {
    for (i = 0; i < j; i++) {
      idx = j * (j - 1) / 2 + i;
      (T->entries)[idx].x = i;
      (T->entries)[idx].y = j;
    }
  }

}


void cpx_table_delete(cpx_table* T) {
  free(T->entries);
  T->entries = NULL;
  T->n = 0;
}


void vrtx_from_idx(cpx_table* T, int* x, int* y, int idx) {

#ifdef DEBUG
  if (idx < 0 || idx >= T->n * (T->n - 1) / 2) {
    fprintf(stderr, "Fatal error in: /solvers/cpx/cpx_table.c\n");
    fprintf(stderr, "function: vrtx_from_idx:\n");
    fprintf(stderr, "idx = %d\n", idx);
    exit(EXIT_FAILURE);
  }
#endif

  *x = (T->entries)[idx].x;
  *y = (T->entries)[idx].y;

}


void idx_from_vrtx(cpx_table* T, int x, int y, int* idx) {

#ifdef DEBUG
  if (x < 0 || x >= T->n ||
      y < 0 || y >= T->n ||
      x == y                    ) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_table.c\n"
                     "function: idx_from_vrtx\n"
                     "x = %d, y = %d, n = %d\n", x, y, T->n);
    exit(EXIT_FAILURE);
  }
#endif 

  if (x < y) {
    *idx = y * (y - 1) / 2 + x;
  } else {
    *idx = x * (x - 1) / 2 + y;
  }

}

