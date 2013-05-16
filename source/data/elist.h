#ifndef ELIST_H_
#define ELIST_H_

#include <stdlib.h>
#include <stdio.h>
#include "edge.h"

// nodo di una lista
struct elist_node {
  edge data; // payload
  struct elist_node* prev; // puntatore al nodo precedente
  struct elist_node* next; // puntatore al nodo succesivo
};

typedef struct elist_node elist_node;

// lista
struct elist {
  int size; // numero di nodi della lista contenenti dati (tutti i nodi eccetto nodi head e tail)
  elist_node head; // nodo di testa
  elist_node tail; // nodo di coda
};

typedef struct elist elist;


// def: una lista L è "cancellata" se la successione di operazioni di inizializzazione e cancellazione su di essa è del tipo: I, D, D, ..., D, I, D, D, ..., D, ..., I, D, D, ..., D
// def: una lista L è "inizializzata" se la successione di operazioni di inizializzazione e cancellazione su di essa è del topo: I, D, D, ..., D, I, D, D, ..., D, ..., I
// def: una lista L è "non-inizializzata" se su di essa non è stata eseguita alcuna operazione di inizializzazione o cancellazione



// inizializza la lista
// Hp: L non-inizializzata o cancellata
void elist_init(elist* EL);

// cancella la lista
// Hp: L inizializzata o cancellata
void elist_delete(elist* EL);

// funzionalmente identica a list_delete
void elist_empty(elist* EL);

// ritorna il numero di elementi presenti nella lista
// Hp: L inizializzata
int elist_get_size(elist* EL);

// ritorna TRUE se la lista è vuota, ritorna FALSE altrimenti
// Hp: L inizializzata
int elist_is_empty(elist* EL);

// inserisci un elemento in fondo alla lista
// Hp: L inizializzata
void elist_push_last(elist* EL, edge e);

// estrai l'elemento in cima alla lista
// Hp: L inizializzata
// Hp: L non vuota *
edge elist_pop_first(elist* EL);

// ritorna un puntatore al primo nodo (contenente dati) della lista
// nota: se la lista è vuota, il puntatore ritornato punta al nodo tail
// Hp: L inizializzata
elist_node* elist_get_first(elist* EL);

// ritorna un puntatore al nodo successivo al nodo il cui puntatore è passato in input
// Hp: L inizializzata
// Hp: il nodo non è il nodo tail *
elist_node* elist_get_next(elist* EL, elist_node* node);

// rimuovi il nodo il cui puntatore è passato in input
// Hp: L inizializzata
// Hp: il nodo non è il nodo head *
// Hp: il nodo non è il nodo tail *
void elist_remove(elist* EL, elist_node* node);

void elist_sort(elist* EL);

#endif
