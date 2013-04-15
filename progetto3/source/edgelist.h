#ifndef EDGELIST_H_
#define EDGELIST_H_

#include <stdlib.h>
#include "edge.h"

struct edgelist_node {
  edge data;
  struct edgelist_node* prev;
  struct edgelist_node* next;
};

typedef struct edgelist_node edgelist_node;

struct edgelist {
  int size;
  edgelist_node head;
  edgelist_node tail;
};

typedef struct edgelist edgelist;

void edgelist_init(edgelist* EL);
void edgelist_delete(edgelist* EL);
void edgelist_copy(edgelist* FROM, edgelist* TO);
void edgelist_empty(edgelist* EL); // identica a edgelist_delete, ma con nome più chiaro per certi contesti
int edgelist_get_size(edgelist* EL);
int edgelist_is_empty(edgelist* EL);
void edgelist_push_last(edgelist* EL, int x, int y, double cost);
//void edgelist_push_last(edgelist* EL, edge e);
edge edgelist_pop_first(edgelist* EL);
edge edgelist_get_element(edgelist* EL, edgelist_node* node);
edgelist_node* edgelist_get_first(edgelist* EL);
edgelist_node* edgelist_get_next(edgelist* EL, edgelist_node* node);
void edgeedgelist_remove(edgelist* EL, edgelist_node* node);

#endif
