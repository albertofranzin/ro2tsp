#include "solve_tsp.h"


graph WORK_GRAPH;

int tour_found_flag;
int current_level;
int num_of_levels; 
int num_of_calls; 


void solve_tsp(graph* G, onetree* H, double* incumbent, int call_flag) {
  int i, j, u, v, w, current_call;
  double z, cost_wv, cost_wu;
  onetree ONE_TREE;
  int n = (*G).n;

  /* effettua una copia del grafo iniziale G nel grafo di lavoro WORK_GRAPH;
   */
  if (call_flag == 0) {
    graph_init(&WORK_GRAPH, 1);
    graph_copy(G, &WORK_GRAPH);
    onetree_delete(H);
    current_level = 1;
    num_of_levels = 1;
    num_of_calls = 1;
    tour_found_flag = 0;
    call_flag = 1;
  }
  else {
    call_flag = 2;
  }

  num_of_levels = (current_level > num_of_levels) ? current_level : num_of_levels;
  current_call = num_of_calls;
  onetree_init(&ONE_TREE, 1);


  /* calcola 1-albero;
   */
  if (call_flag == 1) {
    //compute_ot(&WORK_GRAPH, &ONE_TREE);
    compute_lagrange(&WORK_GRAPH, &ONE_TREE, *incumbent);
  }
  else if (call_flag == 2) {
    compute_lagrange(&WORK_GRAPH, &ONE_TREE, compute_upper_bound(&WORK_GRAPH));
    //compute_ot(&WORK_GRAPH, &ONE_TREE);
  }

  /* calcola z = costo dell'1-albero;
   */
  onetree_set_edge_cost(&ONE_TREE, 1, onetree_get_first_node(&ONE_TREE), graph_get_edge_cost(G, 1, onetree_get_first_node(&ONE_TREE))); 
  onetree_set_edge_cost(&ONE_TREE, 1, onetree_get_second_node(&ONE_TREE), graph_get_edge_cost(G, 1, onetree_get_second_node(&ONE_TREE)));
  for (i = 2; i <= n; i++) {
    onetree_set_edge_cost(&ONE_TREE, onetree_get_pred(&ONE_TREE, i), i, graph_get_edge_cost(G, onetree_get_pred(&ONE_TREE, i), i));
  }
  z = onetree_get_cost(&ONE_TREE);

  /* verifica se possibile potare il ramo corrente
   */
  if (z > *incumbent) {
    onetree_delete(&ONE_TREE);
    return;
  }

  /* verifica se è stato trovato un ciclo (una soluzione ottima per il sottoproblema corrispondente al nodo corrente);
   * aggiorna incumbent e soluzione ottima corrente;
   */
  if (is_cycle(&ONE_TREE)) {
    if (z < *incumbent || tour_found_flag == 0) {
      printf("# updated incumbent = %f : current_call = %d : current level = %d\n", z, current_call, current_level);
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
    num_of_calls++;
    current_level++;
    solve_tsp(G, H, incumbent, call_flag);
    current_level--;
    graph_set_edge_cost(&WORK_GRAPH, w, v, cost_wv);

    // forza il lato {w, v} e vieta il lato {w, u};
    cost_wv = graph_get_edge_cost(&WORK_GRAPH, w, v);
    cost_wu = graph_get_edge_cost(&WORK_GRAPH, w, u);
    graph_set_edge_cost(&WORK_GRAPH, w, v, SMALL);
    graph_set_edge_cost(&WORK_GRAPH, w, u, BIG);
    num_of_calls++;
    current_level++;
    solve_tsp(G, H, incumbent, call_flag);
    current_level--;
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
    num_of_calls++;
    current_level++;
    solve_tsp(G, H, incumbent, call_flag);
    current_level--;
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
    num_of_calls++;
    current_level++;
    solve_tsp(G, H, incumbent, call_flag);
    current_level--;
    graph_set_edge_cost(&WORK_GRAPH, w, v, cost_wv);

    // forza il lato {w, u};
    cost_wv = graph_get_edge_cost(&WORK_GRAPH, w, v);
    graph_set_edge_cost(&WORK_GRAPH, w, v, SMALL);
    num_of_calls++;
    current_level++;
    solve_tsp(G, H, incumbent, call_flag);
    current_level--;
    graph_set_edge_cost(&WORK_GRAPH, w, v, cost_wv);
  }

  /* tutti i lati sono già stati forzati o vietati;
   */
  else if ((v < 1 || v > n) && (u < 1 || u > n)) {
    return;
  }

  if (call_flag == 1) {
    graph_delete(&WORK_GRAPH);
    printf("# number of levels = %d\n# number of calls = %d\n", num_of_levels, num_of_calls);
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
