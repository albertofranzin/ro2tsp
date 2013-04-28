#ifndef ELIST_H_
#define ELIST_H_

#include <stdlib.h>
#include <stdio.h>
#include "edge.h"

// nodo di una lista di lati
struct elist_node {
  edge data; // payload
  struct elist_node* prev; // puntatore al nodo precedente
  struct elist_node* next; // puntatore al nodo successico
};

typedef struct elist_node elist_node;

struct elist {
  int size; // numero di nodi della lista contenenti dati (tutti i nodi eccetto nodi head e tail)
  elist_node head; // nodo di testa
  elist_node tail; // nodo di coda
};

typedef struct elist elist;

// def: una lista di lati EL è "cancellata" se la successione di operazioni di inizializzazione e cancellazione su di essa è del tipo: I, D, D, ..., D, I, D, D, ..., D, ..., I, D, D, ..., D
// def: una lista di lati EL è "inizializzata" se la successione di operazioni di inizializzazione e cancellazione su di essa è del topo: I, D, D, ..., D, I, D, D, ..., D, ..., I
// def: una lista di lati EL è "non-inizializzata" se su di essa non è stata eseguita alcuna operazione di inizializzazione o cancellazione

// inizializza la lista
// Hp: EL non-inizializzata o cancellata
void elist_init(elist* EL);

// cancella la lista
// Hp: EL inizializzata o cancellata
void elist_delete(elist* EL);

// funzionalmente identica a edgelist_delete
void elist_empty(elist* EL);

// copia la lista FROM nella lista TO
void elist_copy(elist* FROM, elist* TO);

// ritorna il numero di elementi presenti nella lista
// Hp: EL inizializzata
int elist_get_size(elist* EL);

// ritorna TRUE se la lista è vuota, ritorna FALSE altrimenti
// Hp: EL inizializzata
int elist_is_empty(elist* EL);

// inserisci un elemento in fondo alla lista
// Hp: EL inizializzata
void elist_push_last(elist* EL, int x, int y, double cost);

// estrai l'elemento in cima alla lista
// Hp: EL inizializzata
// Hp: EL non vuota *
edge elist_pop_first(elist* EL);

// ritorna l'elemento presente nel nodo il cui puntatore è passato in input
// Hp: EL inizializzata
// Hp: il nodo non è il nodo head *
// Hp: il nodo non è il nodo tail *
edge elist_get_element(elist* EL, elist_node* node);

// ritorna un puntatore al primo nodo (contenente dati) della lista
// nota: se la lista è vuota, il puntatore ritornato punta al nodo tail
// Hp: EL inizializzata 
elist_node* elist_get_first(elist* EL);

// ritorna un puntatore al nodo successivo al nodo il cui puntatore è passato in input
// Hp: EL inizializzata
// Hp: il nodo non è il nodo tail *
elist_node* elist_get_next(elist* EL, elist_node* node);

// rimuovi il nodo il cui puntatore è passato in input
// Hp: EL inizializzata
// Hp: il nodo non è il nodo head *
// Hp: il nodo non è il nodo tail *
void elist_remove(elist* EL, elist_node* node);

#endif
