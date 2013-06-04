#include "cpx_solver.h"

/**
 * [cpx_solver description]
 * @param te    problem environment
 * @param ts    problem stats
 * @param pars  user parameters
 */
int cpx_solver(tsp_env *te, tsp_stats *ts, parameters *pars) {

  int i, j, k, status;

  CPXENVptr env = NULL;
  CPXLPptr lp = NULL;
  char* probname = "problema";


  // --------------------------------------------------------------
  // Create cplex-problem.

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
    // Turn on output to the screen.
    status = CPXsetintparam(env, CPX_PARAM_SCRIND, CPX_ON);
    // Turn off output to the screen.
    //status = CPXsetintparam(env, CPX_PARAM_SCRIND, CPX_OFF);
    if (status != 0) {
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c: ");
      fprintf(stderr, "failed to turn on screen indicator, error %d.\n", status);
      exit(1);
    }
    printf("done\n");
  }

  // Turn on data checking
  status = CPXsetintparam (env, CPX_PARAM_DATACHECK, CPX_ON);
#endif



  // --------------------------------------------------------------
  // Create a conversion table.

  int n       = te->G_CURR.n;
  int numcols = n * (n - 1) / 2;
  cpx_table hash_table;

  double objval;
  double x[numcols];

  printf("creating cpx_table\n");

  cpx_table_init(&hash_table, numcols);
  printf("done. Populating...\n");

  cpx_table_populate(&hash_table, &te->G_CURR);
  printf("done\n");




  // --------------------------------------------------------------
  // Fill in cplex-problem with data.

#ifdef DEBUG
  if (pars->verbosity >= ESSENTIAL) {
    printf("Populating problem... ");fflush(stdout);
  }
#endif

  status = cpx_setup_problem(env, lp, &te->G_CURR, &hash_table, pars);
  assert(status == 0);

#ifdef DEBUG
  if (pars->verbosity >= ESSENTIAL) {
    printf("done\n");
  }
  if (pars->verbosity >= VERBOSE) {
    char *prob_filename = NULL;
    if (asprintf(&prob_filename, "%s%s", probname, ".lp") == -1) {
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver :: ");
      fprintf(stderr, "error while allocating memory\n");
      exit(1);
    }
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


  // local branching
  /*status = CPXsetintparam(env, CPX_PARAM_LBHEUR, CPX_ON);
  assert(status == 0);*/


  // --------------------------------------------------------------
  // Set cplex upper-cutoff.
  
  /*printf("Set upper bound to CPLEX: %f\n", te->init_ub);
  status = CPXsetdblparam(env, CPX_PARAM_CUTUP, te->init_ub);
  if ( status != 0 ) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c :: ");
    fprintf(stderr, "failed to set the upper cutoff, error %d.\n", status);
    exit(1);
  }*/
  
  // --------------------------------------------------------------
  // Set cplex lower-cutoff.
  /**/
  printf("Set lower bound to CPLEX: %f\n", ts->init_lb);
  status = CPXsetdblparam(env, CPX_PARAM_CUTLO, ts->init_lb);
  if ( status != 0 ) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c :: ");
    fprintf(stderr, "failed to set the lower cutoff, error %d.\n", status);
    exit(1);
  }
  /**/




  // --------------------------------------------------------------
  // Set cplex absolute gap tolerance.
  
  status = CPXsetdblparam(env, CPX_PARAM_EPGAP, 0.0000000001);
  if ( status != 0 ) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c :: ");
    fprintf(stderr, "failed to set the upper cutoff, error %d.\n", status);
    exit(1);
  }


  // start from a known solution
  status = CPXsetintparam(env, CPX_PARAM_ADVIND, CPX_ON);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching.c :: ");
    fprintf(stderr, "failed in setting CPX_PARAM_ADVIND, error %d.\n", status);
    exit(1);
  }

  // set start solution
  /**/int    ms_indices[numcols];
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

  printf("setting RINS frequency to 50\n");
  status = CPXsetintparam(env, CPX_PARAM_RINSHEUR, 50);/**/


  //
  // set up callbacks
  //

  if (pars->cplex_callbacks) {
    // --------------------------------------------------------------
    // Ensure linear mappings between the presolved and original models

    // Perform only linear reduction:
    status = CPXsetintparam (env, CPX_PARAM_PRELINEAR, CPX_OFF);
    // Perform full reductions:
    //status = CPXsetintparam (env, CPX_PARAM_PRELINEAR, CPX_ON);
    if (status) {
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c :: ");
      fprintf(stderr, "CPXsetintparam :: CPX_PARAM_PRELINEAR : %d\n", status);
      fprintf(stderr, "Failed in setting CPX_PARAM_PRELINEAR.\n");
      exit(1);
    }




    // --------------------------------------------------------------
    // Turn on traditional search for use with control callbacks

    // Apply traditional branch & cut strategy:
    status = CPXsetintparam (env, CPX_PARAM_MIPSEARCH, CPX_MIPSEARCH_TRADITIONAL);
    // Apply dynamic search:
    //status = CPXsetintparam (env, CPX_PARAM_MIPSEARCH, CPX_MIPSEARCH_DYNAMIC);
    if (status) {
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c :: ");
      fprintf(stderr, "CPXsetintparam :: CPX_PARAM_MIPSEARCH : %d\n", status);
      fprintf(stderr, "Failed in setting CPX_PARAM_MIPSEARCH.\n");
      exit(1);
    }

    // --------------------------------------------------------------
    // Let MIP callbacks work on the original model

    // Use the original model:
    status = CPXsetintparam (env, CPX_PARAM_MIPCBREDLP, CPX_OFF);
    // Use reduced, presolved model:
    //status = CPXsetintparam (env, CPX_PARAM_MIPCBREDLP, CPX_ON);
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
    

  } // end if use callbacks


  // --------------------------------------------------------------
  // Add Kruskal-like SECs to the model.

  cpx_add_kruskal_secs(env,
                       lp,
                       &hash_table,
                       &te->G_CURR,
                       &te->OT_CURR,
                       pars);


#ifdef DEBUG
  if (pars->verbosity >= ESSENTIAL) {
    printf("about to solve the problem\n");
  }
#endif

  if (pars->cplex_callbacks) {
    status = CPXmipopt(env, lp);
    if (status) {
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c :: ");
      fprintf(stderr, "CPXmipopt : %d\n", status);
      fprintf(stderr, "Error while solving the problem.\n");
      exit(1);
    }

  } else {  // do not use callbacks

    // variabili per memorizzazione soluzione
    /**/int numsubtrs;

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

      double x[numcols];
      status = CPXgetx(env, lp, x, 0, numcols-1);
      if (status) {
        fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver :: ");
        fprintf(stderr, "CPXgetx : %d\n", status);
        fprintf(stderr, "Failed to get node solution.\n");
        exit(1);
      }

      numsubtrs = cpx_add_secs(env, lp, x, numcols, &hash_table, pars);

    }  // end while

  } // end if !callbacks

  int solstat = CPXgetstat(env, lp);
  // 101 == CPXMIP_OPTIMAL
  // 102 == CPXMIP_OPTIMAL_TOL
  // assert(solstat == 101 || solstat == 102);

#ifdef DEBUG
  if (pars->verbosity >= ESSENTIAL) {
    printf ("Solution status %d.\n", solstat);
  }
#endif


  // --------------------------------------------------------------
  // Retrieve a solution.

  // Retrieve objective function final value.
  status = CPXgetobjval(env, lp, &objval);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver :: ");
    fprintf(stderr, "CPXgetobjval : %d\n", status);
    fprintf(stderr, "Failed to obtain objective value.\n");
    //exit(1);
  }

  ts->z_opt = objval;
  printf("# problem solved! cost = %f\n", objval);

  numcols = CPXgetnumcols(env, lp);

  status = CPXgetx(env, lp, x, 0, numcols-1);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver :: ");
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

  // ==============================================================
  // Turn off the callback.
  /*status = CPXsetlpcallbackfunc(env, NULL, NULL);
  if ( status ) {
    fprintf (stderr, "Failed to turn off callback function.\n");
  }*/


  return SUCCESS;
}
