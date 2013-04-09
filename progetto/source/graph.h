#ifndef GRAPH_H_
#define GRAPH_H_

#if defined __GNUC__ && !defined __GNUC_STDC_INLINE__ && !defined __GNUC_GNU_INLINE__
#define __GNUC_GNU_INLINE__ 1
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include "node.h"
#include "edge.h"


/* grafo non diretto pesato;
 * - sono permessi self-loop;
 * - i nodi 1, 2, ..., n sono memorizzati in un array .V nelle posizioni 0, 1, ..., n-1;
 * - i lati (1,1), (2,1), (2,2), (3,1), (3,2), (3,3), ..., (n,n) sono memorizzati in un array .E nelle posizioni 0, 1, 2, 3, 4, 5, ..., [n * (n+1) / 2]-1;
 */
struct graph {
  int n; // numero nodi del grafo;
  node* V; // nodi del grafo;
  edge* E; // lati del grafo;
};

typedef struct graph graph;



/* inizializza un grafo di n nodi e nessun lato;
 * - si assume G grafo non inizializzato o grafo eliminato;
 * - si assume n > 0;
 */
void initGraph(graph* G, int n);



/* elimina un grafo;
 * - si assume G grafo inizializzato o eliminato;
 */
void deleteGraph(graph* G);



/* genera un grafo euclideo completo con G.n nodi di coordinate casuali in [0, 1);
 * - si assume G grafo inizializzato;
 */
void randomGraph(graph* G);



/* disegna il/i grafo/i con gnuplot;
 * - se opt1 = "default" allora se G1 != NULL disegna G1 con nodi e lati neri, e se G2 != NULL disegna G2 con nodi e lati verdi; si assume opt2 = NULL;
 * - se opt1 = "mstree" allora disegna G1 con nodi e lati grigi, G2 (l'albero ricoprente) con nodi e lati rossi ed evidenzia di nero la radice r; si assume G1 != NULL, G2 albero ricoprente di G1 con radice r, opt2 = "r";
 * - se opt1 = "onetree" allora disegna G1 con nodi e lati grigi, G2 (l'1-albero) con nodi e lati rossi, ed evidenzia di blu il ciclo dell'1-albero, di nero il nodo 1; si assume G1 != NULL, G2 1-albero di G1, opt2 = NULL;
 */
void plotGraph(graph* G1, graph* G2, char* opt1, char* opt2);



/* copia il grafo FROM nel grafo TO;
 * - il contenuto iniziale di TO viene eliminato;
 * - si assume FROM e TO grafi inizializzati;
 */
void copyGraph(graph* FROM, graph* TO);



/* inserisci un nuovo lato di G con estremi u e v; 
 * - si assume G grafo inizializzato;
 * - si assume 1 <= v, u <= G.n;
 * - se il lato esiste già, la funzione non fa nulla
 */
void insert_edge(graph* G, int u, int v);



/* rimuovi un lato di G con estremi u e v;
 * - si assume G grafo inizializzato;
 * - si assume 1 <= u, v <= G.n; 
 * - se il lato non esiste, la funzione non fa nulla;
 */
void remove_edge(graph* G, int u, int v);

/*
 * inline methods should yield better performance, since the calls
 * are replaced with corresponding method code
 */

/* poni al valore x la coordinata x del nodo v di G;
 * - si assume G grafo inizializzato;
 * - si assume 1 <= v <= G.n;
 */
static inline void set_node_x(graph* G, int v, double x) {
  (*G).V[v-1].x = x;
}



/* poni al valore y la coordinata y del nodo v di G;
 * - si assume G grafo inizializzato;
 * - si assume 1 <= v <= G.n;
 */
static inline void set_node_y(graph* G, int v, double y) {
  (*G).V[v-1].y = y;
}



/* ritorna la coordinata x del nodo v di G;
 * - si assume G grafo inizializzato;
 */
static inline double get_node_x(graph* G, int v) {
  return (*G).V[v-1].x;
}



/* ritorna la coordinata y del nodo v di G;
 * - si assume G grafo inizializzato;
 * - si assume 1 <= v <= G.n;
 */
static inline double get_node_y(graph* G, int v) {
  return (*G).V[v-1].y;
}



/* ritorna il grafo (numero di lati incidenti) del nodo v di G;
 * - si assume G grafo inizializzato;
 * - si assume 1 <= v <= G.n;
 */
static inline int get_node_deg(graph* G, int v) {
  return (*G).V[v-1].deg;
}



/* poni al valore cost il costo del lato di estremi u e v di G;
 * - si assume G grafo inizializzato;
 * - si assume 1 <= v, u <= G.n;
 */
static inline void set_edge_cost(graph* G, int u, int v, double cost) {
  (u > v) ? ( (*G).E[ u*(u-1)/2 + v-1 ].cost = cost ) : ( (*G).E[ v*(v-1)/2 + u-1 ].cost = cost );
}



/* ritorna il costo del lato di estremi u e v di G;
 * - si assume G grafo inizializzato;
 * - si assume 1 <= v, u <= G.n;
 */
static inline double get_edge_cost(graph* G, int u, int v) {
  return (u > v) ? (*G).E[ u*(u-1)/2 + v-1 ].cost : (*G).E[ v*(v-1)/2 + u-1 ].cost;
}



/* ritorna 1 se esiste un lato di G con estremi u e v, ritorna 0 altrimenti;
 * - si assume G grafo inizializzato;
 * - si assume 1 <= v, u <= G.n;
 */
static inline int adjacent(graph* G, int u, int v) {
  return (u > v) ? (*G).E[ u*(u-1)/2 + v-1 ].flag : (*G).E[ v*(v-1)/2 + u-1 ].flag;
}



/* ritorna il costo complessivo di G;
 * - si assume G grafo inizializzato;
 */
double get_graph_cost(graph* G);

/*
 * print graph as matrix of costs
 */
void print_graph(graph *G);

#endif
