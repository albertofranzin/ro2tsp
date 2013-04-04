#ifndef COMPUTE_PATH_H_
#define COMPUTE_PATH_H_

#include "graph.h"

/* calcola un percorso dal nodo from al nodo to nel grafo G;
 * - si assume G, P grafi inizializzati;
 * - si assume G una foresta;
 * - se non esiste un percorso allora ritorna 0;
 * - se esiste un percorso allora ritorna 1 e salva il percorso nel grafo P;
 * - il conenuto iniziale di P viene eliminato;
 * - la funzione va chiamata con flag = 0;
 */
int compute_path(graph* G, graph* P, int from, int to, int flag);

#endif
