#ifndef ELIST_H_
#define ELIST_H_



#include <stdlib.h>
#include <stdio.h>
#include "edge.h"



typedef struct _elist_node {

  edge data;
  struct _elist_node* prev;
  struct _elist_node* next;

} elist_node;



typedef struct _elist {

  int size;
  elist_node head;
  elist_node tail;

} elist;



void elist_init(elist* EL);



void elist_delete(elist* EL);



void elist_copy(elist* FROM, elist* TO);



void elist_push_last(elist* EL, int x, int y, double cost, int constr);



edge elist_pop_first(elist* EL);



void elist_remove(elist* EL, elist_node* node);



#endif
