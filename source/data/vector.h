#ifndef VECTOR_H_
#define VECTOR_H_

#include <stdlib.h>
#include <stdio.h>

// vettore di interi
struct vector {
  int n; // numero di celle del vettore
  int* array;
};

typedef struct vector vector;

// inizializza il vettore
// Hp: V non-inizializzato o cancellato
// Hp: n > 0 *
void vector_init(vector* V, int n);

// cancella il vettore
// Hp: V inizializzato o cancellato
void vector_delete(vector* A);

// copia il vettore FROM nel vettore TO
// Hp: FROM inizializzato
// Hp: TO inizializzato o cancellato
void vector_copy(vector* FROM, vector* TO);

// ritorna l'elemento (intero) presente nella cella i-esima del vettore
// Hp: V inzializzato
// Hp: 1 <= i <= n *
int vector_get_element(vector* V, int i);

// inserisci l'elemento (intero) x nella cella i-esima del vettore
// Hp: V inizializzato
// Hp: 1 <= i <= n *
void vector_set_element(vector* V, int i, int x);

#endif
