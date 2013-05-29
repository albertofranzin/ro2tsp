#include "cpx_local_branching.h"

/**
 * [cpx_local_branching description]
 * @param  te   [description]
 * @param  ts   [description]
 * @param  pars [description]
 * @return      [description]
 */
int cpx_local_branching(tsp_env    *te,
                        tsp_stats  *ts,
                        parameters *pars) {

  int i, j, k, status;

  CPXENVptr env      = NULL;
  CPXLPptr  lp       = NULL;
  char     *probname = "problema";

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
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c: ");
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

  status = cpx_setup_problem(env, lp, &te->G_CURR, &hash_table);
  assert(status == 0);

  // --------------------------------------------------------------
  // cplex setup
  // --------------------------------------------------------------

  // upper and lower bounds
  printf("Set upper bound to CPLEX: %f\n", te->init_ub);
  status = CPXsetdblparam(env, CPX_PARAM_CUTUP, te->init_ub);
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

  // timeout
  double time_limit = 12.; // seconds
  printf("Set time limit to CPLEX: %f\n", time_limit);
  status = CPXsetdblparam(env, CPX_PARAM_TILIM, time_limit);
  if ( status != 0 ) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c :: ");
    fprintf(stderr, "failed to set the timeout, error %d.\n", status);
    exit(1);
  }

  // --------------------------------------------------------------
  // 
  // solver : main cycle
  // 
  // --------------------------------------------------------------

  // leave callbacks out for now
  // leave kruskal secs out for now

  // repeat following cycle until problem is solved:
  // * try to solve the problem:
  //   + solve the relaxated problem
  //   + look for subtours, add relative constraints
  //   + iterate until problem is solved, unfeasible or timed-out
  // * add local branching constraint
  // * iterate until problem is solved, timed-out, or infeasible
  // * based on the solution status, add/remove local branching constraints,
  //   and iterate
  int    termination = FALSE, // cycle control
         solstat;             // solution status code
                              // (see appendix B for explanation)
  double x[numcols],          // solution coefficients
         objval;              // solution value (cost)

  // parameters for directing local branching
  int opt       = TRUE,
      first     = TRUE,
      diversify = FALSE;
  int dv        = 0;

  // constraint pools:
  // round1_cpool contains the constraints that have not been proved to make
  // the problem infeasible. When the problem is found to be infeasible,
  // then the last constraint is supposed to be the culprit, and it is
  // taken out from round1_cpool, reversed and inserted into round2_cpool.
  // I'm not sure this is going to work, hope this does not fuck up everything.
  cpx_cpool *round1_cpool = malloc(sizeof(round1_cpool)),
            *round2_cpool = malloc(sizeof(round2_cpool));
  int cid_counter = 0, // constraints ID counter
      round1_last_cid, // ID of the most recent constraint in round1_cpool
      round2_last_cid; // ID of the most recent constraint in round2_cpool

  cpx_cpool_init(round1_cpool);
  cpx_cpool_init(round2_cpool);

  // radius of the neighbourhood - the k = 10, 20 of the paper
  int    radius = 20;

  double rhs        = BIG,
         tl         = BIG,
         current_ub = te->init_ub,
         ub         = te->init_ub;
  double x_opt[numcols],
         cur_x_opt[numcols];

  // cycle begins!
  while (!termination) {

    int numsubtrs = 0;
    while (numsubtrs != 1) {

#ifdef DEBUG
    if (pars->verbosity >= ESSENTIAL) {
      printf("about to solve the problem\n");
    }
#endif

      // solve the MIP problem
      status = CPXmipopt(env, lp);
      if (status) {
        fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver.c :: ");
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

      // if no suitable solution is found, then stop here
      // else, retrieve current solution and follow on adding SECs
      if (solstat == 107 ||  // timed out, integer solution exists
          solstat == 108 ||  // timed out, integer solution does not exist
          solstat == 103   ) // problem infeasible
      {
        // terminate cycle
        numsubtrs = 1;
      } else if (solstat == 101 ||
                 solstat == 102   )
      {
        // retrieve solution coefficients
        status = CPXgetx(env, lp, x, 0, numcols-1);
        if (status) {
          fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver :: ");
          fprintf(stderr, "CPXgetx : %d\n", status);
          fprintf(stderr, "Failed to get node solution.\n");
          exit(1);
        }
        // add SECs for the current solution
        numsubtrs = cpx_add_secs(env, lp, x, numcols, &hash_table, pars);
      }


    }  // end while num of subtours != 1

    switch(solstat) {
      case 101: // optimal solution found
      case 102: // optimal solution found within tolerance
                status = CPXgetobjval(env, lp, &objval);
                if (status) {
                  fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver :: ");
                  fprintf(stderr, "CPXgetobjval : %d\n", status);
                  fprintf(stderr, "Failed to obtain objective value.\n");
                  exit(1);
                }
                printf("objval = %f\n", objval);
                termination = TRUE;
                break;

      case 103: // problem is infeasible
                if (rhs >= BIG) {
                  termination = TRUE;
                  // returns current  opt
                  break;
                }
                // reverse last local branching constraint
                if (round1_cpool->size > 0) {
                  cpx_constraint *outc = cpx_cpool_pop_last(round1_cpool);
                  outc->rhs = outc->rhs+1;
                  if (outc->sense == 'L') {
                    outc->sense = 'U';
                  } else {
                    outc->sense = 'L';
                  }
                  cpx_cpool_insert(round2_cpool, outc);
                }
                if (diversify) {
                  current_ub = BIG;
                  tl         = BIG;
                  dv++;
                  first = TRUE;
                }
                rhs = rhs + ceil(radius / 2);
                diversify = TRUE;
                break;

      case 107: // problem has timed out, a feasible solution has been found
                if (rhs < BIG) {
                  printf("rhs < BIG\n");
                  if (round1_cpool->size > 0) {
                    cpx_constraint *outc = cpx_cpool_pop_last(round1_cpool);
                  }
                  printf("last popped\n");
                  if (first) {
                    // remove constraint from model
                  } else {
                    // add reversed constraint to round2_cpool
                    // ma non ho capito se devo farlo o meno...
                  }
                }
                // refine ?
                status = CPXgetobjval(env, lp, &objval);
                if (status) {
                  fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver :: ");
                  fprintf(stderr, "CPXgetobjval : %d\n", status);
                  fprintf(stderr, "Failed to obtain objective value.\n");
                  exit(1);
                }
                if (objval < ub) {
                  ub = objval;
                  printf("before memcpy 1\n");
                  memcpy(&x_opt, &x, sizeof(x));
                  printf("after memcpy 1\n");
                }
                first      = FALSE;
                diversify  = FALSE;
                current_ub = objval;
                rhs        = radius;
                printf("before memcpy 2\n");
                memcpy(&cur_x_opt, &x, sizeof(x));
                printf("after memcpy 2\n");
                break;

      case 108: // problem infeasible, no feasible solutions found
                printf("diversify = %d\n", diversify);
                if (diversify) {
                  if (round1_cpool->size > 0) {
                    cpx_constraint *outc = cpx_cpool_pop_last(round1_cpool);
                    outc->rhs = outc->rhs+1;
                    if (outc->sense == 'L') {
                      outc->sense = 'U';
                    } else {
                      outc->sense = 'L';
                    }
                    cpx_cpool_insert(round2_cpool, outc);
                  }
                  current_ub = BIG;
                  tl = BIG;
                  dv++;
                  rhs = rhs + ceil(radius / 2);
                  first = TRUE;
                } else {
                  if (round1_cpool->size > 0) {
                    cpx_constraint *outc = cpx_cpool_pop_last(round1_cpool);
                  }
                  rhs = rhs - ceil(radius / 2);
                  diversify = TRUE;
                }
                break;
    }

  } // end while termination

  // Retrieve objective function final value.
  status = CPXgetobjval(env, lp, &objval);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solver :: ");
    fprintf(stderr, "CPXgetobjval : %d\n", status);
    fprintf(stderr, "Failed to obtain objective value.\n");
    //exit(1);
  }

  ts->z_opt = objval;
  printf("# problem solved!\n");

  assert(numcols == CPXgetnumcols(env, lp));

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

  return SUCCESS;
}
