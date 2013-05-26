#include "cpx_solver.h"

/**
 * [cpx_solver description]
 * @param te    problem environment
 * @param ts    problem stats
 * @param pars  user parameters
 */
void cpx_solver(tsp_env *te, tsp_stats *ts, parameters *pars) {
  int i, j, k, pos;

  int status;
  CPXENVptr env = NULL;
  CPXLPptr lp = NULL;
  char* probname = "problema";

  graph G;
  graph_init(&G, 0);
  graph_copy(&te->G_INPUT, &G);


#ifdef DEBUG
  if (pars->verbosity >= ESSENTIAL) {
    printf("Creating problem... ");
  }
#endif

  status = cpx_create_problem(&env, &lp, probname);
  assert(status == 0);

#ifdef DEBUG
  if (pars->verbosity >= ESSENTIAL) {
    printf("done\n");
  }
#endif


#ifdef DEBUG
  if (pars->verbosity >= ESSENTIAL) {
    printf("Setting up CPLEX screen indicator\n");
    /* Turn on output to the screen */
    status = CPXsetintparam(env, CPX_PARAM_SCRIND, CPX_ON);
    if (status != 0) {
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c: ");
      fprintf(stderr, "failed to turn on screen indicator, error %d.\n", status);
      exit(1);
    }
    printf("done\n");
  }

  /* Turn on data checking */
  //status = CPXsetintparam (env, CPX_PARAM_DATACHECK, CPX_ON);
#endif


  int n = G.n;
  int numcols = n * (n - 1) / 2;

  printf("creating cpx_table\n");
  cpx_table hash_table;
  cpx_table_init(&hash_table, numcols);
  printf("done. Populating...\n");
  cpx_table_populate(&hash_table, &G);
  printf("done\n");

#ifdef DEBUG
  if (pars->verbosity >= ESSENTIAL) {
    printf("Populating problem... ");fflush(stdout);
  }
#endif

  status = cpx_setup_problem(env, lp, &G, &hash_table);
  assert(status == 0);

#ifdef DEBUG
  if (pars->verbosity >= ESSENTIAL) {
    printf("done\n");
  }

  if (pars->verbosity >= VERBOSE) {
    char *prob_filename = NULL;
    asprintf(&prob_filename, "%s%s", probname, ".lp");
    printf("Saving problem to file %s...\n", prob_filename);
    status = CPXwriteprob(env, lp, prob_filename, NULL);
    if (status) {
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c:\n");
      fprintf(stderr, "Failed to write problem to file %s, error %d.\n",
                prob_filename, status);
      exit(1);
    } else {
      printf("File saved.\n");
    }
    free(prob_filename);
  }
#endif

  printf("Set upper bound to CPLEX: %f\n", te->input_ub);
  status = CPXsetdblparam(env, CPX_PARAM_CUTUP, te->input_ub);
  if ( status != 0 ) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c :: ");
    fprintf(stderr, "failed to set the upper cutoff, error %d.\n", status);
    exit(1);
  }

  printf("Set lower bound to CPLEX: %f\n", ts->init_lb);
  status = CPXsetdblparam(env, CPX_PARAM_CUTLO, ts->init_lb);
  if ( status != 0 ) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c :: ");
    fprintf(stderr, "failed to set the lower cutoff, error %d.\n", status);
    exit(1);
  }

  // CPX_PARAM_EPAGAP
  status = CPXsetdblparam(env, CPX_PARAM_EPGAP, 0.0000000001);
  if ( status != 0 ) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c :: ");
    fprintf(stderr, "failed to set the upper cutoff, error %d.\n", status);
    exit(1);
  }

  ////////////////////////////////////

  // Assure linear mappings between the presolved and original models
  status = CPXsetintparam (env, CPX_PARAM_PRELINEAR, CPX_OFF);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c :: ");
    fprintf(stderr, "CPXsetintparam :: CPX_PARAM_PRELINEAR : %d\n", status);
    fprintf(stderr, "Failed in setting CPX_PARAM_PRELINEAR.\n");
    exit(1);
  }

  // Turn on traditional search for use with control callbacks
  status = CPXsetintparam (env, CPX_PARAM_MIPSEARCH, CPX_MIPSEARCH_TRADITIONAL);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c :: ");
    fprintf(stderr, "CPXsetintparam :: CPX_PARAM_MIPSEARCH : %d\n", status);
    fprintf(stderr, "Failed in setting CPX_PARAM_MIPSEARCH.\n");
    exit(1);
  }

  // Let MIP callbacks work on the original model
  status = CPXsetintparam (env, CPX_PARAM_MIPCBREDLP, CPX_OFF);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c :: ");
    fprintf(stderr, "CPXsetintparam :: CPX_PARAM_MIPCBREDLP : %d\n", status);
    fprintf(stderr, "Failed in setting CPX_PARAM_MIPCBREDLP.\n");
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

    // set lazy constraint callback function
  // Use a lazy constraint because it gets called when an integer-feasible
  // solution is found.
  // This does not happen with setusercutcallbackfunc, which can be called
  // when there is a non-integer-feasible solution, thus not allowing us
  // to solve the problem.
  status = CPXsetlazyconstraintcallbackfunc(env, cpx_subtour_callback, &ci);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c :: ");
    fprintf(stderr, "CPXsetlazyconstraintcallbackfunc : %d\n", status);
    fprintf(stderr, "Failed in setting the lazy constraint callback.\n");
    exit(1);
  }

  //////////////////////////////////////////////////////////



  onetree OT;
  onetree_init(&OT, 0);
  onetree_copy(&te->OT_CURR, &OT);

  // ------------------------------------------------------------------
  // UPPER-BOUND
  
  double ub, lb;
  ub = te->input_ub;
  /*cycle C;
  cycle_init(&C, 0);
  printf("# compute an upper-bound...\n");
  compute_upper_bound(&G, &C, NEAREST_NEIGHBOUR_2_OPT, &ub);
  cycle_delete(&C);
  printf("# upper-bound = %f\n", ub);*/
  
  // ------------------------------------------------------------------


  // ------------------------------------------------------------------
  // LAGRANGE

  // compute_lagrange(&G, &OT, ub, &lb);

  // ------------------------------------------------------------------


  // ------------------------------------------------------------------
  // MINIMUM ONE TREE
  /*
  compute_ot(&G, &OT);
  */

  // ------------------------------------------------------------------


  // ------------------------------------------
  // FAT EDGES REDUCTION

  int    edg_inds[n * (n - 1) / 2];
  char   lu[n * (n - 1) / 2];
  double bd[n * (n - 1) / 2];
  int ind, numfatedgs;

  /*status = compute_deltas(&G, &OT);

  numfatedgs = 0;
  for (i = 1; i <= n; i++) {
    for (j = i+1; j <= n; j++) {
      if (lb + graph_get_edge_delta(&G, i, j) > ub) {

        indx_from_vertices(&hash_table, i, j, &ind);
        edg_inds[numfatedgs] = ind-1;
        lu[numfatedgs] = 'U';
        bd[numfatedgs] = 0.0;
        numfatedgs++;
     
      }
    }
  }

  status = CPXchgbds(env, lp, numfatedgs, edg_inds, lu, bd);
  printf("status = %d\n", status);

  printf("# fat edges removed = %d\n", numfatedgs);*/

  // ------------------------------------------



  // ------------------------------------------
  // KRUSKAL SECS

  cpx_add_kruskal_secs(env, lp, &hash_table, &OT, pars);

  // ------------------------------------------

  onetree_delete(&OT);

#ifdef DEBUG
  if (pars->verbosity >= ESSENTIAL) {
    printf("about to solve the problem\n");
  }
#endif

  status = CPXmipopt(env, lp);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c :: ");
    fprintf(stderr, "CPXmipopt : %d\n", status);
    fprintf(stderr, "Error while solving the problem.\n");
    exit(1);
  }

  int solstat = CPXgetstat(env, lp);
  // 101 == CPXMIP_OPTIMAL
  // 102 == CPXMIP_OPTIMAL_TOL
  // assert(solstat == 101 || solstat == 102);

#ifdef DEBUG
  if (pars->verbosity >= ESSENTIAL) {
    printf ("Solution status %d.\n", solstat);
  }
#endif

  // variabili per memorizzazione soluzione
  /*int numsubtrs;

  numsubtrs = 0;

  while (numsubtrs != 1) {

#ifdef DEBUG
  if (pars->verbosity >= ESSENTIAL) {
    printf("about to solve the problem\n");
  }
#endif

    status = CPXmipopt(env, lp);
    if (status) {
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c :: ");
      fprintf(stderr, "CPXmipopt : %d\n", status);
      fprintf(stderr, "Error while solving the problem.\n");
      exit(1);
    }

    int solstat = CPXgetstat(env, lp);
    // 101 == CPXMIP_OPTIMAL
    // 102 == CPXMIP_OPTIMAL_TOL
    // assert(solstat == 101 || solstat == 102);

#ifdef DEBUG
  if (pars->verbosity >= ESSENTIAL) {
    printf ("Solution status %d.\n", solstat);
  }
#endif

    numsubtrs = cpx_add_secs(env, lp, &hash_table, &G, pars);

  }*/

  double objval;
  double x[numcols];

  // retrieve objective function final value
  status = CPXgetobjval(env, lp, &objval);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver :: ");
    fprintf(stderr, "CPXgetobjval : %d\n", status);
    fprintf(stderr, "Failed to obtain objective value.\n");
    //exit(1);
  }

  printf("# problem solved!\n");
  printf("# opt cost = %f\n", objval);

  numcols = CPXgetnumcols(env, lp);

  status = CPXgetx(env, lp, x, 0, numcols-1);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver :: ");
    fprintf(stderr, "CPXgetx : %d\n", status);
    fprintf(stderr, "Failed to get node solution.\n");
    exit(1);
  }



  // save tour in the output graph
  graph_delete(&te->G_OUTPUT); graph_init(&te->G_OUTPUT, n);

  for (k = 0; k < numcols; k++) {

    if (x[k] > 0.9) {

      vertices_from_indx(&hash_table, &i, &j, k+1);

#ifdef DEBUG
      if (pars->verbosity >= USEFUL) {
        printf("(%d,%d)\n", i, j);
      }
#endif

      graph_insert_edge(&te->G_OUTPUT, i, j, graph_get_edge_cost(&G, i, j), FREE);

    }
  }

  return;
}
