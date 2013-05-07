#include "compute_upper_bound.h"


double compute_upper_bound(graph* G, cycle *C, int algo) {
  int i, best_index;
  double min, cost_of_cycle;
  int n = G->n;

  // horrible, but something
  if (algo == RANDOM_CYCLE) {
    return random_cycle_generation(G, C, 1000);
  }

  cycle BEST_CYCLE;
  cycle_init(&BEST_CYCLE, n);

  //printf("about to compute first cycle\n");
  min = compute_nearest_neighbour(G, &BEST_CYCLE, 1, algo);
  if (algo == NEAREST_NEIGHBOUR_2_OPT) {
    min = heur2opt(G, &BEST_CYCLE, min);
  }
  //printf("first cycle computed\n");
  best_index = 1;
  for (i = 2; i <= n; i++) {
    cycle_delete(C);
    cycle_init(C, n);
    cost_of_cycle = compute_nearest_neighbour(G, C, i, algo);
    if (algo == NEAREST_NEIGHBOUR_2_OPT) {
      cost_of_cycle = heur2opt(G, C, cost_of_cycle);
    }
    //printf("coc : %f\n", cost_of_cycle);
    if (cost_of_cycle < min) {
        min = cost_of_cycle;
        best_index = i;
        cycle_copy(C, &BEST_CYCLE);
      }
  }
  cycle_copy(&BEST_CYCLE, C);
  cycle_delete(&BEST_CYCLE);
  /*printf("best_index is %d, it costs %f\n", best_index, min);
  char ch = getchar();*/
  return min;
}

double compute_nearest_neighbour(graph *G, cycle *C, int start_node, int algo) {
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
  total_cost += C->costs[G->n - 1];
  /*for (i = 0; i < G->n; ++i) {
    printf("(%d, %f) ", C->node[i], C->costs[i]);
  }
  printf("\n");*/

  return total_cost;

}

/*
 * heur2opt
 * G : graph
 * C : cycle computed by another heuristic
 *     (will be modified if it's not 2-opt)
 * cost : cost of the solution computed by the heuristic
 *
 * compute a 2-opt admissible solution, in order to swap crossing edges
 *
 * return : cost of the 2-opt solution
 */
double heur2opt(graph *G, cycle *C, double ccost) {

  double cost = ccost, delta;
  int changed = 1;
  int i, j, k, h, l, tmp;

  //printf("starting : %f\n", cost);
  //char cch = getchar();

  while (changed) {
    changed = 0;

    for (i = 0; i < C->n ; ++i) {

      for (j = i+1; j < C->n; ++j) {
        k = (i+1) % G->n;
        h = (j+1) % G->n;
        delta = graph_get_edge_cost(G, C->node[i], C->node[j]) +
                graph_get_edge_cost(G, C->node[k], C->node[h]) -
                C->costs[i] -
                C->costs[j];

        if ((delta + EPSILON) < 0) {

          cost += delta;
          for (l = 0; l <= (j - k) / 2; ++l) {

            tmp            = C->node[k + l];
            C->node[k + l] = C->node[j - l];
            C->node[j - l] = tmp;

            C->costs[i + l] = graph_get_edge_cost(G, C->node[i + l],
                                                     C->node[(i+l+1) % G->n]);
            C->costs[j - l] = graph_get_edge_cost(G, C->node[j - l],
                                                     C->node[(j-l+1) % G->n]);

          }
          changed = 1;

          j = C->n;
          i = C->n;
          break;
        }
      }

    }

    //printf("restarting\n");

  }

  //printf("finished\n");

  //cost = cycle_get_cost(C);

  return cost;
}

/*
 * trimmedBranchAndBound
 * G : graph
 * OT : one-tree computed
 *
 * trim the graph, by setting 'longest' edges to infinity
 * execute a branch and bound on trimmed graph
 * 'longest' == longer than a certain threshold, for now fixed at
 * median of the lengths of edges insisting on a node
 *
 * return : cost of the onetree
 */
/*double trimmedBranchAndBound(graph *G, onetree *OT) {
  int i, j, k;
  double thr, cost = 0., ub, z;
  double costs[G->n];

  // make working copy of G
  graph TG;
  graph_init(&TG, 1);
  graph_copy(G, &TG);

  for (i = 1; i <= G->n; ++i) {
    memset(costs, 0., G->n);
    k = 0;

    for (j = 1; j <= G->n; ++j) {
      costs[k++] = graph_get_edge_cost(G, i, j);
    }

    / *for (j = 1; j <= G->n; ++j) {
      printf("%f ", costs[j]);
    }
    printf("\n");* /

    qsort(&costs, G->n, sizeof(double), sebwComp);

    thr = costs[G->n / 2];

    / *for (j = 1; j <= G->n; ++j) {
      printf("%f ", costs[j]);
    }
    printf("\n");
    char ch = getchar();* /

    for (j = 1; j <= G->n; ++j) {
      if (graph_get_edge_cost(&TG, i, j) > thr) {
        graph_set_edge_cost(&TG, i, j, BIG);
      }
    }
  }

  tree T;
  tree_init(&T, 1);
  ub = compute_upper_bound(&TG, &T);

  z = compute_lagrange(&TG, OT, ub);

  solve_tsp(&TG, OT, &ub, OT, z, 0);

  return ub;
}*/

/*
 * improved2opt
 * G: graph
 * T : tree computed by another heuristic
 *     (will be modified if it's not 2-opt)
 * cost : cost of the solution computed by the heuristic
 *
 * compute a 2-opt solution, swap 3 random edges and apply 2-opt again
 *
 * return : cost of the solution
 */
double heur232opt(graph *G, cycle *C, double ccost) {
  double cost = heur2opt(G, C, ccost);

  int n1 = 0, n2 = 0, n3 = 0, p1, p2, p3;

  int i, j, k, h, l, m;

  double second = heur2opt(G, C, cycle_get_cost(C));

  if (second < cost) {
    printf("improved2opt really improved from %f to %f\n", cost, second);
    return second;
  } else {
    printf("improved2opt did not improve\n");
    return cost;
  }

  //return ccost;  // something here is not working...
}/**/

/*
 * random_cycle_generation
 * - G : graph
 * - C : the best cycle computed
 * - num_cycles : number of cycles to be tried
 *
 * generate num_cycles random cycles, and keep the best one
 * (many thanks to Matteo Boscariol for the idea)
 *
 * return : cost of the returned cycle
 */
double random_cycle_generation(graph *G, cycle *C, int num_cycles) {
  double opt_cost = BIG, cost;
  cycle WORK_CYCLE, BEST_CYCLE;
  int i, j, nodes[G->n + 1], remaining, target, tmp;

  cycle_init(&BEST_CYCLE, 1);

  for (i = 0; i < num_cycles; ++i) {
    //printf("i = %d\n", i);

    cycle_init(&WORK_CYCLE, G->n);

    for (j = 0; j < G->n; ++j) {
      nodes[j] = j+1;
    }

    /*for (j = 0; j < G->n; ++j) {
      printf("%d ", nodes[j]);
    }
    printf("\n");*/

    //printf("array filled\n");

    remaining = G->n;

    // generate permutation of the node
    for (j = 0; j < G->n; ++j) {
      target = rand() % remaining;

      tmp = nodes[j];
      nodes[j] = nodes[target + j];
      nodes[target + j] = tmp;

      remaining--;
    }

    // fill in cycle with costs
    cost = 0.;
    for (j = 0; j < G->n; ++j) {
      WORK_CYCLE.node[j] = nodes[j];
      WORK_CYCLE.costs[j] += graph_get_edge_cost(G, nodes[j], nodes[(j+1) % G->n]);
      cost += WORK_CYCLE.costs[j];
    }

    cost = heur2opt(G, &WORK_CYCLE, cost);

    /*for (j = 0; j < G->n; ++j) {
      printf("%d ", nodes[j]);
    }
    printf("\n");*/

    if (cost < opt_cost) {
      cycle_copy(&WORK_CYCLE, &BEST_CYCLE);
      opt_cost = cost;
    }

    cycle_delete(&WORK_CYCLE);
  }

  cycle_copy(&BEST_CYCLE, C);
  cycle_delete(&BEST_CYCLE);

  return opt_cost;
}


