#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>
#include <stdio.h>

// nodo di una lista
struct list_node {
  int data; // payload
  struct list_node* prev; // puntatore al nodo precedente
  struct list_node* next; // puntatore al nodo succesivo
};

typedef struct list_node list_node;

// lista
struct list {
  int size; // numero di nodi della lista contenenti dati (tutti i nodi eccetto nodi head e tail)
  list_node head; // nodo di testa
  list_node tail; // nodo di coda
};

typedef struct list list;


// def: una lista L è "cancellata" se la successione di operazioni di inizializzazione e cancellazione su di essa è del tipo: I, D, D, ..., D, I, D, D, ..., D, ..., I, D, D, ..., D
// def: una lista L è "inizializzata" se la successione di operazioni di inizializzazione e cancellazione su di essa è del topo: I, D, D, ..., D, I, D, D, ..., D, ..., I
// def: una lista L è "non-inizializzata" se su di essa non è stata eseguita alcuna operazione di inizializzazione o cancellazione



// inizializza la lista
// Hp: L non-inizializzata o cancellata
void list_init(list* L);

// cancella la lista
// Hp: L inizializzata o cancellata
void list_delete(list* L);

// funzionalmente identica a list_delete
void list_empty(list* L);

// ritorna il numero di elementi presenti nella lista
// Hp: L inizializzata
int list_get_size(list* L);

// ritorna TRUE se la lista è vuota, ritorna FALSE altrimenti
// Hp: L inizializzata
int list_is_empty(list* L);

// inserisci un elemento in fondo alla lista
// Hp: L inizializzata
void list_push_last(list* L, int x);

// estrai l'elemento in cima alla lista
// Hp: L inizializzata
// Hp: L non vuota *
int list_pop_first(list* L);

// ritorna un puntatore al primo nodo (contenente dati) della lista
// nota: se la lista è vuota, il puntatore ritornato punta al nodo tail
// Hp: L inizializzata
list_node* list_get_first(list* L);

// ritorna un puntatore al nodo successivo al nodo il cui puntatore è passato in input
// Hp: L inizializzata
// Hp: il nodo non è il nodo tail *
list_node* list_get_next(list* L, list_node* node);

// rimuovi il nodo il cui puntatore è passato in input
// Hp: L inizializzata
// Hp: il nodo non è il nodo head *
// Hp: il nodo non è il nodo tail *
void list_remove(list* L, list_node* node);

#endif
