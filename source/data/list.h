#ifndef LIST_H_
#define LIST_H_



#include <stdlib.h>
#include <stdio.h>



typedef struct _list_node {

  int data;
  struct _list_node* prev;
  struct _list_node* next;

} list_node;



typedef struct _list {

  int size;
  list_node head;
  list_node tail;

} list;



void list_init(list* L);



void list_delete(list* L);



void list_push_last(list* L, int x);



int list_pop_first(list* L);



void list_remove(list* L, list_node* node);



#endif
