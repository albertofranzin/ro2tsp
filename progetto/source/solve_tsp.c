#include "graph.h"
#include "solve_tsp.h"

graph INITIAL_GRAPH;

int calls = 0;

void solve_tsp(graph* G, graph* H, double* incumbent, int flag) {
  int i, j, u, v, w;
  double z;
  graph ONE_TREE;
  int n = (*G).n;
  double* previous_cost;;
  double cost_wv, cost_wu;

  calls += 1;

  /* effettua una copia del grafo iniziale G, passato in ingresso alla prima chiamata della funzione;
   */
  if (flag == 0) {
    initGraph(&INITIAL_GRAPH, 1);
    copyGraph(G, &INITIAL_GRAPH);
    flag = 1;
  }
  else {
    flag = 2;
  }

  initGraph(&ONE_TREE, 1);

  /* calcola 1-albero;
   */
  compute_ot(G, &ONE_TREE);

  /* calcola z = costo dell'1-albero;
   */
  for (i = 1; i <= n; i++) {
    for (j = i+1; j <= n; j++)
      set_edge_cost(&ONE_TREE, i, j, get_edge_cost(&INITIAL_GRAPH, i, j));
  }
  z = get_graph_cost(&ONE_TREE);

  /* verifica se possibile potare il ramo corrente
   */
  if (z >= *incumbent) {
    deleteGraph(&ONE_TREE);
    return;
  }

  /* verifica se è stato trovato un ciclo (una soluzione ottima per il sottoproblema corrispondente al nodo corrente);
   * aggiorna incumbent e soluzione ottima corrente;
   */
  if (is_cycle(&ONE_TREE)) {
    printf("updating incumbent : from %f to %f\n", *incumbent, z);
    printf("calls so far: %d\n", calls);
    if (z < *incumbent) {
      *incumbent = z;
      copyGraph(&ONE_TREE, H);
    }
    deleteGraph(&ONE_TREE);
    return;
  }

  /* cerca un nodo w con almeno 3 lati incidenti;
   */
  for (w = 1; w <= n; w++) {
    if (get_node_deg(&ONE_TREE, w) >= 3)
      break;
  }

  /* (tentativo di) ricerca di due lati candidati mai forzati nè vietati;
   */
  for (v = 1; v <= n; v++) {
   if (v != w && adjacent(&ONE_TREE, w, v) && get_edge_cost(G, w, v) > SMALL && get_edge_cost(G, w, v) < BIG)
      break;
  }
  for (u = 1; u <= n; u++) {
    if (u != w && u != v && adjacent(&ONE_TREE, w, u) && get_edge_cost(G, w, u) > SMALL && get_edge_cost(G, w, u) < BIG)
      break;
  }

  deleteGraph(&ONE_TREE);

  /* se esistono due lati mai selezionati allora procedi con BRANCHING A 3 VIE;
   */
  if ((v >= 1 && v <= n) && (u >= 1 && u <= n)) {
    
    // vieta il lato {w, v};
    cost_wv = get_edge_cost(G, w, v);
    set_edge_cost(G, w, v, BIG);
    solve_tsp(G, H, incumbent, flag);
    set_edge_cost(G, w, v, cost_wv);

    // forza il lato {w, v} e vieta il lato {w, u};
    cost_wv = get_edge_cost(G, w, v);
    cost_wu = get_edge_cost(G, w, u);
    set_edge_cost(G, w, v, SMALL);
    set_edge_cost(G, w, u, BIG);
    solve_tsp(G, H, incumbent, flag);
    set_edge_cost(G, w, v, cost_wv);
    set_edge_cost(G, w, u, cost_wu);

    // forza i lati {w, v} e {w, u}, vieta tutti gli altri lati;
    previous_cost = (double*)malloc(sizeof(double) * n);
    for (i = 1; i <= n; i++) { // memorizza i costi attuali dei lati per poter fare un roll back al ritorno dalla prossim chiamata ricorsiva;
      if (i != w) 
	previous_cost[i-1] = get_edge_cost(G, w, i);
    }
    set_edge_cost(G, w, v, SMALL);
    set_edge_cost(G, w, u, SMALL);
    for (i = 1; i <= n; i++) {
      if (i != w && i != v && i != u)
	set_edge_cost(G, w, i, BIG);
    }
    solve_tsp(G, H, incumbent, flag);
    for (i = 1; i <= n; i++) { // roll back dei costi così com'erano prima della chiamata ricorsiva;
      if (i != w)
	set_edge_cost(G, w, i, previous_cost[i-1]);
    }
    free(previous_cost);
  } 

  /* esiste un solo lato che non è mai stato nè forzato nè vietato;
   */
  else if (((v >= 1 && v <= n) && (u < 1 || u > n)) || ((v < 1 || v > n) && (u >= 1 && u <= n))) {
    
    // vieta il lato {w, v};
    cost_wv = get_edge_cost(G, w, v);
    set_edge_cost(G, w, v, BIG);
    solve_tsp(G, H, incumbent, flag);
    set_edge_cost(G, w, v, cost_wv);

    // forza il lato {w, u};
    cost_wv = get_edge_cost(G, w, v);
    set_edge_cost(G, w, v, SMALL);
    solve_tsp(G, H, incumbent, flag);
    set_edge_cost(G, w, v, cost_wv);
  }

  /* tutti i lati sono già stati forzati o vietati;
   */
  else if ((v < 1 || v > n) && (u < 1 || u > n)) {
    return;
  }

  /* copia il grafo iniziale nuovamente il G (la funzione in questo modo non modifica gli oggetti passati in ingresso);
   */
  if (flag == 1) {
    copyGraph(&INITIAL_GRAPH, G);
    deleteGraph(&INITIAL_GRAPH);
    flag = 2;
  }
}

/* ritorna 1 se G è un ciclo, 0 altrimenti;
 * - si assume G connesso (altrimenti non è detto che G sia un ciclo anche se la funzione ritorna 1;
 */
int is_cycle(graph* G) {
  int i;
  int n = (*G).n;
 
  for (i = 1; i <= n; i++) {
    if (get_node_deg(G, i) != 2)
      return 0;
  }
  return 1;
}
