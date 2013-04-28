#ifndef TSP_BACKUP_H_
#define TSP_BACKUP_H_

#include "constants.h"
#include "graph.h"
#include "onetree.h"
#include "vector.h"
#include "lvector.h"
#include "elist.h"
#include "tsp_status.h"

// backup di buona parte delle componenti che costituiscono tsp_status
// per motivi di efficenza non è prevista la possibilità di memorizzare l'immagine di G_curr; di volta in volta, prima di applicare una modifica ad un lato del grafo, sarà possibile memorizzare il lato in G_modified_edges
struct tsp_backup {

  onetree OT_curr;

  double z_curr;

  vector num_forced_edges;

  vector num_forbidden_edges;

  int current_call;

  int current_level;

  lvector forced_edges;

  elist G_modified_edges;

};

typedef struct tsp_backup tsp_backup;

// inizializza un backup
// Hp: backup non-inizializzato
void tsp_backup_init(tsp_backup* backup);

// cancella un backup
// Hp: backup inizializzato o cancellato
void tsp_backup_delete(tsp_backup* backup);

// inserisci nella lista G_modified_edges il lato di costo cost ed estremi x e y
// Hp: backup inizializzato
void tsp_backup_save_edge(tsp_backup* backup, int x, int y, double cost);

// effettua un salvataggio in backup di alcune componenti di status; a ciascuna componente è associata un'opzione: se l'opzione è uguale a YES la componente viene salvata, se l'opzione è uguale a NO la componente non viene salvata
// Hp: backup, status inizializzati
// opt_1: OT_curr
// opt_2: z_curr
// opt_3: num_forced_edges
// opt_4: num_forbidden_edges
// opt_5: current_call
// opt_6: current_level
// opt_7: forced_edges
void tsp_backup_save(tsp_backup* backup, tsp_status* status, int opt_1, int opt_2, int opt_3, int opt_4, int opt_5, int opt_6, int opt_7);

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
void tsp_backup_restore(tsp_backup* backup, tsp_status* status, int opt_1, int opt_2, int opt_3, int opt_4, int opt_5, int opt_6, int opt_7, int opt_8);

#endif
