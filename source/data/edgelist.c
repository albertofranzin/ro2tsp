#include "edgelist.h"

void edgelist_init(edgelist* EL) {
  (*EL).size = 0;
  (*EL).head.prev = NULL;
  (*EL).head.next = &(*EL).tail;
  (*EL).tail.prev = &(*EL).head;
  (*EL).tail.next = NULL;
}

void edgelist_delete(edgelist* EL) {
  while (!edgelist_is_empty(EL)) {
    edgelist_pop_first(EL);
  }
}

void edgelist_copy(edgelist* FROM, edgelist* TO) {
  edgelist_node* current_node;
  edge e;
  int i;
  int n = edgelist_get_size(FROM);
  edgelist_delete(TO);
  current_node = edgelist_get_first(FROM);
  for (i = 0; i < n; i++) {
    e = edgelist_get_element(FROM, current_node);
    edgelist_push_last(TO, e.x, e.y, e.cost);
    current_node = edgelist_get_next(FROM, current_node);
  }
}

void edgelist_empty(edgelist* EL) {
  while (!edgelist_is_empty(EL)) {
    edgelist_pop_first(EL);
  }
}

inline int edgelist_get_size(edgelist* EL) {
  return (*EL).size;
}

inline int edgelist_is_empty(edgelist* EL) {
  return ((*EL).size == 0);
}

void edgelist_push_last(edgelist* EL, int x, int y, double cost) {
  edgelist_node* tail = &((*EL).tail);
  edgelist_node* last_node = (*EL).tail.prev;
  edgelist_node* new_node = (edgelist_node*)malloc(sizeof(edgelist_node));

  (*tail).prev = new_node;
  (*new_node).next = tail;
  (*last_node).next = new_node;
  (*new_node).prev = last_node;
  (*new_node).data.x = x;
  (*new_node).data.y = y;
  (*new_node).data.cost = cost;
  (*EL).size++;
}
/*
void edgelist_push_last(edgelist* EL, edge e) {
  edgelist_node* tail = &((*EL).tail);
  edgelist_node* last_node = (*EL).tail.prev;
  edgelist_node* new_node = (edgelist_node*)malloc(sizeof(edgelist_node));

  (*tail).prev = new_node;
  (*new_node).next = tail;
  (*last_node).next = new_node;
  (*new_node).prev = last_node;
  (*new_node).data = e;
  (*EL).size++;
}
*/

edge edgelist_pop_first(edgelist* EL) {
  edge e;

  edgelist_node* first_node = (*EL).head.next;
  e = (*first_node).data;
  (*(*first_node).next).prev = &(*EL).head;
  (*EL).head.next = (*first_node).next;
  (*EL).size--;
  free(first_node);
  return e;
}

inline edge edgelist_get_element(edgelist* EL, edgelist_node* node) {
  return (*node).data;
}

inline edgelist_node* edgelist_get_first(edgelist* EL) {

  return (*EL).head.next;
}

inline edgelist_node* edgelist_get_next(edgelist* EL, edgelist_node* node) {

  return (*node).next;
}

void edgelist_remove(edgelist* EL, edgelist_node* node) {
  edgelist_node* prev = (*node).prev;
  edgelist_node* next = (*node).next;

  (*prev).next = next;
  (*next).prev = prev;
  (*EL).size--;
  free(node);
}
