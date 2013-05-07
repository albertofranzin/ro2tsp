#ifndef EGRAPH_H_
#define EGRAPH_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../base/constants.h"
#include "graph.h"
#include "tree.h"
#include "onetree.h"
#include "egraph.h"

// nodo del grafo euclideo
struct egraph_node {
  double x; // coordinata asse x
  double y; // coordinata asse y
  int deg; // grado del nodo
};

typedef struct egraph_node egraph_node;

// lato del grafo euclideo
struct egraph_edge {
  int flag; //  flag=1 se il lato è presente, flag=0 altrimenti
  double cost; // costo del lato
};

typedef struct egraph_edge egraph_edge;

// grafo euclideo
struct egraph {
  int n; // numero di nodi del grafo euclideo
  egraph_node* V; // array di nodi del grafo euclideo
  egraph_edge* E; // array di lati del grafo euclideo
  // coords:
  double max_x;
  double min_x;
  double max_y;
  double min_y;

};

typedef struct egraph egraph;

// nota: anche se la struttura del grafo euclideo prevede la presenza di loop, i controlli nelle funzioni escludono la creazione di loop

// def: un grafo euclideo EG è "cancellato" se la successione di operazioni di inizializzazione e cancellazione su di esso è del tipo: I, D, D, ..., D, I, D, D, ..., D, ..., I, D, D, ..., D
// def: un grafo euclideo EG è "inizializzato" se la successione di operazioni di inizializzazione e cancellazione su di esso è del topo: I, D, D, ..., D, I, D, D, ..., D, ..., I
// def: un grafo euclideo EG è "non-inizializzato" se su di esso non è stata eseguita alcuna operazione di inizializzazione o cancellazione

// inizializza il grafo euclideo
// Hp: EG non-inizializzato oppure cancellato
// Hp: n > 0 *
void egraph_init(egraph* EG, int n);

// cancella il grafo euclideo
// Hp: EG inizializzato oppure cancellato
void egraph_delete(egraph* EG);

// copia il primo grafo euclideo nel secondo grafo euclideo
// Hp: FROM inizializzato
// Hp: TO inizializzato o cancellato
void egraph_copy(egraph* FROM, egraph* TO);

// genera un grafo euclideo completo senza self-loop con nodi distribuiti casualmente in [0, 1)x[0, 1)
// per ogni coppia di nodi {u, v}, u != v, si ha c({u, v})=euclidean_distance(u, v)
// Hp: EG inizializzato
void egraph_random(egraph* EG);

// imposta la coordinata x di un nodo
// imposta X(v)=x
// Hp: EG inizializzato
// Hp: 1 <= v <= n *
void egraph_set_node_x(egraph* EG, int v, double x);

// imposta la coordinata y di un nodo
// imposta Y(v)=y
// Hp: EG inizializzato
// Hp: 1 <= v <= n *
void egraph_set_node_y(egraph* EG, int v, double y);

// ritorna la coordinata x di un nodo
// ritorna X(v)
// Hp: EG inizializzato
// Hp: 1 <= v <= n *
double egraph_get_node_x(egraph* EG, int v);

// ritorna la coordinata y di un nodo
// imposta Y(v)
// Hp: EG inizializzato
// Hp: 1 <= v <= n *
double egraph_get_node_y(egraph* EG, int v);

// inserisce un lato
// inserisce il lato {u, v} e imposta il suo costo c({u, v})=cost
// Hp: EG inizializzato
// Hp: 1 <= u, v <= n *
// HP: u != v *
// Hp: u e v non sono adiacenti *
void egraph_insert_edge(egraph* EG, int u, int v, double cost);

// elimina un lato
// elimina il lato {u, v}
// Hp: EG inizializzato
// Hp: 1 <= u, v <= n *
// Hp: u != v *
// Hp: u e v sono adiacenti *
void egraph_remove_edge(egraph* EG, int u, int v);

// imposta il costo di un lato
// imposta c({u, v})=cost
// Hp: EG inizializzato
// Hp: 1 <= u, v <= n *
// Hp: u != v *
// Hp: u e v sono adiacenti *
void egraph_set_edge_cost(egraph* EG, int u, int v, double cost);

// ritorna il costo di un lato
// ritorna c({u, v})
// Hp: EG inizializzato
// Hp: 1 <= u, v <= n *
// Hp: u != v *
// Hp: u e v sono adiacenti *
double egraph_get_edge_cost(egraph* EG, int u, int v);

// ritorna il grado di un nodo
// Hp: EG inizializzato
// Hp: 1 <= v <= n *
int egraph_get_node_deg(egraph* EG, int v);

// ritorna TRUE se c'è un lato tra i due nodi, ritorna FALSE altrimenti
// Hp: EG inizializzato
// Hp: u != v
// Hp: 1 <= u, v <= n *
int egraph_adjacent_nodes(egraph* EG, int u, int v);

// ritorna il costo del grafo euclideo
// Hp: EG inizializzato
double egraph_get_cost(egraph* EG);

// disegna (gnuplot) il grafo euclideo
// se EG1 != NULL e EG2 = NULL, disegna il primo grafo euclideo con nodi grigi, lati grigi, etichette sui nodi nere
// se EG1 = NULL e EG2 != NULL disegna il secondo grafo euclideo con nodi rossi, lati rossi, etichette sui nodi nere
// se EG1 != NULL e EG2 != NULL il secondo grafo euclideo è disegnato sopra il primo
// Hp: EG1 inizializzato
// Hp: EG2 inizializzato
void egraph_plot(egraph* EG1, egraph* EG2);

// memorizza un grafo come grafo euclideo
// nota: l'informazione sulle coordinate dei punti non è presente in G quindi dev'essere caricata a parte in EG oppure già presente in EG
// Hp: G inizializzato
// Hp: EG inizializzato
void graph_to_egraph(graph* G, egraph* EG);

// memorizza un grafo euclideo come grafo
// Hp: EG inizializzato
// Hp: G inizializzato
void egraph_to_graph(egraph* EG, graph* G);

// memorizza un albero come grafo euclideo
// Hp: T inizializzato
// Hp: EG inizializzato
void tree_to_egraph(tree* T, egraph* EG);

// memorizza un 1-albero come grafo euclideo
// Hp: OT inizializzato
// Hp: EG inizializzato
void onetree_to_egraph(onetree* OT, egraph* EG);

/*
 * print egraph as (diagonal) matrix of costs
 */
void egraph_print(egraph *EG);

#endif
