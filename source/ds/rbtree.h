#ifndef __RBTREE_H
#define __RBTREE_H

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "itemset.h"

/* definizione della struttura rbnode
 * contiene:
 * - la chiave (stringa)
 * - il colore del nodo (0 nero, 1 rosso)
 * - i puntatori al nodo genitore, figlio sx, figlio dx
 * */

#define BLACK 0  /* 0 black , 1 red */
#define RED 1 /* 0 black , 1 red */

typedef struct _rbnode {
	void *key;
	short color;
	struct _rbnode *parent;
	struct _rbnode *left;
	struct _rbnode *right;
} rbnode;

static rbnode *nullNode;

/* dichiarazioni */
rbnode *newRBNode(void *);
rbnode *minimum(rbnode *);
rbnode *maximum(rbnode *);
rbnode *successor(rbnode *);
rbnode *predecessor(rbnode *);
void leftRotate(rbnode **, rbnode *);
void rightRotate(rbnode **, rbnode *);
int rbInsert(rbnode **, void *);
int rbInsertItemset(rbnode **, void *);
rbnode *rbSearch(rbnode *, void *);
void transplant(rbnode **, rbnode *, rbnode *);
int rbDelete(rbnode **, void *);
void rbScan(rbnode *, void **);
long rbUnion(rbnode *, rbnode *);
rbnode *createRbTree();
void deleteRbTree(rbnode **);
static void deleteEachRBNode(rbnode *);
void *rbSearchItemset(rbnode *, void *);
rbnode *rbSearchItemsetReturnRBNode(rbnode *, void *);
void printRbTree(rbnode *);

#endif
