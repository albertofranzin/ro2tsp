#ifndef TSP_STATS_H_
#define TSP_STATS_H_

// statistiche
struct tsp_stats {

  int number_of_levels; // numero di livelli dell'albero di ricorsione
  int number_of_calls; // numero di nodi dell'albero di ricorsione

};

typedef struct tsp_stats tsp_stats;

// inizializza stats
void tsp_stats_init(tsp_stats* stats, int n);

// cancella stats
void tsp_stats_delete(tsp_stats* stats);

#endif
