#ifndef __ITEMSET_H
#define __ITEMSET_H

#include <string.h>
#include <malloc.h>

#define NUM_OF_PARS 4
#define FILE_CONFIG "apriori.conf"
#define DB_FILE "db.txt"

/*** STRUCTURES ***/

typedef struct _itemset {
	char *name;
	int count;
} itemset;

itemset *newItemset(char *, int);
void deleteItemset(itemset *);

#endif
