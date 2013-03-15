#include "set.h"

/* implementazione */

/* set *newSet()
 * senza parametri
 *
 * crea un nuovo insieme
 * 
 */
set *newSet(){
	set *s;
	s = malloc(sizeof(set));
	s->root = createRbTree();
	s->cardinality = 0;
	return s;
}

/* void setAdd
 * parametri:
 * - *s puntatore all'insieme
 * - *el stringa da inserire nell'insieme
 *
 * inserisce una stringa nell'insieme, tenendo traccia della cardinalità dell'insieme
 * (la stringa se è già presente nell'insieme non viene effettivamente inserita)
 * 
 */
void setAdd(set *s, void *el) {
	s->cardinality += rbInsert((rbnode **)&(s->root), el);
}

void setAddItemset(set *s, void *el) {

	int r = rbInsertItemset((rbnode **)&(s->root), el);

	if(r == 0) {
		itemset *i = (itemset *)rbSearchItemset(s->root, el);
		deleteItemset((itemset *)el);
		i->count += 1;
	} else {
		s->cardinality += r;
	}
}

/* void setUnion
 * parametri:
 * - i due insieme da unire
 *
 * effettua l'unione insiemistica dei due insiemi s1 e s2 ricevuti come parametri
 * non è distruttiva, ovvero crea un terzo insieme a partire dai due precedenti,
 * lasciando intatti questi ultimi
 * 
 */
set *setUnion(set *s1, set *s2) {
	set *ns;
	if ((s1 == NULL || s1->root == NULL) && s2 != NULL)
		ns = s2;
	else {
		ns = newSet();
		ns->root = s1->root;
		ns->cardinality += rbUnion(ns->root, s2->root);
	}
	return ns;
}

/* char setScan()
 * parametri:
 * - l'insieme di cui effettuare la scansione
 *
 * crea e restituisce una lista (che risulterà ordinata) contenente tutti gli elementi dell'insieme
 *
 */
void **setScan(set *s) {
	void **list;
	list = malloc((s->cardinality)*sizeof(void *));
	rbScan(s->root, list);
	return list;
}

/* void deleteSet()
 * parametri:
 * - l'insieme da eliminare
 *
 * elimina un insieme
 *
 */
void deleteSet(set *s) {
	deleteRbTree(&(s->root));
	free(s);
}

/* void empySet()
 * parametri:
 * - l'insieme da svuotare
 *
 * svuota un insieme, senza eliminarlo
 *
 */
void emptySet(set *s) {
	deleteRbTree(&(s->root));
	s->cardinality = 0;
}

void *searchInSet(set *s, void *i) {
	if(s->cardinality == 0) return NULL;
	return (void *)rbSearch(s->root, i);
}

void *searchItemsetInSet(set *s, itemset *i) {
	if(s->cardinality == 0) return NULL;
	return (void *)rbSearchItemset(s->root, i);
}

void printSet(set *s) {
	printRbTree(s->root);
}

void itemSupportMinsupOk(set *s, int minsup, int dbLength) {
	itemset **scan = (itemset **)setScan(s);
	int i=0;
	
	int numberofel=0;
	for (; i<s->cardinality; i++) {
		if (scan[i]->count <= minsup*dbLength/100) {
			rbDelete(&(s->root), scan[i]);
			numberofel++;
		}
	}
	s->cardinality -= numberofel;
	free(scan);
}

char *buildStreamStringFromSet(set *s) {

	if (s==NULL)
		return;
	
	if (s->cardinality==0)
		return;

	itemset **scan =(itemset **)setScan(s);

	int k=strlen(scan[0]->name);
	int i=0;
	/* +1 per il carattere di fine stringa */
	char *stream = malloc(sizeof(char)*((s->cardinality)*k+1));
	memset(stream, 0, sizeof(char)*((s->cardinality)*k+1));
	char *p = stream;
	
	for (; i<s->cardinality; i++) {
		strncpy(p, scan[i]->name, k);
		p+=k;
	}
	free(scan);
	return stream;
}

int *buildStreamIntFromSet(set *s) {

	if (s==NULL)
		return;
	
	if (s->cardinality==0)
		return;

	itemset **scan =(itemset **)setScan(s);

	int i=0;
	int *stream = malloc(sizeof(int)*(s->cardinality));
	
	for (; i<s->cardinality; i++) {
		stream[i] = scan[i]->count;
	}

	free(scan);
	return stream;
}

set *buildSetFromStreamString(char *stream, int k) {
	set *s = newSet();

	int i=0;
	int lenStream = strlen(stream);
	
	/* +1 per il carattere di fine stringa */
	char *app = malloc(sizeof(char)*(k+1));
	memset(app, 0, sizeof(char)*(k+1));
	char *itStr = stream;
	while (i<lenStream) {
		strncpy(app, itStr, k);
		itStr+=k;
		i+=k;
		itemset *n = newItemset(app, 0);
		setAddItemset(s, n);
	}
	free(app);
	return s;
}

void *getMinimum(set *s) {
	return (minimum(s->root))->key;
}

void *getMinimumAndDelete(set *s) {
	void *key = (minimum(s->root))->key;
	int deleted = rbDelete((rbnode **)&(s->root), key);
	return key;
}
