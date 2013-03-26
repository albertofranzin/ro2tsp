#include <stdlib.h>
#include "graph.h"
#include "solve_tsp.h"

graph INITIAL_GRAPH;

void solve_tsp(graph* G, graph* H, double* incumbent, int flag) {
  int i, j, u, v, w;
  double z;
  graph ONE_TREE;
  int n = (*G).n;
  double* previous_cost = (double*)malloc(sizeof(double) * n);

  if (flag == 0) {
    initGraph(&INITIAL_GRAPH, 1);
    copyGraph(G, &INITIAL_GRAPH);
    flag = 1;
  }
  else {
    flag = 2;
  }

  initGraph(&ONE_TREE, 1);

  compute_ot(G, &ONE_TREE);

  for (i = 1; i <= n; i++) {
    for (j = i+1; j <= n; j++)
      set_edge_cost(&ONE_TREE, i, j, get_edge_cost(&INITIAL_GRAPH, i, j));
  }
  z = get_graph_cost(&ONE_TREE);


  if (z >= *incumbent) {
    return;
  }

  if (is_cycle(&ONE_TREE)) {
    if (z < *incumbent) {
      *incumbent = z;
      copyGraph(&ONE_TREE, H);
    }
    return;
  }

  for (w = 1; w <= n; w++) {
    if (get_node_deg(&ONE_TREE, w) >= 3)
      break;
  }

  for (v = 1; v <= n; v++) {
   if (v != w && adjacent(&ONE_TREE, w, v) && get_edge_cost(G, w, v) > 0 && get_edge_cost(G, w, v) < INF)
      break;
  }
  for (u = 1; u <= n; u++) {
    if (u != w && u != v && adjacent(&ONE_TREE, w, u) && get_edge_cost(G, w, u) > 0 && get_edge_cost(G, w, u) < INF)
      break;
  }

  deleteGraph(&ONE_TREE);

  // 1.1 esistono due lati mai selezionati
  if ((v >= 1 && v <= n) && (u >= 1 && u <= n)) {
    /* BRANCH 1
     */
    previous_cost[v-1] = get_edge_cost(G, w, v);
    set_edge_cost(G, w, v, INF);
    solve_tsp(G, H, incumbent, flag);
    set_edge_cost(G, w, v, previous_cost[v-1]);
    /* BRANCH 2
     */
    previous_cost[v-1] = get_edge_cost(G, w, v);
    previous_cost[u-1] = get_edge_cost(G, w, u);
    set_edge_cost(G, w, v, 0);
    set_edge_cost(G, w, u, INF);
    solve_tsp(G, H, incumbent, flag);
    set_edge_cost(G, w, v, previous_cost[v-1]);
    set_edge_cost(G, w, u, previous_cost[u-1]);

    /* BRANCH 3
     */
    for (i = 1; i <= n; i++) {
      if (i != w)
	previous_cost[i-1] = get_edge_cost(G, w, i);
    }
    set_edge_cost(G, w, v, 0);
    set_edge_cost(G, w, u, 0);
    for (i = 1; i <= n; i++) {
      if (i != w)
	set_edge_cost(G, w, i, INF);
    }
    solve_tsp(G, H, incumbent, flag);
    for (i = 1; i <= n; i++) {
      if (i != w)
	set_edge_cost(G, w, i, previous_cost[i-1]);
    }
  } 

  // 1.2 esiste un solo lato che non è mai stato selezionato
  else if (((v >= 1 && v <= n) && (u < 1 || u > n)) || ((v < 1 || v > n) && (u >= 1 && u <= n))) {
    /* BRANCH 1
     */
    previous_cost[v-1] = get_edge_cost(G, w, v);
    set_edge_cost(G, w, v, INF);
    solve_tsp(G, H, incumbent, flag);
    set_edge_cost(G, w, v, previous_cost[v-1]);

    /* BRANCH 2
     */
    previous_cost[v-1] = get_edge_cost(G, w, v);
    set_edge_cost(G, w, v, 0);
    solve_tsp(G, H, incumbent, flag);
    set_edge_cost(G, w, v, previous_cost[v-1]);
  }

  // 1.3 tutti i lati sono già stati selezionati
  else if ((v < 1 || v > n) && (u < 1 || u > n)) {
    return;
  }

  if (flag == 1) {
    copyGraph(&INITIAL_GRAPH, G);
    deleteGraph(&INITIAL_GRAPH);
    flag = 2;
  }
  free(previous_cost);
}

int is_cycle(graph* G) {
  int i;
  int n = (*G).n;
 
  for (i = 1; i <= n; i++) {
    if (get_node_deg(G, i) != 2)
      return 0;
  }
  return 1;
}