#ifndef COMPUTE_OT_H_
#define COMPUTE_OT_H_

#include "../data/graph.h"
#include "../data/onetree.h"
#include "../algos/compute_mst.h"

/* calcola l'1-albero di G e salva il risultato in T;
 * - si assume G, T grafi inizializzati;
 * - si assume G grafo di n > 2 nodi, non diretto, completo;
 * - il contenuto iniziale di G viene eliminato;
 */
void compute_ot(graph* G, onetree* OT);

#endif
