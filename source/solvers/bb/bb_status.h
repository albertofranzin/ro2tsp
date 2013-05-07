#ifndef BB_STATUS_H_
#define BB_STATUS_H_

#include "../../data/graph.h"
#include "../../data/onetree.h"
#include "../../data/vector.h"
#include "../../data/lvector.h"

struct bb_status {

  graph G_curr; // grafo corrente

  onetree OT_curr; // 1-albero corrente

  double z_curr; // costo dell'1-albero corrente

  onetree H_best; // miglior tour trovato
 
  double z_best; // costo del miglior tour trovato

  vector num_forced_edges; // la cella (i-1)-esima contiene il numero di lati forzati incidenti sul nodo i-esimo, i = 1, 2, ..., n

  vector num_forbidden_edges; // la cella (i-1)-esima contiene il numero di lati vietati incidenti sul nodo i-esimo, i = 1, 2, ..., n

  lvector forced_edges; // linked array per mantenere l'informazione sulle catene di lati forzati

  int current_call; // indice della chiamata corrente

  int current_level; // indice della livello corrente

};

typedef struct bb_status bb_status;

// inizializza status
// Hp: status non-inizializzato
void bb_status_init(bb_status* status, int n);

// cancella status
// Hp: status inizializzato o cancellato
void bb_status_delete(bb_status* status);

#endif
