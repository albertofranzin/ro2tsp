#ifndef COMPUTE_OT_H_
#define COMPUTE_OT_H_

#include "graph.h"
#include "compute_mst.h"

/* calcola l'1-albero di G e salva il risultato in T;
 * - si assume G, T grafi inizializzati;
 * - si assume G grafo di n > 2 nodi, non diretto, completo;
 * - il contenuto iniziale di G viene eliminato;
 */
void compute_ot(graph* G, graph* T);

#endif
