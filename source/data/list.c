#include "../data/node.h"
#include "../data/list.h"


int list_init(list *l) {

    l->size	= -1;

    return 0;

}

int list_setup(list *l) {

	l->size = 0;
	l->head.pred = NULL;
	l->head.next = &(l->tail);
	l->head.data = -1;
	l->tail.pred = &(l->head);
	l->tail.next = NULL;
	l->tail.data = -1;

    return 0;

}

int list_remove(list *l, node *n) {

	node *pred_node = n->pred;
	node *next_node = n->next;

	pred_node->next = next_node;
	next_node->pred = pred_node;
	/*
	n->pred = NULL;
	n->next = NULL;
	*/
	l->size--;

	return 0;

}

int list_insert_last(list *l, node *n) {

	node *last_node = l->tail.pred;

	last_node->next = n;
	n->pred = last_node;
	n->next = &(l->tail);
	l->tail.pred = n;
	l->size++;

	return 0;

}
