#include "cpx_mip_refine.h"

/**
 * MIP-and-refine heuristic
 * get a feasible solution and refine it
 *
 * @param  te   problem environment
 * @param  ts   problem stats
 * @param  pars user parameters
 * @return      status of the operation
 */
int cpx_mip_refine(tsp_env    *te,
                   tsp_stats  *ts,
                   parameters *pars) {

 int i, j, k, status, solstat;

  CPXENVptr env      = NULL;
  CPXLPptr  lp       = NULL;
  char     *probname = "problema";

  double total_time_limit = 60.; // seconds
         // node_time_limit  = 5.,  // s
         // elapsed_time     = 0.;  // s
  double objval;
  // clock_t start_time, now;

  status = cpx_create_problem(&env, &lp, probname);
  assert(status == 0);

#ifdef DEBUG
  if (pars->verbosity >= ESSENTIAL) {
    printf("Setting up CPLEX screen indicator\n");
    // Turn on output to the screen.
    status = CPXsetintparam(env, CPX_PARAM_SCRIND, CPX_ON);
    // Turn off output to the screen.
    //status = CPXsetintparam(env, CPX_PARAM_SCRIND, CPX_OFF);
    if (status != 0) {
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_mip_refine :: ");
      fprintf(stderr, "failed to turn on screen indicator, error %d.\n", status);
      exit(1);
    }
    printf("done\n");
  }

  // Turn on data checking
  //status = CPXsetintparam (env, CPX_PARAM_DATACHECK, CPX_ON);
#endif

  // -------------------------------------------------------------
  // problem set-up
  // -------------------------------------------------------------

  int n       = te->G_CURR.n;
  int numcols = n * (n - 1) / 2;
  cpx_table hash_table;

  cpx_table_init(&hash_table, numcols);
  cpx_table_populate(&hash_table, &te->G_CURR);

  status = cpx_setup_problem(env, lp, &te->G_CURR, &hash_table, pars);
  assert(status == 0);

  // --------------------------------------------------------------
  // cplex setup
  // --------------------------------------------------------------

  // upper and lower bounds
  printf("Set upper bound to CPLEX: %f\n", te->init_ub);
  status = CPXsetdblparam(env, CPX_PARAM_CUTUP, te->init_ub);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_mip_refine :: ");
    fprintf(stderr, "failed to set the upper cutoff, error %d.\n", status);
    exit(1);
  }

  printf("Set lower bound to CPLEX: %f\n", ts->init_lb);
  status = CPXsetdblparam(env, CPX_PARAM_CUTLO, ts->init_lb);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_mip_refine :: ");
    fprintf(stderr, "failed to set the lower cutoff, error %d.\n", status);
    exit(1);
  }

  // timeout
  printf("Set node time limit to CPLEX: %f\n", total_time_limit);
  status = CPXsetdblparam(env, CPX_PARAM_TILIM, total_time_limit);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_mip_refine :: ");
    fprintf(stderr, "failed to set the timeout, error %d.\n", status);
    exit(1);
  }

  // start from a known solution
  status = CPXsetintparam(env, CPX_PARAM_ADVIND, CPX_ON);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_mip_refine :: ");
    fprintf(stderr, "failed in setting CPX_PARAM_ADVIND, error %d.\n", status);
    exit(1);
  }

  // set start solution
  int    ms_indices[numcols];
  double ms_values[numcols];

  memset(ms_values, 0., sizeof(ms_values));
  for (i = 0; i < n; ++i) {
    indx_from_vertices(&hash_table,
                       te->TOUR_OPT.nodes[i],
                       te->TOUR_OPT.nodes[(i+1) % n],
                       &k);
    ms_values[k] = 1.0;
  }

  for (i = 0; i < numcols; ++i) {
    ms_indices[i] = i;
  }

  int beg[1] = {0};
  status = CPXaddmipstarts(env, lp, 1, numcols, beg,
        ms_indices, ms_values, NULL, NULL);

  // -----------------------------

  // set polishing value
  // (here we want to start immediately with polishing)
  // chances are:
  // CPX_PARAM_POLISHAFTERINTSOL : start polish after given integer sol
  // CPX_PARAM_POLISHAFTERNODE : start polishing after given node
  // CPX_PARAM_POLISHAFTERTIME : start polishing after given time
  status = CPXsetintparam(env, CPX_PARAM_POLISHAFTERINTSOL, 1);

  // set RINS frequency
  status = CPXsetintparam(env, CPX_PARAM_RINSHEUR, 1);

  // -------------------------------------------------------------------

  // solve the MIP problem
  status = CPXmipopt(env, lp);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_mip_refine :: ");
    fprintf(stderr, "CPXmipopt : %d\n", status);
    fprintf(stderr, "Error while solving the problem.\n");
    exit(1);
  }

  // retrieve solution status: can we continue or not?
  solstat = CPXgetstat(env, lp);

#ifdef DEBUG
  if (pars->verbosity >= ESSENTIAL) {
    printf("solution status is %d\n", solstat);
    // getchar();
  }
#endif

  // Retrieve objective function final value.
  status = CPXgetobjval(env, lp, &objval);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_mip_refine :: ");
    fprintf(stderr, "CPXgetobjval : %d\n", status);
    fprintf(stderr, "Failed to obtain objective value.\n");
    //exit(1);
  }

  ts->z_opt = objval;
  printf("# problem solved!\n");

  assert(numcols == CPXgetnumcols(env, lp));

  double x[numcols];
  status = CPXgetx(env, lp, x, 0, numcols-1);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_mip_refine :: ");
    fprintf(stderr, "CPXgetx : %d\n", status);
    fprintf(stderr, "Failed to get node solution.\n");
    exit(1);
  }

  // Save tour...
  graph_delete(&te->G_OUTPUT); graph_init(&te->G_OUTPUT, n);

  for (k = 0; k < numcols; k++) {

    if (x[k] > 0.9) {

      vertices_from_indx(&hash_table, &i, &j, k);

#ifdef DEBUG
      if (pars->verbosity >= USEFUL) {
        printf("(%d,%d)\n", i, j);
      }
#endif

      graph_insert_edge(&te->G_OUTPUT, i, j, graph_get_edge_cost(&te->G_INPUT, i, j), FREE);

    }
  }

  return status;
}
