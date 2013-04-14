#ifndef LIST_H_
#define LIST_H_

struct list_node {
  int data;
  struct list_node* prev;
  struct list_node* next;
};

typedef struct list_node list_node;

struct list {
  int size;
  list_node head;
  list_node tail;
};

typedef struct list list;

void list_init(list* L);
void list_delete(list* L);
int list_get_size(list* L);
int list_is_empty(list* L);
void list_push_last(list* L, int x);
int list_pop_first(list* L);
list_node* list_get_first(list* L);
list_node* list_get_next(list* L, list_node* node);
void list_remove(list* L, list_node* node);

#endif
