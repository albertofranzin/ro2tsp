#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "solve_tsp.h"
#include "compute_upper_bound.h"

graph INITIAL_GRAPH;
int nprune;
int ncalls;
int nblock;

void solve_tsp(graph* G, onetree* H, double* incumbent, int flag, double ub, int ub_max_iter, double alpha, int alpha_max_iter, int max_iter) {
  int i, j, u, v, w;
  double z;
  onetree ONE_TREE;
  int n = (*G).n;
  double* previous_cost;;
  double cost_wv, cost_wu;

  int special_case;
  int num_selected;
  int num_forbidden;
  int num_forced;

  onetree_init(&ONE_TREE, 1);

  /* effettua una copia del grafo iniziale G, passato in ingresso alla prima chiamata della funzione;
   */
  if (flag == 0) {
    graph_init(&INITIAL_GRAPH, 1);
    graph_copy(G, &INITIAL_GRAPH);
    flag = 1;
    nprune = 0;
    ncalls = 0;
    nblock = 0;
  }
  else {
    flag = 2;
  }

  ncalls++;

  /* calcola 1-albero;
   */
  if (flag == 1) {
    compute_lagrange(G, &ONE_TREE, ub, ub_max_iter, alpha, alpha_max_iter, max_iter);
  }
  else if (flag == 2) {
    compute_lagrange(G, &ONE_TREE, ub, ub_max_iter, alpha, alpha_max_iter, max_iter);
    //compute_ot(G, &ONE_TREE);
  }

  /* calcola z = costo dell'1-albero;
   */
  for (i = 1; i <= n; i++) {
    for (j = i+1; j <= n; j++)
      onetree_set_edge_cost(&ONE_TREE, i, j, graph_get_edge_cost(&INITIAL_GRAPH, i, j));
  }
  z = onetree_get_cost(&ONE_TREE);
  //printf("valore z = %f\n", z);

  /* verifica se possibile potare il ramo corrente
   */

  
  if (z >= *incumbent) {
    nprune++;
    onetree_delete(&ONE_TREE);
    return;
  }
  
  if (ncalls >= nblock * 1000) {
    printf("numero prune - calls - fraction: %d - %d - %f\n", nprune, ncalls, ((double)nprune / (double)ncalls));
    nblock++;
  }

  /* verifica se è stato trovato un ciclo (una soluzione ottima per il sottoproblema corrispondente al nodo corrente);
   * aggiorna incumbent e soluzione ottima corrente;
   */
  if (is_cycle(&ONE_TREE)) {
    printf("updating incumbent : from %f to %f\n", *incumbent, z);
    if (z < *incumbent) {
      *incumbent = z;
      onetree_copy(&ONE_TREE, H);
    }
    onetree_delete(&ONE_TREE);
    return;
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
   if (v != w && onetree_adjacent_nodes(&ONE_TREE, w, v) && graph_get_edge_cost(G, w, v) > SMALL && graph_get_edge_cost(G, w, v) < BIG)
      break;
  }
  for (u = 1; u <= n; u++) {
    if (u != w && u != v && onetree_adjacent_nodes(&ONE_TREE, w, u) && graph_get_edge_cost(G, w, u) > SMALL && graph_get_edge_cost(G, w, u) < BIG)
      break;
  }

  onetree_delete(&ONE_TREE);

  /* se esistono due lati mai selezionati allora procedi con BRANCHING A 3 VIE;
   */
  if ((v >= 1 && v <= n) && (u >= 1 && u <= n)) {

    // sia E(w) insieme di lati incidenti in w
    // caso speciale 0: default, nessun caso speciale

    // caso speciale 1: E(w)\({w, v}, {w, u}) ha lati TUTTI selezionati, di cui TUTTI vietati => forza {w, v} e {w, u}
    // caso speciale 2: E(w)\({w, v}, {w, u}) ha lati TUTTI selezionati, di cui = 1 forzato => vieta {w, v} e forza {w, u} oppure forza {w, v} e vieta {w, u}
    // caso speciale 3: E(w)\({w, v}, {w, u}) ha lati TUTTI selezionati, di cui = 2 forzati => vieta {w, v} e {w, u}
    // caso speciale 4: E(w)\({w, v}, {w, u}) ha lati > 2 forzati => ritorna

    
    num_selected = num_forbidden = num_forced = 0;
    for (j = 1; i <= n; j++) {
      if (j != v && j != u && j != w) {
	if (graph_get_edge_cost(G, w, j) >= BIG)
	  num_forbidden++;
	if (graph_get_edge_cost(G, w, j) <= SMALL)
	  num_forced++;
      }
    }

    special_case = 0;
    
    if (num_forced == 0 && num_forbidden + num_forced == n-3)
      special_case = 1;
    else if (num_forced == 1 && num_forbidden + num_forced == n-3)
      special_case = 2;
    else if (num_forced == 2 && num_forbidden + num_forced == n-3)
      special_case = 3;
    else if (num_forced > 2)
      special_case = 4;
    

    if (special_case == 1) {
      cost_wv = graph_get_edge_cost(G, w, v);
      cost_wu = graph_get_edge_cost(G, w, u);
      graph_set_edge_cost(G, w, v, SMALL);
      graph_set_edge_cost(G, w, u, SMALL);
      // *** heuristic upper bound ***
      ub = compute_upper_bound(G);
     
      solve_tsp(G, H, incumbent, flag, ub, ub_max_iter, alpha, alpha_max_iter, max_iter);
      graph_set_edge_cost(G, w, v, cost_wv);
      graph_set_edge_cost(G, w, u, cost_wu);
      return;
    }
    if (special_case == 2) {
      cost_wv = graph_get_edge_cost(G, w, v);
      cost_wu = graph_get_edge_cost(G, w, u);
      graph_set_edge_cost(G, w, v, SMALL);
      graph_set_edge_cost(G, w, u, BIG);
      // *** heuristic upper bound ***
      ub = compute_upper_bound(G);

      solve_tsp(G, H, incumbent, flag, ub, ub_max_iter, alpha, alpha_max_iter, max_iter);
      graph_set_edge_cost(G, w, v, cost_wv);
      graph_set_edge_cost(G, w, u, cost_wu);

      cost_wv = graph_get_edge_cost(G, w, v);
      cost_wu = graph_get_edge_cost(G, w, u);
      graph_set_edge_cost(G, w, v, BIG);
      graph_set_edge_cost(G, w, u, SMALL);
      // *** heuristic upper bound ***
      ub = compute_upper_bound(G);

      solve_tsp(G, H, incumbent, flag, ub, ub_max_iter, alpha, alpha_max_iter, max_iter);
      graph_set_edge_cost(G, w, v, cost_wv);
      graph_set_edge_cost(G, w, u, cost_wu);
      return;
    }


    if (special_case == 3) {
      cost_wv = graph_get_edge_cost(G, w, v);
      cost_wu = graph_get_edge_cost(G, w, u);
      graph_set_edge_cost(G, w, v, BIG);
      graph_set_edge_cost(G, w, u, BIG);
      // *** heuristic upper bound ***
      ub = compute_upper_bound(G);

      solve_tsp(G, H, incumbent, flag, ub, ub_max_iter, alpha, alpha_max_iter, max_iter);
      graph_set_edge_cost(G, w, v, cost_wv);
      graph_set_edge_cost(G, w, u, cost_wu);
      return;
    }

    if (special_case == 4) {
      return;
    }

    if (special_case == 0) {
      // vieta il lato {w, v};
      cost_wv = graph_get_edge_cost(G, w, v);
      graph_set_edge_cost(G, w, v, BIG);
      // *** heuristic upper bound ***
      ub = compute_upper_bound(G);

      solve_tsp(G, H, incumbent, flag, ub, ub_max_iter, alpha, alpha_max_iter, max_iter);
      graph_set_edge_cost(G, w, v, cost_wv);

      // forza il lato {w, v} e vieta il lato {w, u};
      cost_wv = graph_get_edge_cost(G, w, v);
      cost_wu = graph_get_edge_cost(G, w, u);
      graph_set_edge_cost(G, w, v, SMALL);
      graph_set_edge_cost(G, w, u, BIG);
      // *** heuristic upper bound ***
      ub = compute_upper_bound(G);

      solve_tsp(G, H, incumbent, flag, ub, ub_max_iter, alpha, alpha_max_iter, max_iter);
      graph_set_edge_cost(G, w, v, cost_wv);
      graph_set_edge_cost(G, w, u, cost_wu);
    
      // forza i lati {w, v} e {w, u}, vieta tutti gli altri lati;
      previous_cost = (double*)malloc(sizeof(double) * n);
      for (i = 1; i <= n; i++) { // memorizza i costi attuali dei lati per poter fare un roll back al ritorno dalla prossim chiamata ricorsiva;
	if (i != w)
	  previous_cost[i-1] = graph_get_edge_cost(G, w, i);
      }
      graph_set_edge_cost(G, w, v, SMALL);
      graph_set_edge_cost(G, w, u, SMALL);
      for (i = 1; i <= n; i++) {
	if (i != w && i != v && i != u)
	  graph_set_edge_cost(G, w, i, BIG);
      }
      // *** heuristic upper bound ***
      ub = compute_upper_bound(G);

      solve_tsp(G, H, incumbent, flag, ub, ub_max_iter, alpha, alpha_max_iter, max_iter);
      for (i = 1; i <= n; i++) { // roll back dei costi così com'erano prima della chiamata ricorsiva;
	if (i != w)
	  graph_set_edge_cost(G, w, i, previous_cost[i-1]);
      }
      free(previous_cost);
    }

  }


  /* esiste un solo lato che non è mai stato nè forzato nè vietato;
   */
  else if (((v >= 1 && v <= n) && (u < 1 || u > n)) || ((v < 1 || v > n) && (u >= 1 && u <= n))) { // per come sono stati cercati v e u, {w, v} è l'unico lato non ancora selezionato

    // sia E(w) insieme di lati incidenti in w
    // caso speciale 0: default, nessun caso specialevieta {w, v} oppure 
    // caso speciale 1: E(w)\({w, v}) ha lati TUTTI vietati => ritorna
    // caso speciale 2: E(w)\({w, v}) ha lati TUTTI selezionati, di cui = 1 forzato => forza {w, v}
    // caso speciale 3: E(w)\({w, v}) ha lati TUTTI selezionati, di cui = 2 forzato => vieta {w, v}
    // caso speciale 4: E(w)\({w, v}) ha lati > 2 forzati => ritorna

     
    num_selected = num_forbidden = num_forced = 0;
    for (j = 1; i <= n; j++) {
      if (j != v && j != u && j != w) {
	if (graph_get_edge_cost(G, w, j) >= BIG)
	  num_forbidden++;
	if (graph_get_edge_cost(G, w, j) <= SMALL)
	  num_forced++;
      }
    }
    

    special_case = 0;
    
    if (num_forced == 0 && num_forbidden + num_forced == n-3)
      special_case = 1;
    else if (num_forced == 1 && num_forbidden + num_forced == n-3)
      special_case = 2;
    else if (num_forced == 2 && num_forbidden + num_forced == n-3)
      special_case = 3;
    else if (num_forced > 2)
      special_case = 4;
    

    if (special_case == 1) {
      return;
    }

    if (special_case == 2) {
      cost_wv = graph_get_edge_cost(G, w, v);
      graph_set_edge_cost(G, w, v, SMALL);
      // *** heuristic upper bound ***
      ub = compute_upper_bound(G);

      solve_tsp(G, H, incumbent, flag, ub, ub_max_iter, alpha, alpha_max_iter, max_iter);
      graph_set_edge_cost(G, w, v, cost_wv);
      return;
    }

    if (special_case == 3) {
      cost_wv = graph_get_edge_cost(G, w, v);
      graph_set_edge_cost(G, w, v, BIG);
      // *** heuristic upper bound ***
      ub = compute_upper_bound(G);

      solve_tsp(G, H, incumbent, flag, ub, ub_max_iter, alpha, alpha_max_iter, max_iter);
      graph_set_edge_cost(G, w, v, cost_wv);
      return;
    }

    if (special_case == 4) {
      return;
    }

    if (special_case == 0) {

      // vieta il lato {w, v};
      cost_wv = graph_get_edge_cost(G, w, v);
      graph_set_edge_cost(G, w, v, BIG);
      // *** heuristic upper bound ***
      ub = compute_upper_bound(G);

      solve_tsp(G, H, incumbent, flag, ub, ub_max_iter, alpha, alpha_max_iter, max_iter);
      graph_set_edge_cost(G, w, v, cost_wv);

      // forza il lato {w, u};
      cost_wv = graph_get_edge_cost(G, w, v);
      graph_set_edge_cost(G, w, v, SMALL);
      // *** heuristic upper bound ***
      ub = compute_upper_bound(G);

      solve_tsp(G, H, incumbent, flag, ub, ub_max_iter, alpha, alpha_max_iter, max_iter);
      graph_set_edge_cost(G, w, v, cost_wv);
    }
  }

  /* tutti i lati sono già stati forzati o vietati;
   */
  else if ((v < 1 || v > n) && (u < 1 || u > n)) {
    return;
  }

  /* copia il grafo iniziale nuovamente il G (la funzione in questo modo non modifica gli oggetti passati in ingresso);
   */
  if (flag == 1) {
    graph_copy(&INITIAL_GRAPH, G);
    graph_delete(&INITIAL_GRAPH);
    flag = 2;
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