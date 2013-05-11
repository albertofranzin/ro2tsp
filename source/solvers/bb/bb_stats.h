#ifndef BB_STATS_H_
#define BB_STATS_H_

// statistiche
struct bb_stats {

  int number_of_levels; // numero di livelli dell'albero di ricorsione
  int number_of_calls; // numero di nodi dell'albero di ricorsione

#ifdef DEBUG
  // numero di branch potati perché di costo maggiore dell'incument
  int pruned_branches;

  // conta quante volte il branch ha preso direzione 1, 2 o 3
  // (da dire alla Mike Bongiorno)
  // (e 4 e 5)
  int took1, took2, took3, took4, took5;
#endif

};

typedef struct bb_stats bb_stats;

// inizializza stats
void bb_stats_init(bb_stats* stats, int n);

// cancella stats
void bb_stats_delete(bb_stats* stats);

#endif
