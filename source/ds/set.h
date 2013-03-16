#ifndef __SET_H
#define __SET_H

#include <malloc.h>
#include <string.h>
#include "rbtree.h"

/* definizione del tipo di dato "set" 
 *
 * un insieme è costituito da:
 * - un albero red-black, contenente gli elementi dell'insieme
 * - in intero contenente la cardinalità dell'insieme
 * */
typedef struct _set {
	rbnode *root;
	int cardinality;
} set;

/* definizione */
set *newSet();
void setAdd(set *, void *);
//void setAddItemset(set *, void *);
set *setUnion(set *, set *);
void **setScan(set *);
void deleteSet(set *);
void emptySet(set *);
void *searchInSet(set *, void *);
//void *searchItemsetInSet(set *, itemset *);
void printSet(set *);
//void itemSupportMinsupOk(set *, int, int);
//char *buildStreamStringFromSet(set *);
//int *buildStreamIntFromSet(set *);
//set *buildSetFromStreamString(char *, int);
void *getMinimum(set *);
void *getMinimumAndDelete(set *);

#endif
