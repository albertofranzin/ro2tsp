#include "compute_upper_bound.h"

int compute_upper_bound(graph* G, cycle* C, int algo, double* ub) {

  if (algo == NEAREST_NEIGHBOUR_2_OPT ||
      algo == NEAREST_NEIGHBOUR         ) {
    return compute_nearest_neighbour(G, C, ub);
  }

  else if (algo == RANDOM_CYCLES      ||
           algo == RANDOM_CYCLES_2OPT   ) {
    return compute_rc(G, C, ub);
  }
  
  else if (algo == DUMB) { 
    return dumb_upper_bound(G, ub);
  }

  // should never arrive here
  assert(0);
  return -1;
}

/**
 * compute NN heuristic starting from each node. Also applies 2-opt
 * @param  G  graph
 * @param  C  best cycle found
 * @param  ub upper bound computed
 * @return    status of the operations
 */
int compute_nearest_neighbour(graph *G, cycle *C, double *ub) {
    int i, status, flag;
    double min = BIG, tour_cost;
    int n = G->n;

    cycle_delete(C);
    cycle_init(C, n);

    cycle C_tmp;
    cycle_init(&C_tmp, n);
    flag = 0;
    for (i = 0; i < n; i++) {

      status = heur_nearest_neighbour(G, &C_tmp, i, &tour_cost);
      // always returns successfully if this case is executed
      // at the root node

      if (status == 1) {

        status = heur_2_opt(G, &C_tmp, tour_cost, &tour_cost);
        // always returns successfully

        if (flag == 0 && status == SUCCESS) {
          cycle_copy(&C_tmp, C);
          min = tour_cost;
          flag = 1;
        }
        else if (flag == 1 && status == SUCCESS && tour_cost < min) {
          cycle_copy(&C_tmp, C);
          min = tour_cost;
        }

      }

    }

    if (flag == 1) {
      *ub = min;
      cycle_delete(&C_tmp);
      return SUCCESS;
    }
    else {
      cycle_delete(&C_tmp);
      return FAILURE;
    }
}

/**
 * compute an ub using RC+2-opt heuristic
 * @param  G  graph
 * @param  C  best cycle found
 * @param  ub uper bound computed
 * @return    status of the operations
 */
int compute_rc(graph *G, cycle *C, double *ub) {
  int i,
      n = G->n,
      status,
      total_trials = NUM_TRIALS_RANDOM_CYCLES_2OPT / n,
      num_of_threads = NUM_OF_THREADS;

  double min;

  pthread_t thread[num_of_threads];
  rc_params rcp[num_of_threads];
  cycle     cycles[num_of_threads];


  for (i = 0; i < num_of_threads; ++i) {
    cycle_init(&cycles[i], 0);
  }

  for (i = 0; i < num_of_threads; ++i) {
    rcp[i].th_no            = i;
    rcp[i].G                = G;
    rcp[i].C                = &cycles[i];
    rcp[i].number_of_cycles = total_trials / num_of_threads;

    if (pthread_create(&thread[i], NULL, rc_thread, (void *)&rcp[i])) {
      fprintf(stderr, "Fatal error in compute_upper_bound.c :: \n");
      fprintf(stderr, "error while creating thread %d\n", i);
      exit(1);
    }
  } // end thread creation

  status = FAILURE;
  min    = BIG;

  for (i = 0; i < num_of_threads; ++i) {
    if (pthread_join(thread[i], NULL)) {
      fprintf(stderr, "Fatal error in compute_upper_bound.c :: \n");
      fprintf(stderr, "error while joining thread %d\n", i);
      exit(1);
    }
    if (rcp[i].return_status == SUCCESS) {
      status = SUCCESS;
      if (min >= rcp[i].ub) {
        cycle_copy(rcp[i].C, C);
        min = rcp[i].ub;
      }
    }
  } // end thread joining

  *ub = min;

  return status;
}

int dumb_upper_bound(graph *G, double* ub) {
  // In this case computational complexity is cubic! Try to do better,
  // you can spend n^2 * logn time (where n is the number of vertices of the graph).
  // If you want to use some recursive algorithm such as merge-sort, then pay attention
  // to memory consumption; it may be better to use some in-place algorithm.

  double cost, max = SMALL;
  int i, j, i_max, j_max, k, flag;
  int n = G->n;
  int* selected = (int*)calloc(n * (n + 1) / 2, sizeof(int));

  *ub = 0;
  for (k = 0; k < n; k++) {

    flag = 0;
    for (i = 0; i < n; i++) {
      for (j = i+1; j < n; j++) {
        if (i != j && selected[ j*(j-1)/2 + i] == 0) {

          cost = graph_get_edge_cost(G, i, j);

          if (flag == 0) {
            max = cost;
            i_max = i;
            j_max = j;
            flag = 1;
          }
          else if (flag == 1 && cost > max) {
            max = cost;
            i_max = i;
            j_max = j;
          }

        }
      }
    }

    selected[ j_max*(j_max-1)/2 + i_max] = 1;
    *ub += max;
  }

  free(selected);
  return SUCCESS;
}

/**
 * compute a rc+2opt ub. Thought to be multithreading
 * @param p params
 */
void *rc_thread(void *p) {
  rc_params *rcp = (rc_params *) p;

  // better trying different random seeds?
  srand(time(NULL) * rcp->th_no);

  int i, status, flag;
  double min = BIG, tour_cost;
  int n      = rcp->G->n,
      trials = rcp->number_of_cycles;

  int count_opt = 0,
      count_onepercent = 0,
      // last_best,
      // last_1pc,
      cc = 0;

  cycle_delete(rcp->C);
  cycle_init(rcp->C, n);

  cycle C_tmp;
  cycle_init(&C_tmp, n);
  flag = 0;
  for (i = 0; i < trials; i++) {

    if ((rand() % 100) / 100. > (1.*i / trials)) {
      // printf("thread %d : %d\n", rcp->th_no, i);
      cc++;
      status = generate_random_cycle(rcp->G, &C_tmp, &tour_cost);
      status = heur_2_opt(rcp->G, &C_tmp, tour_cost, &tour_cost);
      // always returns successfully

      if (flag == 0 && status == SUCCESS) {
        cycle_copy(&C_tmp, rcp->C);
        min = tour_cost;
        flag = 1;
      }
      else if (flag == 1 && status == SUCCESS && tour_cost < min) {
        cycle_copy(&C_tmp, rcp->C);
        min = tour_cost;
        //printf("%d : %f . ratio = %f\n", i, tour_cost, ((double)cc)/i);
        printf("%d : %f\n", i, tour_cost);
        count_opt++;
        // last_best = i;
      }

      if (tour_cost == min) {
        count_onepercent++;
        // last_1pc = i;
        //getchar();
      }

    } //else { printf("********** %d\n", i);getchar();}

  }

  /*printf("++ %f . ratio = %f\n", min, ((double)cc)/i);
  printf("recap : count_opt = %d, count_onepercent = %d\n", count_opt, count_onepercent);
  printf("last_best = %d, last_1pc = %d\n", last_best, last_1pc);
  getchar();*/

  cycle_delete(&C_tmp);
  if (flag == 1) {
    rcp->ub = min;
    rcp->return_status = SUCCESS;
  }
  else {
    rcp->return_status = FAILURE;
  }

  return NULL;
}

int heur_nearest_neighbour(graph *G, cycle* C, int start_node, double* ub) {
  double min, cost;
  int k, i, v_min, curr, some_forced_edges, flag;

  int n = G->n;


  cycle_delete(C);
  cycle_init(C, n);


  int* visited = (int*)calloc(n, sizeof(int));
  // visited[v] is equal to 1 if the vertex v has been visited,
  // otherwise visited[v] is equal to 0

  curr = start_node;
  visited[start_node] = 1;


  // Main loop: at each step a new vertex is visited and a new edge
  // is added to the solution.
  // Forced edges take priority on free edges, independently of their costs.
  // Forbidden edges cannot be chosen.
  // If at some certain point there is no possibility than choose
  // a forbidden edge, then the procedure stops and returns FAILURE.
  for (k = 0; k < n-1; k++) {

    some_forced_edges = 0;
    for (i = 0; i < n; i++) {
      if (visited[i] == 0) {
        if (graph_get_edge_constr(G, curr, i) == FORCED) {
          some_forced_edges = 1;
        }
      }
    }

    if (some_forced_edges == 0) {

      flag = 0;
      v_min = 0;
      for (i = 0; i < n; i++) {
        if (visited[i] == 0                                &&
            graph_get_edge_constr(G, curr, i) != FORBIDDEN   ) {
          cost = graph_get_edge_cost(G, curr, i);
          if (flag == 0) {
            v_min = i;
            min = cost;
            flag = 1;
          }
          else if (flag == 1 && cost < min) {
            v_min = i;
            min = cost;
          }
        }
      }

    }
    else {
      flag = 0;
      v_min = 0;
      for (i = 0; i < n; i++) {
        if (visited[i] == 0 && graph_get_edge_constr(G, curr, i) == FORCED) {
          cost = graph_get_edge_cost(G, curr, i);
          if (flag == 0) {
            v_min = i;
            min = cost;
            flag = 1;
          }
          else if (flag == 1 && cost < min) {
            v_min = i;
            min = cost;
          }
        }
      }

    }


    if (v_min > 0) {
      C->nodes[k] = curr;
      visited[v_min] = 1;
      curr = v_min;
    }
    else {
      free(visited);
      return FAILURE;
    }

  }

  if (graph_get_edge_constr(G, curr, start_node) != FORBIDDEN) {
    C->nodes[n-1] = curr;
  }
  else {
    free(visited);
    return FAILURE;
  }

  int v1      = -1,
      v2      = -1,
      index_1 = -1;
  for (i = 0; i < n; i++) {
    if (C->nodes[i] == 1) {
      v1 = C->nodes[(i+1)%n];
      v2 = C->nodes[(i-1+n)%n];
      index_1 = i;
      break;
    }
  }

  // fill in cycle with costs
  for (i = 0; i < n; i++) {
    C->costs[i] = graph_get_edge_cost(G, C->nodes[i], C->nodes[(i+1)%n]);
  }

  if (v1 > v2) {
    // We do this only to take into account some possibile
    // rounding errors /comparison errors with respect to the costs
    // of the 1-trees computed by compute_ot...
    // maybe this is not necessary.
    // compute the cost of the revese cycle
    *ub = 0.0;
    for (i = 0; i < n; i++) {
      *ub += C->costs[(index_1-i+n)%n];
    }
  }
  else {
    *ub = 0.0;
    for (i = 0; i < n; i++) {
      *ub += C->costs[(index_1+i)%n];
    }
  }

  free(visited);
  return SUCCESS;
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
int heur_2_opt(graph *G, cycle *C, double ccost, double* ub) {

  int i, j, v, z, h, k, l;
  double delta;
  double cost = ccost;

  int n = C->n;
  int changed = 1;

  //int MAX_ITER = 100;
  int counter = 0;

  while (changed /*&& counter < MAX_ITER*/) {
    counter++;
    changed = 0;


    for (i = 0; i < n ; i++) {

      for (j = i+2; j < n; j++) {
        // i+2: We don't want something like i-->i+1=j-->j+1

        if ((j+1) % n != i) {
          // We don't want something like j-->j+1=i-->i+1
          // (this happens only if i=0 and j=n-1 so that n-1-->0-->1).

          v = C->nodes[i];
          z = C->nodes[(i+1) % n];
          h = C->nodes[j];
          k = C->nodes[(j+1) % n];

          // We are guaranteed that v, z, h, k are all different vertices.
          delta = graph_get_edge_cost(G, v, h) +
                  graph_get_edge_cost(G, z, k) -
                  //graph_get_edge_cost(G, v, z) -
                  //graph_get_edge_cost(G, h, k);
                  C->costs[i] -
                  C->costs[j];


          if (delta < 0.0) {

              cost += delta;
              
              // Reverse the portion of the cycle which goes from
              // index i+1 to index j;
              // From ..., node[i]=v,  node[i+1]=z,  node[i+2]=v_1,  ...,
              //    node[j-1]=v_s,  node[j]=h,  node[j+1]=k, ...
              // To   ..., node[i]=v,  node[i+1]=h,  node[i+2]=v_s,  ...,
              //    node[j-1]=v_1,  node[j]=z,  node[j+1]=k, ...
              k = (i+1) % n; //0;
              /*for (l = (i+1)%n; l != (j+1)%n; l = (l+1)%n) {
                tmp_nodes[k] = C->nodes[l];
                k++;
              }

              k = k-1;
              for (l = (i+1)%n; l != (j+1)%n; l = (l+1)%n) {
                C->nodes[l] = tmp_nodes[k];
                k--;
              }*/
              int tmp;
              for (l = 0; l <= (j - k) / 2; ++l) {

                tmp             = C->nodes[k + l];
                C->nodes[k + l] = C->nodes[j - l];
                C->nodes[j - l] = tmp;

                C->costs[i + l] = graph_get_edge_cost(G, C->nodes[i + l], C->nodes[(i+l+1) % G->n]);
                C->costs[j - l] = graph_get_edge_cost(G, C->nodes[j - l], C->nodes[(j-l+1) % G->n]);

              }

              changed = 1;
              i = j = n;

          }
        }
      }
    } // end for

  } // end while


  int v1, v2, index_1;
  for (i = 0; i < n; i++) {
    if ((*C).nodes[i] == 1) {
      v1 = C->nodes[(i+1) % n];
      v2 = C->nodes[(i-1+n) % n];
      index_1 = i;
      break;
    }
  }

  // fill in cycle with costs
  for (i = 0; i < n; i++) {
    C->costs[i] = graph_get_edge_cost(G, C->nodes[i], C->nodes[(i+1)%n]);
  }

  if (v1 > v2) { // to take into account rounding errors, maybe not necessary
    *ub = 0.0;
    for (i = 0; i < n; i++) {
      *ub += C->costs[(index_1-i+n)%n];
    }
  }
  else {
    *ub = 0.0;
    for (i = 0; i < n; i++) {
      *ub += C->costs[(index_1+i)%n];
    }
  }

  *ub = cost;

  return SUCCESS;
}

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
int generate_random_cycle(graph *G, cycle *C, double* ub) {
  int remaining, target, tmp;

  int n = G->n;


  cycle_delete(C);
  cycle_init(C, n);

  cycle WORK_CYCLE;
  cycle_init(&WORK_CYCLE, G->n);

  int j;
  int nodes[n];
  for (j = 0; j < G->n; ++j) {
    nodes[j] = j;
  }

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
  double cost = 0.;
  for (j = 0; j < G->n; ++j) {
    WORK_CYCLE.nodes[j] = nodes[j];
    WORK_CYCLE.costs[j] += graph_get_edge_cost(G, nodes[j], nodes[(j+1) % G->n]);
    cost += WORK_CYCLE.costs[j];
  }

  // cost = heur2opt(G, &WORK_CYCLE, cost);

  cycle_copy(&WORK_CYCLE, C);
  *ub = cost;

  return SUCCESS;
}

/**
 * Compute an upper bound for the solution using a genetic algorithm
 * 
 * @param  G the graph
 * @param  C the cycle containing the best solution found
 * @return   cost of the returned solution
 */
double genetic(graph *G, cycle *C) {
  double cost = 0.;

  int i, j;
  int n = G->n;
  int iterations,
      iterations_wo_improvement = 0;

  cycle c,
        best_cycle;
  cycle_init(&c, n);
  cycle_init(&best_cycle, n);

  // reset
  cycle_delete(C);
  cycle_init(C, n);

  int population = 30;
  int tour[population][n];

  double fitness[population],
         norm_fitness[population], // normalized f., probability to be chosen
         costs[population];

  double best_fitness,
         best_cost;
  int best_index;

  double crossover_prob,
         mutation_prob;

  int target, remaining, tmp;

  // initialize
  for (i = 0; i < n; ++i) {
    for (j = 0; j < population; ++j) {
      tour[j][i] = i+1;
    }
  }

  for (i = 0; i < population; ++i) {

    // generate permutation of the node
    remaining = n;
    for (j = 0; j < n; ++j) {

      target = rand() % remaining;

      tmp                 = tour[i][j];
      tour[i][j]          = tour[i][target + j];
      tour[i][target + j] = tmp;

      remaining--;

    }

  }

  best_index = population + 1;
  best_fitness = 0.;

  for (i = 0; i < population; ++i) {
    costs[i] = 0.;
    for (j = 0; j < n-1; ++j) {
      costs[i] += graph_get_edge_cost(G, tour[i][j], tour[i][j+1]);
    }
    costs[i]  += graph_get_edge_cost(G, tour[i][n], tour[i][0]);
    fitness[i] = 1. / costs[i];

    if (fitness[i] > best_fitness) {
      best_fitness = fitness[i];
      best_cost    = costs[i];
      best_index   = i;
    }

  }

  // normalize probabilities
  for (i = 0; i < population; ++i) {
    norm_fitness[i] = fitness[i] / best_fitness;
  }

  int cp1, cp2; // crossover points




  return cost;
}
