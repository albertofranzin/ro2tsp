#include "lvector.h"

void lvector_init(lvector* LV, int n) {
  if (n >= 1) {
    (*LV).n = n;
    (*LV).array = (lvector_node*)calloc(n, sizeof(lvector_node));
  }
  else {
    printf("error: lvector_init\n");
    exit(EXIT_FAILURE);
  }
}

void lvector_delete(lvector* LV) {

  (*LV).n = 0;
  free((*LV).array);
  (*LV).array = NULL;
}

void lvector_copy(lvector* FROM, lvector* TO) {
  int i;

  int n = (*FROM).n;
  lvector_delete(TO);
  lvector_init(TO, n);
  for (i = 0; i < n; i++) {
    (*TO).array[i] = (*FROM).array[i];
  }
}


int lvector_get_pred(lvector* LV, int i) {
  if (i >= 1 && i <= (*LV).n) {
    return (*LV).array[i-1].pred;
  }
  else {
    printf("error: lvector_get_pred\n");
    exit(EXIT_FAILURE);
  }
}

int lvector_get_succ(lvector* LV, int i) {
  if (i >= 1 && i <= (*LV).n) {
    return (*LV).array[i-1].succ;
  }
  else {
    printf("error: lvector_get_succ\n");
    exit(EXIT_FAILURE);
  }
}

void lvector_set_pred(lvector* LV, int i, int p) {
  if (i >= 1 && i <= (*LV).n && i != p) {
    (*LV).array[i-1].pred = p;
  }
  else {
    printf("error:lvector_set_pred\n");
    exit(EXIT_FAILURE);
  }
}

void lvector_set_succ(lvector* LV, int i, int s) {
  if (i >= 1 && i <= (*LV).n && i != s) {
    (*LV).array[i-1].succ = s;
  }
  else {
    printf("error: lvector_set_succ\n");
    exit(EXIT_FAILURE);
  }
}

void lvector_insert_link(lvector* LV, int i, int j) {
  if (i >= 1 && j >= 1 && i <= (*LV).n && j <= (*LV).n && i != j && (*LV).array[i-1].succ == 0 && (*LV).array[j-1].pred == 0 && (*LV).array[i-1].pred != j && (*LV).array[j-1].succ != i) {
    (*LV).array[i-1].succ = j;
    (*LV).array[j-1].pred = i;
  }
  else {
    printf("error: lvector_insert_link\n");
    exit(EXIT_FAILURE);
  }
}

void lvector_remove_link(lvector* LV, int i, int j) {
  if (i >= 1 && j >= 1 && i <= (*LV).n && j <= (*LV).n && i != j && (((*LV).array[i-1].succ == j && (*LV).array[j-1].pred == i) || ((*LV).array[i-1].pred == j && (*LV).array[j-1].succ == i))) {
    if ((*LV).array[i-1].succ == j && (*LV).array[j-1].pred == i) {
      (*LV).array[i-1].succ = 0;
      (*LV).array[j-1].pred = 0;
    }
    else if ((*LV).array[i-1].pred == j && (*LV).array[j-1].succ == i) {
      (*LV).array[i-1].pred = 0;
      (*LV).array[j-1].succ = 0;
    }
  }
  else {
    printf("error: lvector_remove_link\n");
    exit(EXIT_FAILURE);
  }
}

int lvector_get_element(lvector* LV, int i) {
  if (i >= 1 && i <= (*LV).n) {
    return (*LV).array[i-1].data;
  }
  else {
    printf("error: lvector_get_element\n");
    exit(EXIT_FAILURE);
  }
}

void lvector_set_element(lvector* LV, int i, int x) {
  if (i >= 1 && i <= (*LV).n) {
    (*LV).array[i-1].data = x;
  }
  else {
    printf("error: lvector_set_element\n");
    exit(EXIT_FAILURE);
  }
}
