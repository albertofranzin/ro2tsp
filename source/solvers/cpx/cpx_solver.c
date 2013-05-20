#include "cpx_solver.h"

/**
 * [cpx_solver description]
 * @param G  input graph of the problem
 * @param H  output graph containing the optimal tour
 * @param te problem environment
 * @param ts problem stats
 */
void cpx_solver(tsp_env *te, tsp_stats *ts, parameters *pars) {
  int i, j, k, pos;

  int       status;
  CPXENVptr env      = NULL;
  CPXLPptr  lp       = NULL;
  char     *probname = "problema";

  graph G;
  graph_init(&G, 0);
  graph_copy(&te->G_INPUT, &G);


#ifdef DEBUG
  if (pars->verbosity >= ESSENTIAL) {
    printf("Creating problem... ");
  }
#endif

  status = cpx_create_problem(&env, &lp, probname, pars);
  assert(status == 0);

#ifdef DEBUG
  if (pars->verbosity >= ESSENTIAL) {
    printf("done\n");
  }
#endif


#ifdef DEBUG
  if (pars->verbosity >= ESSENTIAL) {
    /* Turn on output to the screen */
    status = CPXsetintparam (env, CPX_PARAM_SCRIND, CPX_ON);
    if ( status != 0 ) {
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c:\n");
      fprintf (stderr, "Failed to turn on screen indicator, error %d.\n", status);
      exit(1);
    }
  }

  /* Turn on data checking */
  //status = CPXsetintparam (env, CPX_PARAM_DATACHECK, CPX_ON);
#endif


  int n = G.n;
  int numcols = n * (n - 1) / 2;

  cpx_table hash_table;
  cpx_table_init(&hash_table, numcols);
  cpx_table_populate(&hash_table, &G);

#ifdef DEBUG
  if (pars->verbosity >= ESSENTIAL) {
    printf("Populating problem... ");
  }
#endif

  status = cpx_setup_problem(env, lp, &G, &hash_table, pars);
  assert(status == 0);

#ifdef DEBUG
  if (pars->verbosity >= ESSENTIAL) {
    printf("done\n");
  }
#endif

  printf("Set upper bound to CPLEX: %f\n", te->input_ub);
  status = CPXsetdblparam(env, CPX_PARAM_CUTUP, te->input_ub);
  if ( status != 0 ) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c:\n");
    fprintf (stderr, "Failed to set the upper cutoff, error %d.\n", status);
    exit(1);
  }

  printf("Set lower bound to CPLEX: %f\n", ts->init_lb);
  status = CPXsetdblparam(env, CPX_PARAM_CUTLO, ts->init_lb);
  if ( status != 0 ) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c:\n");
    fprintf (stderr, "Failed to set the lower cutoff, error %d.\n", status);
    exit(1);
  }

  // cutinfo for passing parameters to the callback
  cutinfo ci;
  ci.lp  = lp;
  ci.x   = (double *) malloc (numcols * sizeof (double));
  ci.beg = (int *)    malloc (11 * sizeof (int));
  ci.ind = (int *)    malloc (numcols * sizeof (int));
  ci.val = (double *) malloc (numcols * sizeof (double));
  ci.rhs = (double *) malloc (10 * sizeof (double));
  ci.pars            = pars;
  ci.numcols         = numcols;
  ci.hash_table      = hash_table;
  ci.number_of_nodes = n;

  /**
   * set parameters for callbacks
   */

  // Assure linear mappings between the presolved and original models
  status = CPXsetintparam (env, CPX_PARAM_PRELINEAR, CPX_OFF);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c :: \n");
    fprintf(stderr, "CPXsetintparam :: CPX_PARAM_PRELINEAR : %d\n", status);
    printf("Failed in setting CPX_PARAM_PRELINEAR.\n");
    exit(1);
  }

  // Turn on traditional search for use with control callbacks
  status = CPXsetintparam (env, CPX_PARAM_MIPSEARCH,
                                CPX_MIPSEARCH_TRADITIONAL);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c :: \n");
    fprintf(stderr, "CPXsetintparam :: CPX_PARAM_MIPSEARCH : %d\n", status);
    printf("Failed in setting CPX_PARAM_MIPSEARCH.\n");
    exit(1);
  }

  // Let MIP callbacks work on the original model
  status = CPXsetintparam (env, CPX_PARAM_MIPCBREDLP, CPX_OFF);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c :: \n");
    fprintf(stderr, "CPXsetintparam :: CPX_PARAM_MIPCBREDLP : %d\n", status);
    printf("Failed in setting CPX_PARAM_MIPCBREDLP.\n");
    exit(1);
  }

  // set lazy constraint callback function
  // Use a lazy constraint because it gets called when an integer-feasible
  // solution is found.
  // This does not happen with setusercutcallbackfunc, which can be called
  // when there is a non-integer-feasible solution, thus not allowing us
  // to solve the problem.
  status = CPXsetlazyconstraintcallbackfunc(env, cpx_subtour_callback, &ci);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c :: \n");
    fprintf(stderr, "CPXsetlazyconstraintcallbackfunc : %d\n", status);
    printf("Failed in setting the lazy constraint callback.\n");
    exit(1);
  }

  // add Kruskal-like constraints
  //status = cpx_mark_subtours_the_kruskal_way(env, lp, G,
  //                                           &hash_table, n, pars);
  //assert(status == 0);


  /**
   * solve the problem
   */

  // variabili per memorizzazione soluzione
  int    cur_numcols;
  double objval;
  double solstat;
  double x[numcols];

  int edge_indices[n];

#ifdef DEBUG
  if (pars->verbosity >= ESSENTIAL) {
    printf("about to solve the problem\n");
  }
#endif

  status = CPXmipopt(env, lp);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c :: \n");
    fprintf(stderr, "CPXmipopt : %d\n", status);
    fprintf(stderr, "Error while solving the problem.\n");
    exit(1);
  }

  solstat = CPXgetstat(env, lp);
  // 101 == CPXMIP_OPTIMAL
  // 102 == CPXMIP_OPTIMAL_TOL
  assert(solstat == 101 || solstat == 102);

#ifdef DEBUG
  if (pars->verbosity >= ESSENTIAL) {
    printf ("Solution status %f.\n", solstat);
  }
#endif

  // retrieve objective function final value
  status = CPXgetobjval (env, lp, &objval);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver :: \n");
    fprintf(stderr, "CPXgetobjval : %d\n", status);
    fprintf(stderr,"Failed to obtain objective value.\n");
    exit(1);
  }

  printf("Problem solved!\n");
  printf("Solution cost is: %f\n", objval);

  /**
   * try to retrieve optimal tour graph
   */

  cur_numcols = CPXgetnumcols(env, lp);
  status  = CPXgetx(env, lp, x, 0, numcols-1);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver :: \n");
    fprintf(stderr, "CPXgetx : %d\n", status);
    fprintf(stderr, "Failed to get node solution.\n");
    exit(1);
  }

  k = 0;
  for (i = 0; i < cur_numcols; i++) {
    if (x[i] > 0.9) {
      // oppure x[i] == 1.0 dopo aver arrotondato le x[i] all'intero più vicino
      edge_indices[k] = i+1;

#ifdef DEBUG
      if (pars->verbosity >= USEFUL) {
        printf("%d ", edge_indices[k]);
      }
#endif

      k++;
    }
  }
#ifdef DEBUG
  if (pars->verbosity >= USEFUL) {
    printf("\n");
  }
#endif

  // save tour in the output graph
  graph_delete(&te->G_OUTPUT);
  graph_init(&te->G_OUTPUT, n);
  for (pos = 1; pos <= cur_numcols; pos++) {

    if (x[pos-1] > 0.9) {
      vertices_from_pos(&hash_table, &i, &j, pos);

#ifdef DEBUG
      if (pars->verbosity >= USEFUL) {
        printf("(%d,%d)\n", i, j);
      }
#endif

      graph_insert_edge(&te->G_OUTPUT, i, j,
                        graph_get_edge_cost(&G, i, j), FREE);
    }
  }

}
