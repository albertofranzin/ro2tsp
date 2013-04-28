#include "compute_ot.h"

void compute_ot(graph* G, onetree* OT) {
  int i, j, v1, v2;
  double min1, min2, c;

  int n = (*G).n;

  onetree_delete(OT);
  onetree_init(OT, n);

  // cerca i due lati {1, v1} e {1, v2} di costo minore tra i lati incidenti in 1
  // si assume cost({1, v1}) <= cost({1, v2})
  v1 = (graph_get_edge_cost(G, 1, 2) < graph_get_edge_cost(G, 1, 3)) ? 2 : 3;
  v2 = (v1 == 2) ? 3 : 2;
  min1 = graph_get_edge_cost(G, 1, v1);
  min2 = graph_get_edge_cost(G, 1, v2);

  for (i = 4; i <= n; i++) {
    c = graph_get_edge_cost(G, 1, i);
    if (c < min1) {
      v2 = v1;
      min2 = min1;
      v1 = i;
      min1 = c;
    }
    else if (c < min2) {
      v2 = i;
      min2 = c;
    }
  }

  // calcola mst sui nodi {2, 3, ..., n} con radice r = 2; la procedura è una versione riadattata di compute_mst
  int u, v, m;
  double min;
  list_node* current_node;
  list_node* chosen_node;

  int pred[n-1];
  double cost[n-1];
  list not_visited;
  list_init(&not_visited);

  int r = v1;
  pred[r-2] = 0; // ricorda che pred[i], cost[i], dove 0 <= i < n-1, sono associati al nodo i+2 del grafo

  for (j = 2; j <= n; j++) {
    if (j == r) {
      pred[j-2] = 0;
    }
    else {
      list_push_last(&not_visited, j);
      if (graph_adjacent_nodes(G, r, j)) {
	pred[j-2] = r;
	cost[j-2] = graph_get_edge_cost(G, r, j);
      }
      else {
	pred[j-2] = 0;
      }
    }
  }

  while (!list_is_empty(&not_visited)) {
    m = list_get_size(&not_visited);

    current_node = list_get_first(&not_visited);
    chosen_node = current_node;
    v = (*chosen_node).data;
    min = cost[v-2];
    for (j = 0; j < m-1; j++) {
      current_node = list_get_next(&not_visited, current_node);
      u = (*current_node).data;
      if (pred[u-2] > 0 && cost[u-2] < min) {
	chosen_node = current_node;
	v = u;
	min = cost[v-2];
      }
    }

    list_remove(&not_visited, chosen_node);

    onetree_insert_edge(OT, pred[v-2], v, graph_get_edge_cost(G, pred[v-2], v));

    if (!list_is_empty(&not_visited)) {
      m = list_get_size(&not_visited);
      current_node = list_get_first(&not_visited);
      u = (*current_node).data;
      if (graph_adjacent_nodes(G, v, u) && (pred[u-2] == 0 || (pred[u-2] > 0 && graph_get_edge_cost(G, v, u) < cost[u-2]))) {
	pred[u-2] = v;
	cost[u-2] = graph_get_edge_cost(G, v, u);
      }
      for (j = 0; j < m-1; j++) {
	current_node = list_get_next(&not_visited, current_node);
	u = (*current_node).data;
	if (graph_adjacent_nodes(G, v, u) && (pred[u-2] == 0 || (pred[u-2] > 0 && graph_get_edge_cost(G, v, u) < cost[u-2]))) {
	  pred[u-2] = v;
	  cost[u-2] = graph_get_edge_cost(G, v, u);
	}
      }
    }
  }

  // infine vengono aggiunti i due lati di costo minimo incidenti in 1
  onetree_insert_edge(OT, 1, v1, min1); // dopo l'inserimento del lato {1, v1}, in OT è presente un albero ricoprente sui nodi {1, 2, ..., n} radicato in 1, in cui ogni nodo eccetto 1 possiede un predecessore, consistentemente con l'ordinamento indotto dalla radicatura in 1
  onetree_insert_edge(OT, v2, 1, min2); // viene aggiunto il lato {1, v2} ponendo v2 come predecessore di 1
}
