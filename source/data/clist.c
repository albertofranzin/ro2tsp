#include "clist.h"

void clist_init(clist* CL) {

  (*CL).size = 0;
  (*CL).head.prev = NULL;
  (*CL).head.next = &(*CL).tail;
  (*CL).tail.prev = &(*CL).head;
  (*CL).tail.next = NULL;
}

void clist_delete(clist* CL) {

  while (!clist_is_empty(CL)) {
    clist_pop_first(CL);
  }
}

void clist_copy(clist* FROM, clist* TO) {
  clist_node* current_node;
  cnstr c;
  int i;

  int n = clist_get_size(FROM);
  clist_delete(TO);
  current_node = clist_get_first(FROM);
  for (i = 0; i < n; i++) {
    c = clist_get_element(FROM, current_node);
    clist_push_last(TO, c.x, c.y, c.constr);
    current_node = clist_get_next(FROM, current_node);
  }
}

void clist_empty(clist* CL) {

  while (!clist_is_empty(CL)) {
    clist_pop_first(CL);
  }
}

int clist_get_size(clist* CL) {

  return (*CL).size;
}

int clist_is_empty(clist* CL) {

  return ((*CL).size == 0);
}

void clist_push_last(clist* CL, int x, int y, int constr) {

  clist_node* tail = &((*CL).tail);
  clist_node* last_node = (*CL).tail.prev;
  clist_node* new_node = (clist_node*)malloc(sizeof(clist_node));
  (*tail).prev = new_node;
  (*new_node).next = tail;
  (*last_node).next = new_node;
  (*new_node).prev = last_node;
  (*new_node).data.x = x;
  (*new_node).data.y = y;
  (*new_node).data.constr = constr;
  (*CL).size++;
}

cnstr clist_pop_first(clist* CL) {
  if ((*CL).size != 0) {
    cnstr c;

    clist_node* first_node = (*CL).head.next;
    c = (*first_node).data;
    (*(*first_node).next).prev = &(*CL).head;
    (*CL).head.next = (*first_node).next;
    (*CL).size--;
    free(first_node);
    return c;
  }
  else {
    printf("error: clist_pop_first\n");
    exit(EXIT_FAILURE);
  }
}

cnstr clist_get_element(clist* CL, clist_node* node) {
  if ((*node).prev != NULL && (*node).next != NULL) {
    return (*node).data;
  }
  else {
    printf("error: clist_get_element\n");
    exit(EXIT_FAILURE);
  }
}

clist_node* clist_get_first(clist* CL) {

  return (*CL).head.next;
}

clist_node* clist_get_next(clist* CL, clist_node* node) {
  if ((*node).next != NULL) {
    return (*node).next;
  }
  else {
    exit(EXIT_FAILURE);
  }
}

void clist_remove(clist* CL, clist_node* node) {
  if ((*node).prev != NULL && (*node).next != NULL)  {
    clist_node* prev = (*node).prev;
    clist_node* next = (*node).next;
    (*prev).next = next;
    (*next).prev = prev;
    (*CL).size--;
    free(node);
  }
  else {
    printf("error: clist_remove\n");
    exit(EXIT_FAILURE);
  }
}
