#include "rbtree.h"

/* implementazioni */

/* rbnode *createRbTree
 * nessun parametro
 *
 * crea un albero creandone la sua radice, vuota.
 */
rbnode *createRbTree(){
	nullNode = malloc(sizeof(rbnode));
	memset(nullNode, 0, sizeof(rbnode));
	nullNode->key = NULL;
	nullNode->parent = NULL;
	nullNode->left = NULL;
	nullNode->right = NULL;
	nullNode->color = BLACK;
	
	rbnode *root = nullNode;
	return root;
}

/* rbnode *newNode
 * parametri:
 * - la stringa da inserire
 *
 * crea un nuovo nodo, settando il suo campo chiave al parametro passato,
 * il colore a nero e i puntatori a NULL
 *
 */
rbnode *newRBNode(void *data) {
	rbnode *nn = malloc(sizeof(rbnode));
	memset(nn, 0, sizeof(rbnode));
	nn->key = data;
	nn->color = BLACK;
	nn->parent = nullNode;
	nn->left = nullNode;
	nn->right = nullNode;
	return nn;
}

/* rbnode  *minimum
 * parametri:
 * - l'albero in cui cercare il minimo
 *
 * ritorna il nodo contenente il minimo elemento presente in un albero (in ordine lessicografico)
 *
 */
rbnode *minimum(rbnode *root) {
	rbnode *node = root;
	while (node->left->key != NULL)
		node = node->left;
	return node;
}

/* rbnode  *maximum
 * parametri:
 * - l'albero in cui cercare il massimo
 *
 * ritorna il nodo contenente il massimo elemento presente in un albero (in ordine lessicografico)
 *
 */
rbnode *maximum(rbnode *root) {
	rbnode *node;
	node = root;
	while (node->right->key != NULL)
		node = node->right;
	return node;
}

/* rbnode *successor
 * parametri:
 * - un nodo dell'albero
 *
 * ritorna il nodo contenente la stringa successiva (in ordine lessicografico) a quella contenuta nel nodo passato come parametro
 *
 */
rbnode *successor(rbnode *x) {
	if(x->right->key != NULL)
		return minimum(x->right);

	rbnode *y;
	y = x->parent;
	while( y->key != NULL && x == y->right) {
		x = y;
		y = y->parent;
	}
	return y;
}

/* rbnode *predecessor
 * parametri:
 * - un nodo dell'albero
 *
 * ritorna il nodo contenente la stringa precedente (in ordine lessicografico) a quella contenuta nel nodo passato come parametro
 *
 */
rbnode *predecessor(rbnode *x) {
	if(x->left->key != NULL)
		return maximum(x->left);

	rbnode *y;
	y = x->parent;
	while(y->key != NULL && x == y->left) {
		x = y;
		y = y->parent;
	}
	return y;
}

/* inserimento */

/* void leftRotate e void rightRotate
 * scambiano di posto due nodi
 *
 */
void leftRotate(rbnode **root, rbnode *x) {

	rbnode *y = x->right;
	x->right = y->left;
	
	if(y->left->key != NULL)
		y->left->parent = x;
	
	y->parent = x->parent;
	
	if(x->parent->key == NULL) 
		*root = y;
	else {
		if (x == x->parent->left)
			x->parent->left = y;
		else x->parent->right = y;
	}
	y->left = x;
	x->parent = y;
}

void rightRotate(rbnode **root, rbnode *x) {

	rbnode *y = x->left;
	x->left = y->right;
	
	if(y->right->key != NULL) { 
		y->right->parent = x; 
	}
	
	y->parent = x->parent;

	if(x->parent->key == NULL)
		*root = y;
	else {
		if (x == x->parent->right)
			x->parent->right = y;
		else x->parent->left = y;
	}
	y->right = x;
	x->parent = y;

}

/* int rbInsert
 * parametri:
 * - l'albero
 * - la stringa da inserire
 *
 * se non è gia presente, inserisce nell'albero la stringa, effettuando
 * gli aggiustamenti necessari per preservare le proprietà dell'albero
 * restituisce il numero di elementi effettivamente inseriti (0/1)
 *
 */
int rbInsert(rbnode **root, void *key) {

	if(rbSearch(*root,key) != NULL)
		return 0;

	rbnode *node, *z, *y, *x, *tmp, *tmp2;
	z = newRBNode(key);

	y = NULL;
	x = *root;

	while(x != NULL){
		y = x;
		if(strcmp(key, x->key) < 0)
         x = x->left;
		else x = x->right;
	}
	z->parent = y;

	if (y == NULL)
      *root = z;
	else if (strcmp(key, y->key) < 0)
      y->left = z;
	else y->right = z;

	z->color = RED;
	
	/* rb-insert-fixup */
	while(z->parent->color == RED) {
		if(z->parent == z->parent->parent->left) {
			y = z->parent->parent->right;

			if(y!=NULL && y->color == RED){
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			} else if(z == z->parent->right) {
				z = z->parent;
				leftRotate(root,z);
			}
         z->parent->color = BLACK;
         z->parent->parent->color = RED;
         rightRotate(root, z->parent->parent);

		} else {
         y = z->parent->parent->left;
			
         if(y!=NULL && y->color == RED){
            z->parent->color = BLACK;
            y->color = BLACK;
            z->parent->parent->color = RED;
            z = z->parent->parent;
         } else if(z == z->parent->left) {
            z = z->parent;
            rightRotate(root,z);
         }
         z->parent->color = BLACK;
         z->parent->parent->color = RED;
         leftRotate(root, z->parent->parent);
		}
	}

	(*root)->color = BLACK;
	return 1;
}

int rbInsertItemset(rbnode **root, void *key) {

	if(rbSearchItemset(*root,key) != NULL)
		return 0;

	rbnode *node, *z, *y, *x;
	z = newRBNode(key);
	itemset *i = (itemset *)key;
	char *str = i->name;

	y = nullNode;
	x = *root;

	while(x->key != NULL){
		y = x;
		i = (itemset *)x->key;
		if(strcmp(str, i->name) < 0)
         x = x->left;
		else x = x->right;
	}

	z->parent = y;
	if(y->key == NULL) {
		*root = z;
	}
	else if (strcmp(str, (i->name)) < 0)
		y->left = z;
	else
		y->right = z;

	z->color=RED;

	/* rb-insert-fixup */
	while(z->parent->color == RED) {
		if(z->parent == z->parent->parent->left) {
			y = z->parent->parent->right;

			if(y->color == RED){
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			} else {
				if(z == z->parent->right) {
					z = z->parent;
					leftRotate(root,z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				rightRotate(root, z->parent->parent);
			}
			
		} else {
         y = z->parent->parent->left;
			
         if(y->color == RED){
            z->parent->color = BLACK;
            y->color = BLACK;
            z->parent->parent->color = RED;
            z = z->parent->parent;
         } else {
				if(z == z->parent->left) {
					z = z->parent;
					rightRotate(root,z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				leftRotate(root, z->parent->parent);
			}
		}
	}
	
	(*root)->color = BLACK;

	return 1;
}

/* rbnode *rbSearch
 * parametri:
 * - l'albero in cui effettuare la ricerca
 * - la chiave da cercare
 *
 * ritorna il nodo contenente la stringa cercata se presente, NULL altrimenti
 * 
 */
rbnode *rbSearch(rbnode *root, void *key) {

	int r;
	rbnode *x = root;
	while(x->key != NULL) {
		r = strcmp(key, x->key);
		if(r == 0) return x;
		if(r < 0) x = x->left;
		else x = x->right;
	}
	return x;
}

void *rbSearchItemset(rbnode *root, void *key) {
	
	int r;
	rbnode *x = root;
	itemset *i;
	char *kn = ((itemset *)key)->name;
	
 	while(x->key != NULL) {
		i = x->key;
		r = strcmp(kn, i->name);
		if(r == 0) {
			return x->key;
		}
		if(r < 0)
			x = x->left;
		else
			x = x->right;
	}
	return NULL;
}


rbnode *rbSearchItemsetReturnRBNode(rbnode *root, void *key) {
	int r;
	rbnode *x = root;
	itemset *i;
	char *kn = ((itemset *)key)->name;

 	while(x->key != NULL) {
		i = x->key;
		r = strcmp(kn, i->name);
		if(r == 0) {
			return x;
		}
		if(r < 0)
			x = x->left;
		else
			x = x->right;
	}
	return NULL;
}

/* eliminazione */

void transplant(rbnode **root, rbnode *u, rbnode *v) {
	if(u->parent->key == NULL)
		*root = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else u->parent->right = v;

	v->parent = u->parent;
}


/* int rbDelete
 * parametri
 * - l'albero
 * - la stringa da cercare
 *
 * se c'è la stringa cercate, elimina il nodo effettuando le operazioni necessarie a mantenere
 * la proprietà dell'albero. Ritorna il numero di elementi effettivamente eliminati.
 */
int rbDelete(rbnode **root, void *key) {
	rbnode *z;
	z = rbSearchItemsetReturnRBNode(*root,key);

	if(z == NULL) {
		/* printf("not found\n"); */
		return 0;
 	}

	rbnode *y, *x, *w;
	y = z;
	short y_old_color = y->color;
	
	if(z->left->key == NULL) {
		x = z->right;
		transplant(root, z, z->right);
	} else if(z->right->key == NULL) {
		x = z->left;
		transplant(root, z, z->left);
	} else {
		y = minimum(z->right);
		y_old_color = y->color;
		x = y->right;
		if(y->parent == z) {
			x->parent = y;
		} else {
			transplant(root, y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}
      transplant(root, z, y);      
		y->left = z->left;
      y->left->parent = y;
		y->color = z->color;
	}

	if (y_old_color == BLACK) {
		/* rb-delete-fixup */
		while(x != *root && x->color == BLACK) {
			if(x == x->parent->left) {
				w = x->parent->right;
				if(w->color == RED) {
					w->color = BLACK;
					x->parent->color = RED;
					leftRotate(root, x->parent);
					w = x->parent->right;
				}
				
				if(w->left->color == BLACK && w->right->color == BLACK) {
					w->color = RED;
					x = x->parent;
				} else {
					if (w->right->color == BLACK) {
						w->left->color = BLACK;
						w->color = RED;
						rightRotate(root, w);
						w = x->parent->right;
					}
					w->color = x->parent->color;
					x->parent->color = BLACK;
					w->right->color = BLACK;
					leftRotate(root, x->parent);
					x = *root;
				}
				
			} else {
				
				w = x->parent->left;
				if(w->color == RED) {
					w->color = BLACK;
					x->parent->color = RED;
					rightRotate(root, x->parent);
					w = x->parent->left;
				}
				
				if(w->right->color == BLACK && w->left->color == BLACK) {
					w->color = RED;
					x = x->parent;
				} else {
					if(w->left->color == BLACK) {
						w->right->color = BLACK;
						w->color = RED;
						leftRotate(root, w);
						w = x->parent->left;
					}
					w->color = x->parent->color;
					x->parent->color = BLACK;
					w->left->color = BLACK;
					rightRotate(root, x->parent);
					x = *root;
				}
			}
		}
		x->color = 0;
	}

	deleteItemset(z->key);
	free(z);
	return 1;
}

/* void rbScan
 * parametri:
 * - l'albero
 * - una lista in cui inserire gli elementi
 *
 * partendo dal minimo elemento, riempie la lista con tutti gli elementi dell'albero (in ordine)
 * 
 */
void rbScan(rbnode *root, void **listRcvd) {

	if (root==NULL || root->key==NULL)
		return;

	rbnode *node = root;
	int i = 0;
	void **list = listRcvd;
	
	while (node->left->key != NULL)
		node = node->left;
	
	while(node->key != NULL) {
		list[i++] = node->key;
		node = successor(node);
	}
}

/* long rbUnion 
 * parametri:
 * - i due alberi da unire
 *
 * fa l'unione insiemistica di due alberi, inserendo il secondo nel primo
 * ritorna il numero di elementi inseriti nel primo albero
 * 
 */
long rbUnion(rbnode *r1, rbnode *r2) {
	long hm = 0;
	if(r2 == NULL) return 0;
	if(r1 == r2) return 0;

	hm += rbInsert(&r1, r2->key);
	rbUnion(r1, r2->left);
	rbUnion(r1, r2->right);
	return hm;
}

/* void deleteEachRBNode
 * parametri:
 * - l'albero
 *
 * elimina ricorsivamente l'albero
 * 
 */

static void deleteEachRBNode(rbnode *root) {
	if (root->key==NULL)
		return;
	
	deleteEachRBNode(root->left);
	deleteItemset(root->key);
	deleteEachRBNode(root->right);
	memset(root, 0, sizeof(rbnode));
	free(root);
}


/* void deleteRbTree
 * parametri:
 * - radice dell'albero
 * 
 * elimina ricorsivamente l'albero
 * elimina il nodo nullNode
 * 
 */

void deleteRbTree(rbnode **root) {
	deleteEachRBNode(*root);
	*root = NULL;
}


void printRbTree(rbnode *root) {
	rbnode *node = root;

	if (node==NULL || node->key==NULL)
		return;
	
	while (node->left->key != NULL)
		node = node->left;

	while(node->key != NULL) {
		printf("%s - %d\n",((itemset *)node->key)->name,
				((itemset *)node->key)->count);
		node = successor(node);
	}
}
