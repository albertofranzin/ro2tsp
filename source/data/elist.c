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

int elist_get_size(elist* EL) {

  return (*EL).size;
}

int elist_is_empty(elist* EL) {

  return ((*EL).size == 0);
}

void elist_push_last(elist* EL, edge e) {
  elist_node* tail = &((*EL).tail);
  elist_node* last_node = (*EL).tail.prev;
  elist_node* new_node = (elist_node*)malloc(sizeof(elist_node));

  (*tail).prev = new_node;
  (*new_node).next = tail;
  (*last_node).next = new_node;
  (*new_node).prev = last_node;
  (*new_node).data = e;
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

elist_node* elist_get_first(elist* EL) {

  return (*EL).head.next;
}

elist_node* elist_get_next(elist* EL, elist_node* node) {
  if ((*node).next != NULL) {
    return (*node).next;
  }
  else {
    printf("error: list_get_next\n");
    exit(EXIT_FAILURE);
  }
}

void elist_remove(elist* EL, elist_node* node) {
  if ((*node).prev != NULL && (*node).next != NULL) {
    elist_node* prev = (*node).prev;
    elist_node* next = (*node).next;

    (*prev).next = next;
    (*next).prev = prev;
    (*EL).size--;
    free(node);
  }
  else {
    printf("error: list_remove\n");
    exit(EXIT_FAILURE);
  }
}

// Sort the list: edges with highest costs at the top.
void elist_sort(elist* EL) {

  int i, j, max, distance, curr_distance;
  elist_node *curr_node, *tmp_node, *max_node, *max_search_node, *prev1, *next1, *prev2, *next2;

  int n = (*EL).size;

  curr_node = elist_get_first(EL);

  for (i = 0; i < n; i++) {

    max_node = curr_node;
    max = (*curr_node).data.cost;

    max_search_node = elist_get_next(EL, curr_node);

    distance = 0;
    curr_distance = 0;
    for (j = i+1; j < n; j++) {

      curr_distance++;

      if ((*max_search_node).data.cost > max) {
	distance = curr_distance;
	max_node = max_search_node;
	max = (*max_search_node).data.cost;

      }

      max_search_node = elist_get_next(EL, max_search_node);

    }

    if (distance == 0) { // Do nothing, current_node already contains the minimum
      ; 
    }
    else if (distance == 1) { // min_node is the node immediately after current_node
      prev1 = (*curr_node).prev;
      next2 = (*max_node).next;
      (*prev1).next = max_node;
      (*max_node).prev = prev1;
      (*next2).prev = curr_node;
      (*curr_node).next = next2;

      (*curr_node).prev = max_node;
      (*max_node).next = curr_node;
    }
    else if (distance > 1) {
      prev1 = (*curr_node).prev;
      next1 = (*curr_node).next;
      prev2 = (*max_node).prev;
      next2 = (*max_node).next;

      (*prev1).next = max_node;
      (*max_node).prev = prev1;
      (*max_node).next = next1;
      (*next1).prev = max_node;

      (*prev2).next = curr_node;
      (*curr_node).prev = prev2;
      (*curr_node).next = next2;
      (*next2).prev = curr_node;
    }

    curr_node = elist_get_next(EL, max_node); // Note that now min_node is in the position where it was curr_node: so we call list_get_next(EL, min_node) instead of list_get_next(EL, curr_node).

  }
}
