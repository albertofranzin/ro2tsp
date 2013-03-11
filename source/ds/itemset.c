#include "itemset.h"

itemset *newItemset(char *n, int c) {
	itemset *it = malloc(sizeof(itemset));
	memset(it, 0, sizeof(itemset));
	it->name = malloc(sizeof(char)*strlen(n));
	strcpy(it->name, n);
	it->count=c;

	return it;
}

void deleteItemset(itemset *i) {
	if (i==NULL)
		return;
	
	free(i->name);
	free(i);
}
