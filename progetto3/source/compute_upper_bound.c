#include "compute_upper_bound.h"

/*double compute_upper_bound(graph* G) { // da migliorare magari usando altri euristici
  int i;
  double min, cost_of_cycle;
  int n = (*G).n;

  min = compute_nearest_neighbour(G, 1);
  for (i = 2; i <= n; i++) {
    cost_of_cycle = compute_nearest_neighbour(G, i);
    if (cost_of_cycle < min)
      min = cost_of_cycle;
  }
  return min;
}*/

double compute_upper_bound(graph* G, tree *OT) { // da migliorare magari usando altri euristici
  int i, best_index;
  double min, cost_of_cycle;
  int n = (*G).n;

  tree BEST_OT;
  tree_init(&BEST_OT, n);

  min = compute_nearest_neighbour(G, &BEST_OT, 1);
  best_index = 1;
  for (i = 2; i <= n; i++) {
    tree_delete(OT);
    tree_init(OT, n);
    cost_of_cycle = compute_nearest_neighbour(G, OT, i);
    if (cost_of_cycle < min) {
        min = cost_of_cycle;
        best_index = i;
        tree_copy(OT, &BEST_OT);
      }
  }
  tree_copy(&BEST_OT, OT);
  tree_delete(&BEST_OT);
  //printf("best_index is %d\n", best_index);
  return min;
}

/*
 * heur2opt
 * G: graph
 * T : tree computed by another heuristic
 *     (will be modified if it's not 2-opt)
 * cost  cost of the solution computed by the heuristic
 *
 * compute a 2-opt admissible solution, in order to swap crossing edges
 *
 * return : cost of the 2-opt solution
 */
double heur2opt(graph *G, tree *T, double ccost) {

  double cost = ccost, delta;
  int changed = 1;
  int i, j, k, h;

  //printf("starting : %f\n", cost);
  //char cch = getchar();

  while (changed) {
    changed = 0;

    for (i = 1; i <= T->n ; ++i) {

      //printf("i = %d\n", i);
      k = tree_get_pred(T, i);

      //assert(i == tree_get_succ(T, k));

      for (j = i+1; j <= T->n; ++j) {
        //printf("j = %d\n", j);
        h = tree_get_pred(T, j);

        /*printf("%d %d\n", h, j);
        assert(j == tree_get_succ(T, h));*/

        delta = 0. - graph_get_edge_cost(G, i, k) - graph_get_edge_cost(G, j, h) +
                        graph_get_edge_cost(G, i, j) + graph_get_edge_cost(G, h, k);

        if (j != k && i != h && k != h && k > 0 && h > 0 && //tree_get_succ(T, i) > 0 && tree_get_succ(T, j) > 0 &&
              !tree_adjacent_nodes(T, i, j) && !tree_adjacent_nodes(T, k, h) &&
              tree_get_node_deg(T, i) == 2 && tree_get_node_deg(T, j) == 2 &&
              tree_get_node_deg(T, h) == 2 && tree_get_node_deg(T, k) == 2 &&
              graph_get_edge_cost(G, i, j) < BIG && graph_get_edge_cost(G, h, k) < BIG &&
              graph_get_edge_cost(G, i, j) > SMALL && graph_get_edge_cost(G, h, k) > SMALL &&
/*              graph_get_edge_cost(G, i, j) + graph_get_edge_cost(G, k, h)
              < graph_get_edge_cost(G, i, k) + graph_get_edge_cost(G, j, h)) {*/
              delta < 0 ) {

          printf("swapping (%d,%d), (%d,%d) with (%d,%d), (%d,%d), with a delta of %f\n", i,k,j,h,i,j,k,h,delta);

          tree_swap_edges(G, T, i, j);

          changed = 1;
          cost = cost + delta;

          break;

        }

      }

    }

    printf("restarting! %f\n", cost);
    //char ch = getchar();

  }

  //printf("finished\n");

  return cost;
}


