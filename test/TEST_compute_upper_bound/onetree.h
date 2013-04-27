#ifndef ONETREE_H_
#define ONETREE_H_

#include <stdlib.h>
#include <stdio.h>
#include "tree.h"

// 1-albero
struct onetree {
  int n; // numero nodi dell'1-albero
  tree_node* V; // array di nodi dell'1-albero
};

typedef struct onetree onetree;

// nota: onetree è un 1-albero di n nodi v = 1, 2, ..., n; 
// un 1-albero sui nodi {1, 2, ..., n} si può vedere come un albero ricoprente sui nodi {1, 2, ..., n} radicato in 1, più un lato aggiuntivo incidente in 1
// si è deciso di organizzare l'1-albero secondo una struttura praticamente identica a quella utilizzata per l'albero, consentendo però al nodo radice 1 di avere un predecessore: l'informazione sul lato aggiuntivo, sia esso {1, a}, è memorizzata ponendo il predecessore di 1 uguale ad a

// def: un 1-albero OT è "cancellato" se la successione di operazioni di inizializzazione e cancellazione su di esso è del tipo: I, D, D, ..., D, I, D, D, ..., D, ..., I, D, D, ..., D
// def: un 1-albero OT è "inizializzato" se la successione di operazioni di inizializzazione e cancellazione su di esso è del topo: I, D, D, ..., D, I, D, D, ..., D, ..., I
// def: un 1-albero OT è "non-inizializzato" se su di esso non è stata eseguita alcuna operazione di inizializzazione o cancellazione


// inizializza l'1-albero
// Hp: OT non-inizializzato o cancellato
// Hp: n > 1 *
void onetree_init(onetree* OT, int n);

// cancella l'1-albero
// Hp: OT inizializzato o cancellato
void onetree_delete(onetree* OT);

// copia il primo 1-albero nel secondo 1-albero
// Hp: FROM inizializzato
// Hp: TO inizializzato o cancellato 
void onetree_copy(onetree* FROM, onetree* TO);

// ritorna il predecessore di un nodo
// Hp: OT inizializzato
// Hp: 1 <= v <= n *
// Hp: v != root *
int onetree_get_pred(onetree* OT, int v);

// inserisce un lato
// inserisce il lato {u, v}, ponendo il predecessore di v uguale a u, e il costo del lato uguale a cost
// Hp: OT inizializzato
// Hp: 1 <= u, v <= n *
// Hp: u != v *
// Hp: u e v non sono adiacenti *
// Hp: v non ha predecessori *
void onetree_insert_edge(onetree* OT, int u, int v, double cost);

// elimina un lato
// elimina il lato {u, v}
// Hp: OT inizializzato
// Hp: 1 <= u, v <= n *
// Hp: u != v *
// Hp: u e v sono adiacenti *
void onetree_remove_edge(onetree* OT, int u, int v);

// imposta il costo di un lato
// Hp: OT inizializzato
// Hp: 1 <= u, v <= n *
// Hp: u != v *
// Hp: u e v sono adiacenti *
void onetree_set_edge_cost(onetree* OT, int u, int v, double cost);

// ritorna il costo di un lato
// Hp: OT inizializzato
// Hp: 1 <= u, v <= n *
// Hp: u != v *
// Hp: u e v sono adiacenti *
double onetree_get_edge_cost(onetree* OT, int u, int v);

// ritorna il grado di un nodo
// Hp: OT inizializzato
// Hp: 1 <= v <= n *
int onetree_get_node_deg(onetree* OT, int v);

// ritorna TRUE se c'è un lato tra i due nodi, ritorna FALSE altrimenti
// Hp: OT inizializzato
// Hp: 1 <= u, v <= n *
// Hp: u != v
int onetree_adjacent_nodes(onetree* OT, int u, int v);

// ritorna il costo dell'1-albero
// Hp: OT inizializzato
double onetree_get_cost(onetree* OT);

// ritorna TRUE se l'1-albero è un ciclo, ritorna FALSE altrimenti
// Hp: OT inizializzato
// nota: in particolare l'oggetto memorizzato in OT deve effettivamente corrispondere ad un grafo connesso; sotto tale ipotesi, il grafo è un ciclo sse tutti i nodi hanno grado 2
int onetree_is_cycle(onetree* OT);

#endif
