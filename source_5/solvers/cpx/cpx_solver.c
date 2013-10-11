#include "cpx_solver.h"

void cpx_solver(cpx_env    *ce,
                cpx_stats  *cs,
                parameters *pars) {

  CPXENVptr env;
  CPXLPptr  lp;

  int i, j, idx;
  int status = 0;

  // make cplex table from graph
  cpx_table_init(&ce->T, &ce->G_INPUT);

  // create and setup problem environment
  status = cpx_create_problem(&env, &lp, "problem");
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c:\n"
                    "function: cpx_solver:\n"
                    "cpx_create_problem : %d\n", status);
    exit(1);
  }

#ifdef DEBUG
  if (ce->pars->verbosity >= ANNOYING) {
    printf("solvers/cpx/cpx_solver.c :: before entering cpx_setup_problem\n");
  }
#endif

  status = cpx_setup_problem(env, lp, ce);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c:\n"
                    "function: cpx_solver:\n"
                    "cpx_setup_problem : %d\n", status);
    exit(1);
  }

#ifdef DEBUG
  if (ce->pars->verbosity >= ANNOYING) {
    printf("solvers/cpx/cpx_solver.c :: before entering cpx_set_problem_parameters\n");
  }
#endif

  status = cpx_set_problem_parameters(env, lp, ce);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c:\n"
                    "function: cpx_solver:\n"
                    "cpx_set_problem_parameters : %d\n", status);
    exit(1);
  }

#ifdef DEBUG
  if (ce->pars->verbosity >= ANNOYING) {
    printf("solvers/cpx/cpx_solver.c :: before entering cpx_preprocessing\n");
  }
#endif

  status = cpx_preprocessing(env, lp, ce, cs, pars);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c:\n"
                    "function: cpx_solver:\n"
                    "cpx_preprocessing : %d\n", status);
    exit(1);
  }

  // write problem to file
  status = CPXwriteprob(env, lp, "myprob.lp", "LP");
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c:\n"
                    "function: cpx_solver:\n"
                    "CPXwriteprob : %d\n", status);
    exit(1);
  }


  // call the chosen solver, among
  // - iterative cplex
  // - cplex with callbacks
  // - matheuristic
  int    solstat;
  int    n       = ce->G.n,
         numcols = n * (n - 1) / 2;
  double *x;

  clock_t time_start, time_end;

  x = (double *)calloc(numcols, sizeof(double));

  ce->mylp = lp;

  time_start = clock();
  if (ce->pars->cplex_callbacks == TRUE) {
    status = cpx_solve_miliotis(env, lp, ce, cs, x, n * (n - 1) / 2, &solstat);
  } else {
    status = cpx_solve_iterative(env, lp, ce, cs, x, n * (n - 1) / 2, &solstat);
  }
  //status = cpx_solve_proximity(env, lp, ce, cs, x, n * (n - 1) / 2, &solstat);
  time_end = clock();
  cs->cpx_time = (double)(time_end - time_start)/CLOCKS_PER_SEC;

  // set up output, retrieving edges whose corresponding variable
  // is close enough to 1 (tolerance should be much higher, but...)
  graph_init(&ce->G_OUTPUT, n);
  for (idx = 0; idx < n * (n - 1) / 2; idx++) {
    if (x[idx] > 0.9) {
      vrtx_from_idx(&ce->T, &i, &j, idx);
      graph_insert_edge(&ce->G_OUTPUT, i, j, 0.0, FREE);
      cs->z_opt += graph_get_edge_cost(&ce->G_INPUT, i, j);
    }
  }

  free(x);

}
