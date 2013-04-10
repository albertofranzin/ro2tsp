#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "graph.h"
#include "onetree.h"
#include "solve_tsp.h"

graph WORK_GRAPH; // grafo "di lavoro": inizialmente contiene una copia esatta del grafo originale; via via che l'esecuzione procede vengono modificati i costi dei lati per forzarli o vietarli.

int tour_found_flag; // = 1 se è stato trovato un ciclo hamiltoniano, = 0 se non è stato ancora trovato
int calls_counter; // numero di chiamate effettuate al tempo corrente
int level_counter; // livello corrente nell'albero di ricorsione
int max_depth_reached; // massimo livello (profondità) raggiunto nell'albero di ricorsione

void solve_tsp(graph* G, onetree* H, double* incumbent, int call_flag) {
  int i, j, u, v, w;
  double z, cost_wv, cost_wu;

  onetree ONE_TREE;
  int n = (*G).n;

  /* effettua una copia del grafo iniziale G nel grafo di lavoro WORK_GRAPH;
   */
  if (call_flag == 0) {
    graph_init(&WORK_GRAPH, 1);
    graph_copy(G, &WORK_GRAPH);
    onetree_delete(H);
    calls_counter = 1;
    level_counter = 1;
    max_depth_reached = 1;
    tour_found_flag = 0;
    call_flag = 1;
  }
  else {
    call_flag = 2;
  }

  max_depth_reached = (level_counter > max_depth_reached) ? level_counter : max_depth_reached;
  onetree_init(&ONE_TREE, 1);

  /* calcola 1-albero;
   */
  compute_ot(&WORK_GRAPH, &ONE_TREE);

  /* calcola z = costo dell'1-albero;
   */
  onetree_set_edge_cost(&ONE_TREE, 1, onetree_get_first_node(&ONE_TREE), graph_get_edge_cost(G, 1, onetree_get_first_node(&ONE_TREE))); // setto al costo originale il costo del lato {1, x} dell'1-albero (siano {1, x} e {¡, y} i lati incidenti in 1 nell'1-albero)
  onetree_set_edge_cost(&ONE_TREE, 1, onetree_get_second_node(&ONE_TREE), graph_get_edge_cost(G, 1, onetree_get_second_node(&ONE_TREE))); // setto al costo originale il costo del lato {1, y} dell'1-albero
  for (i = 2; i <= n; i++) { // setto al costo originale il costo dei lati dell'1-albero non incidenti in 1
    onetree_set_edge_cost(&ONE_TREE, onetree_get_pred(&ONE_TREE, i), i, graph_get_edge_cost(G, onetree_get_pred(&ONE_TREE, i), i));
  }
  z = onetree_get_cost(&ONE_TREE);

  /* verifica se possibile potare il ramo corrente
   */
  if (z > *incumbent) { // il caso z = *incumbent viene trattato nell'if successivo: questo per evitare che non venga memorizzato in H alcun ciclo nel caso in cui solve_tsp sia chiamato con un upper bound (incumbent iniziale) esattamente uguale al costo del ciclo hamiltoniano ottimo (improbabile ma possibile)
    onetree_delete(&ONE_TREE);
    return;
  }

  /* verifica se è stato trovato un ciclo (una soluzione ottima per il sottoproblema corrispondente al nodo corrente);
   * aggiorna incumbent e soluzione ottima corrente;
   */
  if (is_cycle(&ONE_TREE)) {
    if (z < *incumbent || tour_found_flag == 0) {
      printf("# updated incumbent = %f : calls up to now = %d : current depth = %d\n", z, calls_counter, level_counter);
      *incumbent = z;
      onetree_copy(&ONE_TREE, H);
      tour_found_flag = 1;
    }
    else { // z = *incubent AND è già stato trovato (e memorizzato in H) un candidato per il ciclo hamiltoniano ottimo 
      onetree_delete(&ONE_TREE);
      return;
    }
  }

  /* cerca un nodo w con almeno 3 lati incidenti;
   */
  for (w = 1; w <= n; w++) {
    if (onetree_get_node_deg(&ONE_TREE, w) >= 3)
      break;
  }

  /* (tentativo di) ricerca di due lati candidati mai forzati nè vietati;
   */
  for (v = 1; v <= n; v++) {
   if (v != w && onetree_adjacent_nodes(&ONE_TREE, w, v) && graph_get_edge_cost(&WORK_GRAPH, w, v) > SMALL && graph_get_edge_cost(&WORK_GRAPH, w, v) < BIG)
      break;
  }
  for (u = 1; u <= n; u++) {
    if (u != w && u != v && onetree_adjacent_nodes(&ONE_TREE, w, u) && graph_get_edge_cost(&WORK_GRAPH, w, u) > SMALL && graph_get_edge_cost(&WORK_GRAPH, w, u) < BIG)
      break;
  }

  onetree_delete(&ONE_TREE);

  /* se esistono due lati mai selezionati allora procedi con BRANCHING A 3 VIE;
   */
  if ((v >= 1 && v <= n) && (u >= 1 && u <= n)) {

    // vieta il lato {w, v};
    cost_wv = graph_get_edge_cost(&WORK_GRAPH, w, v);
    graph_set_edge_cost(&WORK_GRAPH, w, v, BIG);
    calls_counter++;
    level_counter++;
    solve_tsp(G, H, incumbent, call_flag);
    level_counter--;
    graph_set_edge_cost(&WORK_GRAPH, w, v, cost_wv);

    // forza il lato {w, v} e vieta il lato {w, u};
    cost_wv = graph_get_edge_cost(&WORK_GRAPH, w, v);
    cost_wu = graph_get_edge_cost(&WORK_GRAPH, w, u);
    graph_set_edge_cost(&WORK_GRAPH, w, v, SMALL);
    graph_set_edge_cost(&WORK_GRAPH, w, u, BIG);
    calls_counter++;
    level_counter++;
    solve_tsp(G, H, incumbent, call_flag);
    level_counter--;
    graph_set_edge_cost(&WORK_GRAPH, w, v, cost_wv);
    graph_set_edge_cost(&WORK_GRAPH, w, u, cost_wu);

    // forza i lati {w, v} e {w, u}, vieta tutti gli altri lati;
    double* previous_cost = (double*)malloc(sizeof(double) * n);
    for (i = 1; i <= n; i++) { // memorizza i costi attuali dei lati per poter fare un roll back al ritorno dalla prossim chiamata ricorsiva;
      if (i != w) 
	previous_cost[i-1] = graph_get_edge_cost(&WORK_GRAPH, w, i);
    }
    graph_set_edge_cost(&WORK_GRAPH, w, v, SMALL);
    graph_set_edge_cost(&WORK_GRAPH, w, u, SMALL);
    for (i = 1; i <= n; i++) {
      if (i != w && i != v && i != u)
	graph_set_edge_cost(&WORK_GRAPH, w, i, BIG);
    }
    calls_counter++;
    level_counter++;
    solve_tsp(G, H, incumbent, call_flag);
    level_counter--;
    for (i = 1; i <= n; i++) { // roll back costi così com'erano prima della chiamata ricorsiva
      if (i != w)
	graph_set_edge_cost(&WORK_GRAPH, w, i, previous_cost[i-1]);
    }
    free(previous_cost);
  } 

  /* esiste un solo lato che non è mai stato nè forzato nè vietato;
   */
  else if (((v >= 1 && v <= n) && (u < 1 || u > n)) || ((v < 1 || v > n) && (u >= 1 && u <= n))) {

    // vieta il lato {w, v};
    cost_wv = graph_get_edge_cost(&WORK_GRAPH, w, v);
    graph_set_edge_cost(&WORK_GRAPH, w, v, BIG);
    calls_counter++;
    level_counter++;
    solve_tsp(G, H, incumbent, call_flag);
    level_counter--;
    graph_set_edge_cost(&WORK_GRAPH, w, v, cost_wv);

    // forza il lato {w, u};
    cost_wv = graph_get_edge_cost(&WORK_GRAPH, w, v);
    graph_set_edge_cost(&WORK_GRAPH, w, v, SMALL);
    calls_counter++;
    level_counter++;
    solve_tsp(G, H, incumbent, call_flag);
    level_counter--;
    graph_set_edge_cost(&WORK_GRAPH, w, v, cost_wv);
  }

  /* tutti i lati sono già stati forzati o vietati;
   */
  else if ((v < 1 || v > n) && (u < 1 || u > n)) {
    return;
  }

  if (call_flag == 1) {
    graph_delete(&WORK_GRAPH);
    printf("# max depth reached = %d\n# number of calls = %d\n", max_depth_reached, calls_counter);
  }
}



/* ritorna 1 se G è un ciclo, 0 altrimenti;
 * - si assume G connesso (altrimenti non è detto che G sia un ciclo anche se la funzione ritorna 1;
 */
int is_cycle(onetree* OT) {
  int i;
  int n = (*OT).n;
 
  for (i = 1; i <= n; i++) {
    if (onetree_get_node_deg(OT, i) != 2)
      return 0;
  }
  return 1;
}
