#ifndef BB_STATS_H_
#define BB_STATS_H_

// statistiche
struct bb_stats {

  int number_of_levels; // numero di livelli dell'albero di ricorsione
  int number_of_calls; // numero di nodi dell'albero di ricorsione

};

typedef struct bb_stats bb_stats;

// inizializza stats
void bb_stats_init(bb_stats* stats, int n);

// cancella stats
void bb_stats_delete(bb_stats* stats);

#endif
