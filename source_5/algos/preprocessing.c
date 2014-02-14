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
  int    status, i, n = G->n;

  int *zeros = malloc(n*(n-1)*sizeof(int)/2),
      *ones  = malloc(n*(n-1)*sizeof(int)/2);
  // initially, all zeros - just saves time later
  for (i = 0; i < n*(n-1)/2; ++i) {
    zeros[i] = 1;
  }
  memset(ones, 0, n*(n-1)*sizeof(int)/2);

  graph_copy(G, &te->G_INPUT); graph_copy(G, &te->G_CURR);




  //----------------------------------------------------------------
  // Compute an initial upper-bound.
 
  cycle TOUR_RC2OPT, TOUR_NN2OPT;
  cycle_init(&TOUR_RC2OPT, 0); cycle_init(&TOUR_NN2OPT, 0);

  // Random Cycles + 2opt.
  status = compute_upper_bound(&te->G_CURR, &TOUR_RC2OPT,
			        RANDOM_CYCLES_2OPT, &ub,
              &ones, &zeros);

  ts->rc2opt_ub = ub;
  printf("# preprocessing : upper-bound : heur. rc+2opt = %f\n", ts->rc2opt_ub);

  // Nearest Neighbour + 2opt.
  status = compute_upper_bound(&te->G_CURR, &TOUR_NN2OPT,
			       NEAREST_NEIGHBOUR_2_OPT, &ub,
             &ones, &zeros);

  ts->nn2opt_ub = ub;
  printf("# preprocessing : upper-bound : heur. nn+2opt = %f\n", ts->nn2opt_ub);

  // Dumb.
  status = compute_upper_bound(&te->G_CURR, NULL, DUMB, &ub,
                                 &ones, &zeros);

  ts->dumb_ub = ub;
  printf("# preprocessing : upper-bound : dumb          = %f\n", ts->dumb_ub);

  // Select the best upper-bound as the initial upper-bound.
  if (ts->nn2opt_ub < ts->rc2opt_ub) {

    te-> init_ub = ts->init_ub = ts->nn2opt_ub;

    cycle_copy(&TOUR_NN2OPT, &te->TOUR_INIT);

  }
  else {

    te-> init_ub = ts->init_ub = ts->rc2opt_ub;

    cycle_copy(&TOUR_RC2OPT, &te->TOUR_INIT);

  }
  printf("# preprocessing : initial upper-bound         = %f\n", ts->init_ub);

  te->dumb_ub = ts->dumb_ub;




  //----------------------------------------------------------------
  // Compute an initial lower-bound.

  status = compute_lagrange(&te->G_CURR, &te->OT_CURR, te->init_ub, &lb);

  if (INTEGER_ROUNDING_OPT == ENABLED) {

    if (lb - floor(lb) > SAFE_ROUNDING_THRESHOLD) lb = ceil(lb);
    else                       ts->num_fails_integer_rounding++;

  }

  te->init_lb = ts->init_lb = lb;
  printf("# preprocessing : initial lower-bound         = %f\n", ts->init_lb);




  //----------------------------------------------------------------
  // Problem reduction: discard fat edges.

  printf("# preprocessing : gap                         = %f %%\n",
	 (ts->init_ub - ts->init_lb)/(ts->init_lb) * 100);


  /* ... insert code ... */




  cycle_delete(&TOUR_RC2OPT);
  cycle_delete(&TOUR_NN2OPT);

  return status;
}
