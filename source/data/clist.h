#ifndef CLIST_H_
#define CLIST_H_

#include <stdlib.h>
#include <stdio.h>
#include "cnstr.h"

// nodo di una lista di lati
struct clist_node {
  cnstr data; // payload
  struct clist_node* prev; // puntatore al nodo precedente
  struct clist_node* next; // puntatore al nodo successico
};

typedef struct clist_node clist_node;

struct clist {
  int size; // numero di nodi della lista contenenti dati (tutti i nodi eccetto nodi head e tail)
  clist_node head; // nodo di testa
  clist_node tail; // nodo di coda
};

typedef struct clist clist;

// def: una lista di lati EL è "cancellata" se la successione di operazioni di inizializzazione e cancellazione su di essa è del tipo: I, D, D, ..., D, I, D, D, ..., D, ..., I, D, D, ..., D
// def: una lista di lati EL è "inizializzata" se la successione di operazioni di inizializzazione e cancellazione su di essa è del topo: I, D, D, ..., D, I, D, D, ..., D, ..., I
// def: una lista di lati EL è "non-inizializzata" se su di essa non è stata eseguita alcuna operazione di inizializzazione o cancellazione

// inizializza la lista
// Hp: EL non-inizializzata o cancellata
void clist_init(clist* CL);

// cancella la lista
// Hp: EL inizializzata o cancellata
void clist_delete(clist* CL);

// funzionalmente identica a edgelist_delete
void clist_empty(clist* CL);

// copia la lista FROM nella lista TO
void clist_copy(clist* FROM, clist* TO);

// ritorna il numero di elementi presenti nella lista
// Hp: EL inizializzata
int clist_get_size(clist* CL);

// ritorna TRUE se la lista è vuota, ritorna FALSE altrimenti
// Hp: EL inizializzata
int clist_is_empty(clist* CL);

// inserisci un elemento in fondo alla lista
// Hp: EL inizializzata
void clist_push_last(clist* CL, int x, int y, int constr);

// estrai l'elemento in cima alla lista
// Hp: EL inizializzata
// Hp: EL non vuota *
cnstr clist_pop_first(clist* CL);

// ritorna l'elemento presente nel nodo il cui puntatore è passato in input
// Hp: EL inizializzata
// Hp: il nodo non è il nodo head *
// Hp: il nodo non è il nodo tail *
cnstr clist_get_element(clist* CL, clist_node* node);

// ritorna un puntatore al primo nodo (contenente dati) della lista
// nota: se la lista è vuota, il puntatore ritornato punta al nodo tail
// Hp: EL inizializzata 
clist_node* clist_get_first(clist* CL);

// ritorna un puntatore al nodo successivo al nodo il cui puntatore è passato in input
// Hp: EL inizializzata
// Hp: il nodo non è il nodo tail *
clist_node* clist_get_next(clist* CL, clist_node* node);

// rimuovi il nodo il cui puntatore è passato in input
// Hp: EL inizializzata
// Hp: il nodo non è il nodo head *
// Hp: il nodo non è il nodo tail *
void clist_remove(clist* CL, clist_node* node);

#endif
