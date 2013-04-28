#ifndef LVECTOR_H_
#define LVECTOR_H_

#include <stdlib.h>
#include <stdio.h>

// nodo di linked vector
struct lvector_node {
  int pred; // indice nodo predecessore
  int succ; // indice nodo successore
  int data; // payload
};

typedef struct lvector_node lvector_node;

// linked vector
struct lvector {
  int n; // numero di celle del linked vector
  lvector_node* array;
};

typedef struct lvector lvector;

// un linked vector di dimensione n è una sequenza di n celle con indici {1, 2, ..., n}, ciascuna delle quale contiene un payload (in questo caso un numero intero), l'indice della cella successiva (non necessariamente la cella di indice successivo!) e l'indice della cella precedente; è quindi possibile creare nel vettore un insieme di catene di celle linkate

// inizializza il linked vector
// Hp: LV non-inizializzato o cancellato
// Hp: n > 0 *
void lvector_init(lvector* LV, int n);

// cancella il linked vector
// Hp: LV inizializzato o cancellato
void lvector_delete(lvector* LV);

// copia il linked vector FROM nel linked vector TO
// Hp: FROM inizializzato
// Hp: TO inizializzato o cancellato
void lvector_copy(lvector* FROM, lvector* TO);

// ritorna l'indice della cella predecessore della cella i
// Hp: LV inizializzato
// Hp: 1 <= i <= n *
int lvector_get_pred(lvector* LV, int i);

// ritorna l'indice della cella successore della cella i
// Hp: LV inizializzato
// Hp: 1 <= i <= n *
int lvector_get_succ(lvector* LV, int i);

// poni uguale a p l'indice della cella predecessore della cella i
// Hp: LV inizializzato
// Hp: 1 <= i <= n *
// Hp: i != p
void lvector_set_pred(lvector* LV, int i, int p);

// poni uguale a s l'indice della cella successore della cella i
// Hp: LV inizializzato
// Hp: 1 <= i <= n *
// Hp: i != s
void lvector_set_succ(lvector* LV, int i, int s); 

// inserisci un link tra la cella i e la cella j
// la cella i sarà predecessore della cella j, la cella j sarà successore della cella i
// Hp: LV inizializzato
// Hp: 1 <= i, j <= n *
// Hp: i != j *
// Hp: i non ha successore, j non ha predecessore *
// Hp: i non ha j come predecessore, j non ha i come successore *
void lvector_insert_link(lvector* LV, int i, int j);

// rimuovi il link tra la cella i e la cella j
// Hp: LV inizializzato
// Hp: 1 <= i, j <= n *
// Hp: i != j *
// Hp: i ha j come successore e j ha i come predecessore, oppure i ha j come predecessore e j ha i come successore *
void lvector_remove_link(lvector* LV, int i, int j);

// ritorna l'elemento (intero) presente nella cella i-esima del linked vector
// Hp: LV inzializzato
// Hp: 1 <= i <= n *
int lvector_get_element(lvector* LV, int i);

// inserisci l'elemento (intero) x nella cella i-esima del linked vector
// Hp: LV inizializzato
// Hp: 1 <= i <= n *
void lvector_set_element(lvector* LV, int i, int x);

#endif
