#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>
#include <stdio.h>

#include "../data/node.h"


typedef struct _list {

	int 	size;
	node	head;
	node	tail;

} list;


int list_init(list *l);

int list_delete(list *l);

int list_setup(list *l);

int list_remove(list *l, node *n);

int list_insert_last(list *l, node *n);


#endif /* LIST_H_ */
