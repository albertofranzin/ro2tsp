#include "vector.h"

void vector_init(vector* V, int n) {

  (*V).n = n;
  (*V).array = (int*)calloc(n, sizeof(int));

}

void vector_delete(vector* V) {

  free((*V).array);
  (*V).array = NULL;
  (*V).n = 0;
}

void vector_copy(vector* FROM, vector* TO) {
  int i;

  int n = (*FROM).n;
  vector_delete(TO);
  vector_init(TO, n);
  for (i = 0; i < n; i++) {
    (*TO).array[i] = (*FROM).array[i];
  }
}

int vector_get_element(vector* V, int i) {
  if (i >= 1 && i <= (*V).n) {
    return (*V).array[i-1];
  }
  else {
    printf("error: vector_get_element\n");
    exit(EXIT_FAILURE);
  }
}

void vector_set_element(vector* V, int i, int x) {
  if (i >= 1 && i <= (*V).n) {
    (*V).array[i-1] = x;
  }
  else {
    printf("error: vector_set_element\n");
    exit(EXIT_FAILURE);
  }
}
