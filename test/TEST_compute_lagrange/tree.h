#ifndef TREE_H_
#define TREE_H_

#include <stdlib.h>
#include <stdio.h>

// nodo dell'albero
struct tree_node {
  int pred; // indice del nodo predecessore; pred=0 sse il nodo non ha predecessore
  int deg; // grado del nodo
  double cost; // costo del lato tra il nodo e il suo predecessore
};

typedef struct tree_node tree_node;

// albero
struct tree {
  int n; // numero nodi dell'albero
  int root; // radice dell'albero
  tree_node* V; // array di nodi dell'albero
};

typedef struct tree tree;

// nota: tree è un albero di n nodi v = 1, 2, ..., n, radicato nel nodo 1 <= root <= n; ciascun nodo eccetto il nodo radice ha un predecessore ben definito dall'ordinamento indotto dalla radicatura dell'albero;
// l'informazione relativa ai lati dell'albero viene memorizzata associando a ciascun nodo il suo predecessore e il costo del lato tra il nodo e il suo predecessore

// def: un albero T è "cancellato" se la successione di operazioni di inizializzazione e cancellazione su di esso è del tipo: I, D, D, ..., D, I, D, D, ..., D, ..., I, D, D, ..., D
// def: un albero T è "inizializzato" se la successione di operazioni di inizializzazione e cancellazione su di esso è del topo: I, D, D, ..., D, I, D, D, ..., D, ..., I
// def: un albero T è "non-inizializzato" se su di esso non è stata eseguita alcuna operazione di inizializzazione o cancellazione


// inizializza l'albero con radice r
// Hp: T non-inizializzato o cancellato
// Hp: n > 0 *
void tree_init(tree* T, int n, int r);

// cancella l'albero
// Hp: T inizializzato o cancellato
void tree_delete(tree* T);

// copia il primo albero nel secondo albero
// Hp: FROM inizializzato
// Hp: TO inizializzato o cancellato 
void tree_copy(tree* FROM, tree* TO);

// ritorna il nodo radice
// Hp: T inizializzato
int tree_get_root(tree* T);

// ritorna il predecessore di un nodo
// Hp: T inizializzato
// Hp: 1 <= v <= n *
// Hp: v != root *
int tree_get_pred(tree* T, int v);

// inserisce un lato
// inserisce il lato {u, v}, ponendo il predecessore di v uguale a u, e il costo del lato uguale a cost
// Hp: T inizializzato
// Hp: 1 <= u, v <= n *
// Hp: u != v *
// Hp: u e v non sono adiacenti *
// Hp: v non ha predecessori *
// Hp: v != root *
void tree_insert_edge(tree* T, int u, int v, double cost);

// elimina un lato
// elimina il lato {u, v}
// Hp: T inizializzato
// Hp: 1 <= u, v <= n *
// Hp: u != v *
// Hp: u e v sono adiacenti *
void tree_remove_edge(tree* T, int u, int v);

// imposta il costo di un lato
// Hp: T inizializzato
// Hp: 1 <= u, v <= n *
// Hp: u != v *
// Hp: u e v sono adiacenti *
void tree_set_edge_cost(tree* T, int u, int v, double cost);

// ritorna il costo di un lato
// Hp: T inizializzato
// Hp: 1 <= u, v <= n *
// Hp: u != v *
// Hp: u e v sono adiacenti *
double tree_get_edge_cost(tree* T, int u, int v);

// ritorna il grado di un nodo
// Hp: H inizializzato
// Hp: 1 <= v <= n *
int tree_get_node_deg(tree* T, int v);

// ritorna TRUE se c'è un lato tra i due nodi, ritorna FALSE altrimenti
// Hp: T inizializzato
// Hp: 1 <= u, v <= n *
// Hp: u != v
int tree_adjacent_nodes(tree* T, int u, int v);

// ritorna il costo dell'albero
// Hp: T inizializzato
double tree_get_cost(tree* T);

#endif
