#include "preprocessing.h"

/**
 * preprocess the tsp instance, computing upper and lower bounds
 * on the solution, and initializing problem statistics
 * @param G    the graph
 * @param pars parameters of the problem
 * @param te   environment structure
 * @param ts   problem stats
 * @return     status of preprocessing
 */
int preprocessing(graph      *G,
                  parameters *pars,
                  tsp_env    *te,
                  tsp_stats  *ts)
{
  double ub, lb;
  int    status;

  cycle CYCLE;
  cycle_init(&CYCLE, 0);

  // Set G_CURR to be a copy of the input graph.
  //graph_copy(&te->G_INPUT, G);
  graph_copy(G, &te->G_CURR);
  graph_copy(G, &te->G_INPUT);

  // Compute a naive upper bound that will be used when our heuristic is
  // not able to find any upper bound and any tour with no forbidden edges.
  // This upper bound will be a valid upper bound to the cost of
  // an optimal tour (with no forbidden edges) for every configuration
  // of constraints on the edges of the graph
  // (in fact, it is the sum of the first n edges of the input graph
  // with highest costs).

  // Naive.
  status = compute_upper_bound(&te->G_CURR, NULL, DUMB, &ub);

  ts->dumb_ub = ub;
  printf("# node %d : upper bound : dumb            = %f\n",
          te->curr_call, ts->dumb_ub);

  // Compute an upper bound of the cost of the optimal tour.
  // The procedure will always successfully returns, since
  // the graph is complete and no forbidden (nor forced) edges
  // are currently present.

  // Random Cycles.
  status = compute_upper_bound(&te->G_CURR, &CYCLE, RANDOM_CYCLES, &ub);

  ts->rc_ub = ub;
  printf("# node %d : upper bound : heur. rc        = %f\n",
          te->curr_call, ts->rc_ub);

  // Random Cycles + 2OPT.
  status = compute_upper_bound(&te->G_CURR, &CYCLE, RANDOM_CYCLES_2OPT, &ub);

  ts->rc2opt_ub = ub;
  printf("# node %d : upper bound : heur. rc + 2opt = %f\n",
          te->curr_call, ts->rc2opt_ub);

  // Nearest Neighbour.
  status = compute_upper_bound(&te->G_CURR, &CYCLE, NEAREST_NEIGHBOUR, &ub);

  ts->nn_ub = ub;
  printf("# node %d : upper bound : heur. nn        = %f\n",
          te->curr_call, ts->nn_ub);

  // Nearest Neighbour + 2OPT.
  status = compute_upper_bound(&te->G_CURR, &CYCLE,
                                  NEAREST_NEIGHBOUR_2_OPT, &ub);

  ts->nn2opt_ub = ub;
  printf("# node %d : upper bound : heur. nn + 2opt = %f\n",
          te->curr_call, ts->nn2opt_ub);

  // Set the upper-bound(s)
  te->dumb_ub = ts->dumb_ub;
  ub = ts->nn2opt_ub < ts->rc2opt_ub ?
        ts->nn2opt_ub :
        ts->rc2opt_ub;

  // We assume that, in the case in which compute_upper_bound
  // successfully returns, then OT_CURR is a tour and ub is the cost
  // of that tour.
  // We set the initial incumbent equal to the upper bound...
  te->incumbent = te->z_opt = ub;

  ts->init_ub = ub;
  printf("# node %d : initial upper bound           = %f\n",
          te->curr_call, ts->init_ub);

  //printf("# node %d : initial incumbent             = %f\n",
  //      te->curr_call, ts->init_ub);

  // and the optimal solution TOUR_OPT equal to the tour just computed.
  cycle_copy(&CYCLE, &te->TOUR_OPT);

  // The procedure will always successfully returns,
  // since the graph is complete and no forbidden 
  // (nor forced) edges are currently present.
  status = compute_lagrange(&te->G_CURR, &te->OT_CURR, ub, &lb);

  //onetree_plot(OT_CURR, &te->EG_INPUT, "Lagr. 1-tree");


  // Discard fat edges.
  if (FAT_EDGES_OPT == ENABLED) {
    //graph_remove_fat_edges(te, ts, ub);
  }

  if (INTEGER_ROUNDING_OPT == ENABLED) {

    if (lb - floor(lb) > SAFE_ROUNDING_THRESHOLD) {
      lb = ceil(lb);
    } else {
      ts->num_fails_integer_rounding++;
    }

  }

  te->z_curr = lb;

  ts->init_lb = lb;
  printf("# node %d : initial lower bound           = %f\n",
          te->curr_call, ts->init_lb);

  te->input_ub = ub;
  printf("upper bound: %f\n", te->input_ub);

  printf("gap = %f %%\n", (ub - lb)/lb * 100);

#ifdef DEBUG
  //char ch = getchar();
#endif

  return status;
}
