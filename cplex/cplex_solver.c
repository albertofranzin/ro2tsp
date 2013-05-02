#include "cplex_solver.h"

void cplex_table_init(cplex_table* CPX_TAB, int size) {

  if (size < 1) {
    printf("error: table_init\n");
  }

  (*CPX_TAB).size = size;
  (*CPX_TAB).n = 0;
  (*CPX_TAB).entries = (int**)malloc(sizeof(int*) * size);

  int i;
  for (i = 0; i < size; i++) {
    (*CPX_TAB).entries[i]  = (int*)calloc(3, sizeof(int));
  }
}

void cplex_table_delete(cplex_table* CPX_TAB) {
  int i;

  for (i = 0; i < (*CPX_TAB).size; i++) {
    free((*CPX_TAB).entries[i]);
  }
  free((*CPX_TAB).entries);
  (*CPX_TAB).size = 0;
  (*CPX_TAB).n = 0;
}

void cplex_table_populate(cplex_table* CPX_TAB, graph* G) {

  if ((*CPX_TAB).size != ((*G).n * ( (*G).n - 1))  /  2) {
    printf("error: table_populate\n");
    exit(EXIT_FAILURE);
  }

  int i, j, pos;
  int n = (*G).n;

  (*CPX_TAB).n = n;

  for (i = 1; i <= n; i++) {
    for (j = i+1; j <= n; j++) {
      // assumendo che i nodi del grafo siano indicizzati 0, 1, 2, ..., n-1 si ha:
      // pos = n * (i + 1)     -    ((i * (i + 1)) / 2) - n    +    j - i - 1
      // nel nostro caso i nodi del grafo sono indicizzati 1, 2, ..., n; inoltre vogliamo che pos parta da 1 e non da 0
      // pos = 1     +     n * i     -     ((i * (i - 1)) / 2) - n     +     j - i - 1
      pos = n * i     -     ((i * (i - 1)) / 2) - n     +     j - i; 
      (*CPX_TAB).entries[pos-1][0] = i;
      (*CPX_TAB).entries[pos-1][1] = j;
      (*CPX_TAB).entries[pos-1][2] = pos;
    }
  }
}

void pos_from_vertices(cplex_table* CPX_TAB, int* x, int* y, int* pos) {

  if (*x < 1 || *x > (*CPX_TAB).n || *y < 1 || *y > (*CPX_TAB).n || *x == *y) {
    printf("error: pos_from_vertices\n");
    exit(EXIT_FAILURE);
  }
  if (*x < *y) {
    *pos = (*CPX_TAB).n * *x     -     ((*x * (*x - 1)) / 2) - (*CPX_TAB).n     +      *y - *x;
  }
  else {
    *pos = (*CPX_TAB).n * *y     -     ((*y * (*y - 1)) / 2) - (*CPX_TAB).n     +      *x - *y;
  }
}

void vertices_from_pos(cplex_table* CPX_TAB, int* x, int* y, int* pos) {
  
  if (*pos < 1 || *pos > (*CPX_TAB).size) {
    printf("error: vertices_from_pos\n");
    exit(EXIT_FAILURE);
  }

  *x = (*CPX_TAB).entries[*pos-1][0];
  *y = (*CPX_TAB).entries[*pos-1][1];

} 
