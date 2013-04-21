#include "compute_nearest_neighbour.h"

/*double compute_nearest_neighbour(graph *G, int start_node) {
  double min, cost_of_cycle;
  int k, i, current, next;

  int n = (*G).n;
  int visited[n+1];

  for (i = 0; i < n+1; i++)
    visited[i] = 0;

  cost_of_cycle = 0;

  current = start_node;
  visited[current] = 1;

  for (k = 1; k < n; k++) {
    for (i = 1; i <= n; i++) {
      if (visited[i] == 0) {
        min = graph_get_edge_cost(G, current, i);
        next = i;
        break;
      }
    }
    for (i = 1; i <= n; i++) {
      if (visited[i] == 0 && graph_get_edge_cost(G, current, i) < min) {
        min = graph_get_edge_cost(G, current, i);
        next = i;
      }
    }
    cost_of_cycle += min;
    current = next;
    visited[current] = 1;
  }

  cost_of_cycle += graph_get_edge_cost(G, current, start_node);

  return cost_of_cycle;
}*/


/*double compute_nearest_neighbour(graph *G, tree *OT, int start_node) {
  double min, cost_of_cycle, tmp_cost;
  int k, i, current, next;

  int n = (*G).n;
  short visited[n];
  memset(visited, 0, sizeof(visited));


  tree_delete(OT);
  tree_init(OT, n);
  tree_set_root(OT, start_node);

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

    if (next == -1) {*/
      /*for (i = 1; i <= n; i++) {
        tmp_cost = graph_get_edge_cost(G, current, i);
        printf("--> current = %d ; i = %d ; cost = %f\n", current, i, tmp_cost);
      }*/
      //char ch = getchar();
      /*return BIG;
    }*/

    /*for (i = 1; i <= n; i++) {
      if (visited[i] == 0 && graph_get_edge_cost(G, current, i) < min) {
        min = graph_get_edge_cost(G, current, i);
        next = i;
      }
    }*/

    /*cost_of_cycle += min;
    tree_insert_edge(OT, current, next, min);
    //printf("(%d,%d), \n", current, next);
    visited[current-1] = 1;
    current = next;
  }

  // append last edge, for comin' home to 1
  min = graph_get_edge_cost(G, current, start_node);
  tree_insert_edge(OT, current, start_node, min);
  //printf("(%d,%d)\n", current, start_node);

  cost_of_cycle += min;*/

  /*printf("cost of the NN tour starting from node %d: %f.\n", start_node, cost_of_cycle);
  int a = 0, b = OT->V[a].pred;
  for (i = 0; i < n; ++i) {
    printf("(%d,%d) ", a, b);
    a = b;
    b = OT->V[a].pred;
    //printf("%d ", OT->V[i].pred);
  }
  printf("\n");/ **/

  /*return cost_of_cycle;
}*/

double compute_nearest_neighbour(graph *G, cycle *C, int start_node) {
  int i, j, k;
  int current, next;
  double min, current_cost, total_cost = 0.;
  short visited[G->n + 1];
  memset(visited, 0, sizeof(short) * (G->n + 1));

  //printf("beginning NN\n");

  cycle_delete(C);

  //printf("deleted\n");

  cycle_init(C, G->n);

  //printf("hello\n");

  current = start_node;
  visited[current] = 1;
  //C->node[0] = start_node;

  for (i = 1; i < G->n; ++i) {

    min = BIG;
    next = -1;

    for (j = 1; j <= G->n; ++j) {
      current_cost = graph_get_edge_cost(G, current, j);
      if (current != j && visited[j] == 0 && current_cost < min) {
        //printf("%d %d %f\n", current, j, current_cost);
        next = j;
        min = current_cost;
      }
    }

    if (next == -1) {
      return BIG;
    }

    total_cost += min;
    C->node[i-1] = current;
    C->costs[i-1] = min;

    visited[current] = 1;
    current = next;

  }

  C->node[G->n - 1] = current;
  C->costs[G->n - 1] = graph_get_edge_cost(G, current, start_node);
  /*for (i = 0; i < G->n; ++i) {
    printf("(%d, %f) ", C->node[i], C->costs[i]);
  }
  printf("\n");*/

  return total_cost;

}

