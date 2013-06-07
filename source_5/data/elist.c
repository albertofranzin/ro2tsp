#include "elist.h"



void elist_init(elist* EL) {

  EL->size = 0;
  EL->head.prev = NULL;
  EL->head.next = &(EL->tail);
  EL->tail.prev = &(EL->head);
  EL->tail.next = NULL;

}



void elist_delete(elist* EL) {

  while (EL->size > 0) {
    elist_pop_first(EL);
  }

}



void elist_copy(elist* FROM, elist* TO) {

  elist_node* curr_node;
  edge e;
  int i;
  int n = FROM->size;

  elist_delete(TO);
  curr_node = FROM->head.next;

  for (i = 0; i < n; i++) {
    e = curr_node->data;
    elist_push_last(TO, e.x, e.y, e.cost, e.constr);
    curr_node = curr_node->next;
  }

}



void elist_push_last(elist* EL, int x, int y, double cost, int constr) {

  elist_node* tail = &(EL->tail);
  elist_node* last_node = EL->tail.prev;
  elist_node* new_node = (elist_node*)malloc(sizeof(elist_node));

  tail->prev = new_node;
  new_node->next = tail;
  last_node->next = new_node;
  new_node->prev = last_node;
  new_node->data.x = x;
  new_node->data.y = y;
  new_node->data.cost = cost;
  new_node->data.constr = constr;
  EL->size++;

}



edge elist_pop_first(elist* EL) {

#ifdef DEBUG
  if (EL->size <= 0) {
    printf("error: elist_pop_first\n");
    exit(1);
  }
#endif

  edge e;
  elist_node* first_node = EL->head.next;

  e = first_node->data;
  (first_node->next)->prev = &(EL->head);
  EL->head.next = first_node->next;
  EL->size--;
  free(first_node);
  return e;

}



void elist_remove(elist* EL, elist_node* node) {

#ifdef DEBUG
  if (node->prev == NULL || node->next == NULL) {
    printf("error: elist_remove\n");
    exit(1);
  }
#endif

  elist_node* prev = node->prev;
  elist_node* next = node->next;

  prev->next = next;
  next->prev = prev;
  EL->size--;
  free(node);

}
