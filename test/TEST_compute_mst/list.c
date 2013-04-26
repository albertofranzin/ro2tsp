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
