#ifndef ARRAYLIST_H_
#define ARRAYLIST_H_

#include <stdlib.h>
#include <stdio.h>

#include "../base/global.h"
#include "../base/utils.h"


typedef struct _arraylist {

	int 	capacity;
	int		size;
	int		*next;
	int		*pred;

} arraylist;


int arraylist_init(arraylist *l);

int arraylist_delete(arraylist *l);

int arraylist_empty(arraylist *l);

int arraylist_setup(arraylist *l, int capacity);

int arraylist_copy(arraylist *from, arraylist *to);

int arraylist_insert_next(arraylist *l, int i, int e);

int arraylist_insert_pred(arraylist *l, int i, int e);

int arraylist_remove(arraylist *l, int e);

int arraylist_isin(arraylist *l, int e);

#endif /* ARRAYLIST_H_ */
