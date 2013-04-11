#include "compute_nearest_neighbour.h"

double compute_nearest_neighbour(graph *G, tree *OT, int start_node) {
  double min, cost_of_cycle, tmp_cost;
  int k, i, current, next;

  int n = (*G).n;
  short visited[n];
  memset(visited, 0, sizeof(visited));

  tree_delete(OT);
  tree_init(OT, n);
  tree_set_root(OT, start_node);

  /*for (i = 0; i < n+1; i++)
    visited[i] = 0;*/

  cost_of_cycle = 0;

  // mark first node as visited
  current = start_node;
  visited[current-1] = 1;

  // iterate thru all the nodes, starting from node 1
  for (k = 1; k < n; k++) {

    // initialize minimum cost
    min = BIG;
    next = -1;

    // look for min-cost not visited neighbour
    for (i = 1; i <= n; i++) {
      tmp_cost = graph_get_edge_cost(G, current, i);
      if (i != current && visited[i-1] == 0 && tmp_cost < min) {
        min = tmp_cost;
        next = i;
        //break;
      }
    }

    /*for (i = 1; i <= n; i++) {
      if (visited[i] == 0 && graph_get_edge_cost(G, current, i) < min) {
        min = graph_get_edge_cost(G, current, i);
        next = i;
      }
    }*/

    cost_of_cycle += min;
    tree_insert_edge(OT, current, next, min);
    //printf("(%d,%d), \n", current, next);
    visited[current-1] = 1;
    current = next;
  }

  // append last edge, for comin' home to 1
  min = graph_get_edge_cost(G, current, start_node);
  tree_insert_edge(OT, current, start_node, min);
  //printf("(%d,%d)\n", current, start_node);

  cost_of_cycle += min;

  /*printf("cost of the NN tour starting from node %d: %f.\n", start_node, cost_of_cycle);
  int a = 0, b = OT->V[a].pred;
  for (i = 0; i < n; ++i) {
    printf("(%d,%d) ", a, b);
    a = b;
    b = OT->V[a].pred;
    //printf("%d ", OT->V[i].pred);
  }
  printf("\n");/ **/

  return cost_of_cycle;
}
