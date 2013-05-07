#include "elist.h"

void elist_init(elist* EL) {

  (*EL).size = 0;
  (*EL).head.prev = NULL;
  (*EL).head.next = &(*EL).tail;
  (*EL).tail.prev = &(*EL).head;
  (*EL).tail.next = NULL;
}

void elist_delete(elist* EL) {

  while (!elist_is_empty(EL)) {
    elist_pop_first(EL);
  }
}

void elist_copy(elist* FROM, elist* TO) {
  elist_node* current_node;
  edge e;
  int i;

  int n = elist_get_size(FROM);
  elist_delete(TO);
  current_node = elist_get_first(FROM);
  for (i = 0; i < n; i++) {
    e = elist_get_element(FROM, current_node);
    elist_push_last(TO, e.x, e.y, e.cost);
    current_node = elist_get_next(FROM, current_node);
  }
}

void elist_empty(elist* EL) {

  while (!elist_is_empty(EL)) {
    elist_pop_first(EL);
  }
}

int elist_get_size(elist* EL) {

  return (*EL).size;
}

int elist_is_empty(elist* EL) {

  return ((*EL).size == 0);
}

void elist_push_last(elist* EL, int x, int y, double cost) {

  elist_node* tail = &((*EL).tail);
  elist_node* last_node = (*EL).tail.prev;
  elist_node* new_node = (elist_node*)malloc(sizeof(elist_node));
  (*tail).prev = new_node;
  (*new_node).next = tail;
  (*last_node).next = new_node;
  (*new_node).prev = last_node;
  (*new_node).data.x = x;
  (*new_node).data.y = y;
  (*new_node).data.cost = cost;
  (*EL).size++;
}

edge elist_pop_first(elist* EL) {
  if ((*EL).size != 0) {
    edge e;

    elist_node* first_node = (*EL).head.next;
    e = (*first_node).data;
    (*(*first_node).next).prev = &(*EL).head;
    (*EL).head.next = (*first_node).next;
    (*EL).size--;
    free(first_node);
    return e;
  }
  else {
    printf("error: elist_pop_first\n");
    exit(EXIT_FAILURE);
  }
}

edge elist_get_element(elist* EL, elist_node* node) {
  if ((*node).prev != NULL && (*node).next != NULL) {
    return (*node).data;
  }
  else {
    printf("error: elist_get_element\n");
    exit(EXIT_FAILURE);
  }
}

elist_node* elist_get_first(elist* EL) {

  return (*EL).head.next;
}

elist_node* elist_get_next(elist* EL, elist_node* node) {
  if ((*node).next != NULL) {
    return (*node).next;
  }
  else {
    exit(EXIT_FAILURE);
  }
}

void elist_remove(elist* EL, elist_node* node) {
  if ((*node).prev != NULL && (*node).next != NULL)  {
    elist_node* prev = (*node).prev;
    elist_node* next = (*node).next;
    (*prev).next = next;
    (*next).prev = prev;
    (*EL).size--;
    free(node);
  }
  else {
    printf("error: elist_remove\n");
    exit(EXIT_FAILURE);
  }
}
