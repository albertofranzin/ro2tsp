#ifndef BB_BACKUP_H_
#define BB_BACKUP_H_

#include "../../base/constants.h"
#include "../../data/graph.h"
#include "../../data/onetree.h"
#include "../../data/vector.h"
#include "../../data/lvector.h"
#include "../../data/elist.h"
#include "bb_status.h"

// backup di buona parte delle componenti che costituiscono tsp_status
// per motivi di efficenza non è prevista la possibilità di memorizzare l'immagine di G_curr; di volta in volta, prima di applicare una modifica ad un lato del grafo, sarà possibile memorizzare il lato in G_modified_edges
struct bb_backup {

  onetree OT_curr;

  double z_curr;

  vector num_forced_edges;

  vector num_forbidden_edges;

  int current_call;

  int current_level;

  lvector forced_edges;

  elist G_modified_edges;

};

typedef struct bb_backup bb_backup;

// inizializza un backup
// Hp: backup non-inizializzato
void bb_backup_init(bb_backup* backup);

// cancella un backup
// Hp: backup inizializzato o cancellato
void bb_backup_delete(bb_backup* backup);

// inserisci nella lista G_modified_edges il lato di costo cost ed estremi x e y
// Hp: backup inizializzato
void bb_backup_save_edge(bb_backup* backup, int x, int y, double cost);

// effettua un salvataggio in backup di alcune componenti di status; a ciascuna componente è associata un'opzione: se l'opzione è uguale a YES la componente viene salvata, se l'opzione è uguale a NO la componente non viene salvata
// Hp: backup, status inizializzati
// opt_1: OT_curr
// opt_2: z_curr
// opt_3: num_forced_edges
// opt_4: num_forbidden_edges
// opt_5: current_call
// opt_6: current_level
// opt_7: forced_edges
void bb_backup_save(bb_backup* backup, bb_status* status, int opt_1, int opt_2, int opt_3, int opt_4, int opt_5, int opt_6, int opt_7);

// effettua un restore in status di alcune componenti presenti in backup; a ciascuna componente è associata un'opzione: se l'opzione è uguale a YES la componente viene ripristinata, se l'opzione è uguale a NO la componente non viene ripristinata
// Hp: backup, status inizializzati
// opt_1: OT_curr
// opt_2: z_curr
// opt_3: num_forced_edges
// opt_4: num_forbidden_edges
// opt_5: current_call
// opt_6: current_level
// opt_7: forced_edges
// opt_8: modified_edges
// nota: se opt_8 = YES, vengono estratti e ripristinati in status.G_curr i lati contenuti nella lista G_modified_edges interna a backup; al termine della procedura la lista risulta vuota
void bb_backup_restore(bb_backup* backup, bb_status* status, int opt_1, int opt_2, int opt_3, int opt_4, int opt_5, int opt_6, int opt_7, int opt_8);

#endif
