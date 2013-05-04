#ifndef COMPUTE_MST_H_
#define COMPUTE_MST_H_

#include <stdlib.h>
#include "../data/graph.h"
#include "../data/list.h"
#include "../data/tree.h"

/* calcola un albero ricoprente di costo minimo con radice root per il grafo G, e salva il riultato in T;
 * - si assume G, T grafi inizializzati;
 * - si assume G un grafo non diretto semplice (eventualmente non connesso);
 * - si assume 1 <= root <= G.n;
 * - il contenuto iniziale di T viene eliminato;
 * - se G non è connesso, P conterrà oltre ad un albero ricoprente di costo minimo per il sottografo connesso di G a cui appartiene root, anche i nodi appartenenti agli altri sottografi connessi di G;
 */
void compute_mst(graph* G, tree* T, int root);

#endif
