#include "list.h"

void list_init(list* L) {

  (*L).size = 0;
  (*L).head.prev = NULL;
  (*L).head.next = &(*L).tail;
  (*L).head.data = 0;
  (*L).tail.prev = &(*L).head;
  (*L).tail.next = NULL;
  (*L).tail.data = 0;
}

void list_delete(list* L) {

  while (!list_is_empty(L)) {
    list_pop_first(L);
  }
}

int list_get_size(list* L) {

  return (*L).size;
}

int list_is_empty(list* L) {

  return ((*L).size == 0);
}

void list_push_last(list* L, int x) {
  list_node* tail = &((*L).tail);
  list_node* last_node = (*L).tail.prev;
  list_node* new_node = (list_node*)malloc(sizeof(list_node));

  (*tail).prev = new_node;
  (*new_node).next = tail;
  (*last_node).next = new_node;
  (*new_node).prev = last_node;
  (*new_node).data = x;
  (*L).size++;
}

int list_pop_first(list* L) {
  if ((*L).size != 0) {
    int x;

    list_node* first_node = (*L).head.next;
    x = (*first_node).data;
    (*(*first_node).next).prev = &(*L).head;
    (*L).head.next = (*first_node).next;
    (*L).size--;
    free(first_node);
    return x;
  }
  else {
    printf("error: list_pop_first\n");
    exit(EXIT_FAILURE);
  }
}

list_node* list_get_first(list* L) {

  return (*L).head.next;
}

list_node* list_get_next(list* L, list_node* node) {
  if ((*node).next != NULL) {
    return (*node).next;
  }
  else {
    printf("error: list_get_next\n");
    exit(EXIT_FAILURE);
  }
}

void list_remove(list* L, list_node* node) {
  if ((*node).prev != NULL && (*node).next != NULL) {
    list_node* prev = (*node).prev;
    list_node* next = (*node).next;

    (*prev).next = next;
    (*next).prev = prev;
    (*L).size--;
    free(node);
  }
  else {
    printf("error: list_remove\n");
    exit(EXIT_FAILURE);
  }
}

void list_sort(list* L) {

  int i, j, min, distance, curr_distance;
  list_node *curr_node, *tmp_node, *min_node, *min_search_node, *prev1, *next1, *prev2, *next2;

  int n = (*L).size;

  curr_node = list_get_first(L);

  for (i = 0; i < n; i++) {

    min_node = curr_node;
    min = (*curr_node).data;

    min_search_node = list_get_next(L, curr_node);

    distance = 0;
    curr_distance = 0;
    for (j = i+1; j < n; j++) {

      curr_distance++;

      if ((*min_search_node).data < min) {
	distance = curr_distance;
	min_node = min_search_node;
	min = (*min_search_node).data;

      }

      min_search_node = list_get_next(L, min_search_node);

    }

    if (distance == 0) { // Do nothing, current_node already contains the minimum
      ; 
    }
    else if (distance == 1) { // min_node is the node immediately after current_node
      prev1 = (*curr_node).prev;
      next2 = (*min_node).next;
      (*prev1).next = min_node;
      (*min_node).prev = prev1;
      (*next2).prev = curr_node;
      (*curr_node).next = next2;

      (*curr_node).prev = min_node;
      (*min_node).next = curr_node;
    }
    else if (distance > 1) {
      prev1 = (*curr_node).prev;
      next1 = (*curr_node).next;
      prev2 = (*min_node).prev;
      next2 = (*min_node).next;

      (*prev1).next = min_node;
      (*min_node).prev = prev1;
      (*min_node).next = next1;
      (*next1).prev = min_node;

      (*prev2).next = curr_node;
      (*curr_node).prev = prev2;
      (*curr_node).next = next2;
      (*next2).prev = curr_node;
    }

    curr_node = list_get_next(L, min_node); // Note that now min_node is in the position where it was curr_node: so we call list_get_next(L, min_node) instead of list_get_next(L, curr_node).

  }
}
