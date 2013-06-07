#include "list.h"

void list_init(list* L) {

  L->size = 0;
  L->head.prev = NULL;
  L->head.next = &(L->tail);
  L->head.data = 0;
  L->tail.prev = &(L->head);
  L->tail.next = NULL;
  L->tail.data = 0;

}



void list_delete(list* L) {

  while (L->size > 0) {
    list_pop_first(L);
  }

}



void list_push_last(list* L, int x) {

  list_node* tail = &(L->tail);
  list_node* last_node = L->tail.prev;
  list_node* new_node = (list_node*)malloc(sizeof(list_node));

  tail->prev = new_node;
  new_node->next = tail;
  last_node->next = new_node;
  new_node->prev = last_node;
  new_node->data = x;
  L->size++;

}



int list_pop_first(list* L) {

#ifdef DEBUG
  if (L->size <= 0) {
    printf("error: list_pop_first\n");
    exit(1);
  }
#endif

  int x;
  list_node* first_node = L->head.next;

  x = first_node->data;
  (first_node->next)->prev = &(L->head);
  L->head.next = first_node->next;
  L->size--;
  free(first_node);
  return x;

}



void list_remove(list* L, list_node* node) {

#ifdef DEBUG
  if (node->prev == NULL || node->next == NULL) {
    printf("error: list_remove\n");
    exit(1);
  }
#endif

  list_node* prev = node->prev;
  list_node* next = node->next;

  prev->next = next;
  next->prev = prev;
  L->size--;
  free(node);
 
}
