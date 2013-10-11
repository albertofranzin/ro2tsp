#include "cpx_preprocessing.h"


int cpx_preprocessing(CPXENVptr   env,
                      CPXLPptr    lp,
                      cpx_env    *ce,
                      cpx_stats  *cs,
                      parameters *pars)
{

  double ub, lb;
  int    status;
  clock_t  time_start_heurs, time_end_heurs;

  time_start_heurs = clock();

  graph_copy(&ce->G_INPUT, &ce->G);
  cycle TOUR_RC2OPT, TOUR_NN2OPT;
  cycle_init(&TOUR_RC2OPT, 0); cycle_init(&TOUR_NN2OPT, 0);


  // Random Cycles + 2opt //
  if (ce->pars->heuristic_algo == RANDOM_CYCLES      ||
      ce->pars->heuristic_algo == RANDOM_CYCLES_2OPT ||
      ce->pars->heuristic_algo == ALL                  ) {
    status = compute_upper_bound(&ce->G, &TOUR_RC2OPT,
                                 RANDOM_CYCLES_2OPT, &ub);
    
    if (status) {
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_preprocessing.c:\n"
                      "function: cpx_preprocessing:\n"
                      "compute_upper_bound (RANDOM_CYCLES_2OPT): %d\n", status);
      return status;
    }

    cs->rc2opt_ub = ub;
    printf("# preprocessing : upper-bound : heur. rc+2opt = %f\n", cs->rc2opt_ub);
  } else {
    cs->rc2opt_ub = BIG;
  }

  // Nearest Neighbour + 2opt //
  if (ce->pars->heuristic_algo == NEAREST_NEIGHBOUR       ||
      ce->pars->heuristic_algo == NEAREST_NEIGHBOUR_2_OPT ||
      ce->pars->heuristic_algo == ALL                       ) {
    status = compute_upper_bound(&ce->G, &TOUR_NN2OPT,
                                 NEAREST_NEIGHBOUR_2_OPT, &ub);
    if (status) {
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_preprocessing.c:\n"
                      "function: cpx_preprocessing:\n"
                      "compute_upper_bound (NEAREST_NEIGHBOUR_2OPT): %d\n", status);
      return status;
    }

    cs->nn2opt_ub = ub;
    printf("# preprocessing : upper-bound : heur. nn+2opt = %f\n", cs->nn2opt_ub);
  } else {
    cs->nn2opt_ub = BIG;
  }

  // select the best upper bound as the initial upper bound //

  if (cs->nn2opt_ub < cs->rc2opt_ub) {
    ce->init_ub = cs->init_ub = cs->nn2opt_ub;
    cycle_copy(&TOUR_NN2OPT, &ce->TOUR_HEUR);
  } else {
    ce-> init_ub = cs->init_ub = cs->rc2opt_ub;
    cycle_copy(&TOUR_RC2OPT, &ce->TOUR_HEUR);
  }
   
  //ce->init_ub = cs->init_ub = 640;

  printf("# preprocessing : initial upper-bound         = %f\n", cs->init_ub);


  status = CPXsetdblparam(env, CPX_PARAM_CUTUP, ce->init_ub);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_preprocessing.c:\n"
                    "function: cpx_preprocessing:\n"
                    "CPXsetdblparam (CPX_PARAM_CUTUP): %d\n", status);
    return status;
  }

  time_end_heurs = clock();
  cs->heur_time = (double)(time_end_heurs - time_start_heurs)/CLOCKS_PER_SEC;


  //----------------------------------------------------------------
  // Compute an initial lower-bound.

  time_start_heurs = clock(); // use same var also for lagrangian
  status = compute_lagrange(&ce->G, &ce->OT, ce->init_ub, &lb);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_preprocessing.c:\n"
                    "function: cpx_preprocessing:\n"
                    "compute_lagrange: %d\n", status);
    return status;
  }
  ce->init_lb = cs->init_lb = lb;
  printf("# preprocessing : initial lower-bound         = %f\n", cs->init_lb);

  time_end_heurs = clock();
  cs->lb_time = (double)(time_end_heurs - time_start_heurs)/CLOCKS_PER_SEC;  


  //----------------------------------------------------------------
  // Problem reduction: discard fat edges.

  int n       = ce->G.n;
  int numcols = n * (n - 1) / 2;
  int i, j, idx, cnt;

  int    *indices;
  char   *lu;
  double *bd;

  indices = (int *)calloc(numcols, sizeof(int));
  lu      = (char *)calloc(numcols, sizeof(char));
  bd      = (double *)calloc(numcols, sizeof(double));

  
  /**
   * ERROR HERE
   */
  status = compute_deltas(&ce->G, &ce->OT);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_preprocessing.c:\n"
                    "function: cpx_preprocessing:\n"
                    "compute_deltas: %d\n", status);
    return status;
  }


  cnt = 0;
  for (idx =  0; idx < numcols; idx++) {
    vrtx_from_idx(&ce->T, &i, &j, idx);
    if (ce->init_lb + graph_get_edge_delta(&ce->G, i, j) > ce->init_ub) {
      indices[cnt] = idx;
      lu[cnt]      = 'B';
      bd[cnt]      = 0.0;
      cnt++;
    }
  }

  status = CPXchgbds(env, lp, cnt, indices, lu, bd);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_preprocessing.c:\n"
                    "function: cpx_preprocessing:\n"
                    "CPXchgbds: %d\n", status);
    return status;
  }

  printf("# preprocessing : discarded fat edges         = %d (%2.2f%%)\n",
            cnt, 100.*cnt/numcols);


  cycle_delete(&TOUR_RC2OPT);
  cycle_delete(&TOUR_NN2OPT);

  free(indices);
  free(lu);
  free(bd);

  return 0;

}
