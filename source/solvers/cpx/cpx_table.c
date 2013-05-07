#include "cpx_table.h"

/*
 * cpx_table_init
 *
 * create a table starting from a graph
 */
void cpx_table_init(cpx_table* CPX_TAB, int size) {
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

/*
 * cpx_table_delete
 *
 * delete a table
 */
void cpx_table_delete(cpx_table* CPX_TAB) {
  int i;

  for (i = 0; i < (*CPX_TAB).size; i++) {
    free((*CPX_TAB).entries[i]);
  }

  free((*CPX_TAB).entries);
  (*CPX_TAB).size = 0;
  (*CPX_TAB).n = 0;
}

/*
 * cpx_table_populate
 *
 * hash (v1, v2)->(pos)
 *
 * cpx_table * : hash table
 * int *         : pointer to index of vertex 1 (not modified)
 * int *         : pointer to index of vertex 2 (not modified)
 * int *         : pointer to index of position (to be modified)
 *
 * le posizioni dei lati (i, j) sono assegnate come segue (sia n=5)
 * lato: (1,2) (1,3) (1,4) (1,5) (2,3) (2,4) (2,5) (3,4) (3,5) ...
 * pos :   1     2     3     4     5     6     7     8     9   ...
 */
void cpx_table_populate(cpx_table* CPX_TAB, graph* G) {

  if ((*CPX_TAB).size != ((*G).n * ( (*G).n - 1))  /  2) {
    printf("error: table_populate\n");
    exit(EXIT_FAILURE);
  }
  // assert(CPX_TAB->size == G->n * (G->n - 1)) / 2);

  int i, j, pos;
  int n = (*G).n;

  (*CPX_TAB).n = n;

  // assumendo che i nodi del grafo siano 0, 1, 2, ..., n-1 si ha:
  // pos = n * (i + 1) - ((i * (i + 1)) / 2) - n + j - i - 1
  // nel nostro caso i nodi del grafo sono indicizzati 1, 2, ..., n
  // inoltre vogliamo che pos parta da 1 e non da 0
  // pos = 1 + n * i ((i * (i - 1)) / 2) - n + j - i - 1
  for (i = 1; i <= n; i++) {
    for (j = i+1; j <= n; j++) {
      pos = n * i - ((i * (i - 1)) / 2) - n + j - i;
      (*CPX_TAB).entries[pos-1][0] = i;
      (*CPX_TAB).entries[pos-1][1] = j;
      (*CPX_TAB).entries[pos-1][2] = pos;
    }

  }

} // end cpx_table_populate

/*
 * vertices_from_pos
 *
 * hash (pos)->(v1, v2)
 *
 * cpx_table * : hash table
 * int *         : pointer to index of vertex 1 (to be modified)
 * int *         : pointer to index of vertex 2 (to be modified)
 * int           : index of position (not modified)
 */
void vertices_from_pos(cpx_table* CPX_TAB, int* x, int* y, int pos) {
  if (pos < 1 || pos > CPX_TAB->size) {
    fprintf(stderr, "error: vertices_from_pos\n");
    fprintf(stderr, "pos = %d, size = %d\n", pos, CPX_TAB->size);
    exit(EXIT_FAILURE);
  }

  *x = CPX_TAB->entries[pos-1][0];
  *y = CPX_TAB->entries[pos-1][1];
}

/*
 * pos_from_vertices
 *
 * hash (pos)<-(v1, v2)
 *
 * cpx_table * : hash table
 * int           : index of vertex 1 (not modified)
 * int           : index of vertex 2 (not modified)
 * int *         : pointer to index of position (to be modified)
 */
void pos_from_vertices(cpx_table* CPX_TAB, int x, int y, int* pos) {
  //printf("%d %d\n", x, y);
  if (x < 1 || x > CPX_TAB->n ||
      y < 1 || y > CPX_TAB->n ||
      x == y                    ) {
    fprintf(stderr, "error: pos_from_vertices\n");
    printf("x = %d, y = %d, n = %d\n", x, y, CPX_TAB->n);
    exit(EXIT_FAILURE);
  }
  if (x < y) {
    *pos = CPX_TAB->n * x - ((x * (x - 1)) / 2) - CPX_TAB->n + y - x;
  } else {
    *pos = CPX_TAB->n * y - ((y * (y - 1)) / 2) - CPX_TAB->n + x - y;
  }
}
