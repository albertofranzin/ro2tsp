#include "graph.h"
#include "list.h"
#include "tree.h"
#include "compute_mst.h"

void compute_mst(graph* G, tree* T, int root) {
  int i, j, u, v, m;
  double min;

  int n = (*G).n;
  int pred[n];
  double cost[n];
  list not_visited;
  list_node* current_node;
  list_node* chosen_node;

  list_init(&not_visited);
  tree_delete(T);
  tree_init(T, n);
  tree_set_root(T, root);

  /* inizializzazione delle principali strutture;
   * - pongo a root il predecessore dei nodi direttamente connessi alla radice;
   * - pongo a 0 il predecessore dei nodi non direttamente connessi alla radice;
   *   si tratta di nodi di cui al momento non si conosce la raggiungibilità (da root);
   */
  for (j = 1; j <= n; j++) {
    if (j == root) {
      pred[root-1] = 0;
    }
    else {
      list_push_last(&not_visited, j);
      if (graph_adjacent_nodes(G, root, j)) {
        pred[j-1] = root;
        cost[j-1] = graph_get_edge_cost(G, root, j);
      }
      else {
        pred[j-1] = 0;
      }
    }
  }

  /* loop principale di n-1 cicli;
   * - se G è connesso, allora ad ogni ciclo viene aggiunto un nuovo lato;
   * - se G non è connesso, è possibile che negli ultimi n-k-1 cicli non venga aggiunto
   *   alcun lato; k è il numero di nodi del sottografo connesso al quale appartiene root;
   */ 
  while (!list_is_empty(&not_visited)) {
    m = list_get_size(&not_visited);

    // ricerca un primo nodo candidato per estendere la regione dei nodi già ricoperti dall'albero;
    current_node = list_get_first(&not_visited);
    chosen_node = current_node;
    v = (*chosen_node).data;
    min = cost[v-1];

    // a partire dal candidato, cerca il miglior candidato per estendere la regione dei nodi già ricoperti dall'albero;

    for (j = 0; j < m-1; j++) {
      current_node = list_get_next(&not_visited, current_node);
      u = (*current_node).data;
      if (pred[u-1] > 0 && cost[u-1] < min) {
	chosen_node = current_node;
	v = u;
	min = cost[v-1];
      }
    }

    // segna il nodo come ricoperto;
    list_remove(&not_visited, chosen_node);

    // inserisci un nuovo lato;
    tree_insert_edge(T, pred[v-1], v, graph_get_edge_cost(G, pred[v-1], v));

    // aggiorna i costi di raggiungibilità dei nodi non ancora ricoperti e raggiungibili direttamente dal nodo appena ricoperto;
    if (!list_is_empty(&not_visited)) {
      m = list_get_size(&not_visited);
      current_node = list_get_first(&not_visited);
      u = (*current_node).data;
      if (graph_adjacent_nodes(G, v, u) && (pred[u-1] == 0 || (pred[u-1] > 0 && graph_get_edge_cost(G, v, u) < cost[u-1]))) {
	pred[u-1] = v;
	cost[u-1] = graph_get_edge_cost(G, v, u);
      }
      for (j = 0; j < m-1; j++) {
	current_node = list_get_next(&not_visited, current_node);
	u = (*current_node).data;
	if (graph_adjacent_nodes(G, v, u) && (pred[u-1] == 0 || (pred[u-1] > 0 && graph_get_edge_cost(G, v, u) < cost[u-1]))) {
	  pred[u-1] = v;
	  cost[u-1] = graph_get_edge_cost(G, v, u);
	}
      }
    }
  }
}
