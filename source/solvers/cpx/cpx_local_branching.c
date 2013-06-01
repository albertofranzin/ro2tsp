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

  double total_time_limit = 60., // seconds
         node_time_limit  = 5.,  // s
         elapsed_time     = 0.;  // s
  clock_t start_time, now;

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
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching.c: ");
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
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching.c :: ");
    fprintf(stderr, "failed to set the upper cutoff, error %d.\n", status);
    exit(1);
  }

  printf("Set lower bound to CPLEX: %f\n", ts->init_lb);
  status = CPXsetdblparam(env, CPX_PARAM_CUTLO, ts->init_lb);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching.c :: ");
    fprintf(stderr, "failed to set the lower cutoff, error %d.\n", status);
    exit(1);
  }

  // timeout
  printf("Set node time limit to CPLEX: %f\n", node_time_limit);
  status = CPXsetdblparam(env, CPX_PARAM_TILIM, node_time_limit);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching.c :: ");
    fprintf(stderr, "failed to set the timeout, error %d.\n", status);
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
  int dv        = 0,
      dv_max    = 100000, // ????
      lcr;                // last constraint row

  // constraint pools:
  // round1_cpool contains the constraints that have not been proved to make
  // the problem infeasible. When the problem is found to be infeasible,
  // then the last constraint is supposed to be the culprit, and it is
  // taken out from round1_cpool, reversed and inserted into round2_cpool.
  // I'm not sure this is going to work, hope this does not fuck up everything.
  cpx_cpool *round1_cpool = malloc(sizeof(round1_cpool)),
            *round2_cpool = malloc(sizeof(round2_cpool));
  int cid_counter = 0; // constraints ID counter
  //    round1_last_cid, // ID of the most recent constraint in round1_cpool
  //    round2_last_cid; // ID of the most recent constraint in round2_cpool

  cpx_cpool_init(round1_cpool);
  cpx_cpool_init(round2_cpool);

  // radius of the neighbourhood - the k = 10, 20 of the paper
  int    radius = 20;

  double rhs        = BIG,
         tl         = BIG,
         ub         = BIG,//te->init_ub,
         best_ub    = BIG;//te->init_ub;
  double x_opt[numcols],
         cur_x_opt[numcols];

  memset(x_opt, 0., sizeof(x_opt));
  // memset(cur_x_opt, 0., sizeof(cur_x_opt));
  memcpy(&cur_x_opt, &ms_values, sizeof(ms_values));

  // set time controls
  start_time = clock();

  // cycle begins!
  while (!termination) {

    if (rhs < BIG) {
      printf("creating constraint\n");
      cpx_constraint *cpxc = cpx_create_lb_constraint(cur_x_opt,
                                                      numcols,
                                                      cid_counter++,
                                                      'L',
                                                      rhs);
      printf("done\n");
      status = cpx_add_lb_constraint(env, lp, cpxc, pars);
      assert(status == 0);
      printf("asserted!\n");
    }

    tl = fmin(tl, total_time_limit - elapsed_time);

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
        fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching.c :: ");
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
          fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
          fprintf(stderr, "CPXgetx : %d\n", status);
          fprintf(stderr, "Failed to get node solution.\n");
          exit(1);
        }
        // add SECs for the current solution
        numsubtrs = cpx_add_secs(env, lp, x, numcols, &hash_table, pars);
      }


    }  // end while num of subtours != 1

    tl = node_time_limit;

    switch(solstat) {
      case 101: // optimal solution found
      case 102: // optimal solution found within tolerance
                status = CPXgetobjval(env, lp, &objval);
                if (status) {
                  fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
                  fprintf(stderr, "case %d :: CPXgetobjval : %d\n", solstat, status);
                  fprintf(stderr, "Failed to obtain objective value.\n");
                  exit(1);
                }
                printf("objval = %f\n", objval);
                if (objval < best_ub) {
                  best_ub = objval;
                  memcpy(&x_opt, &x, sizeof(x));
                }
                if (rhs >= BIG) {
                  termination = TRUE;
                  break;
                }
                diversify = FALSE;
                first     = FALSE;
                ub        = objval;
                rhs       = radius;
                memcpy(&cur_x_opt, &x, sizeof(x));
                // reverse last constraint
                lcr = CPXgetnumrows(env, lp);
                char asense[1];
                asense[0] = 'G';
                int inds[1];
                inds[0] = lcr-1;
                status = CPXchgsense(env, lp, 1, inds, asense);
                if (status) {
                  fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
                  fprintf(stderr, "case 101 :: CPXchgsense : %d\n", status);
                  fprintf(stderr, "Failed to invert constraint.\n");
                  exit(1);
                }
                double arhs[1];
                arhs[0] = rhs + 1;
                status = CPXchgrhs(env, lp, 1, inds, arhs);
                if (status) {
                  fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
                  fprintf(stderr, "case 101 :: CPXchgrhs : %d\n", status);
                  fprintf(stderr, "Failed to invert constraint.\n");
                  exit(1);
                }
                break;

      case 103: // problem is infeasible
                if (rhs >= BIG) {
                  termination = TRUE;
                  break;
                }
                // reverse last local branching constraint
                if (round1_cpool->size > 0) {
                  /*cpx_constraint *outc = cpx_cpool_pop_last(round1_cpool);
                  outc->rhs[0] = outc->rhs[0]+1;
                  if (outc->sense[0] == 'L') {
                    outc->sense[0] = 'G';
                  } else {
                    outc->sense[0] = 'L';
                  }
                  cpx_cpool_insert(round2_cpool, outc);*/
                  lcr = CPXgetnumrows(env, lp);
                  char asense[1];
                  asense[0] = 'G';
                  int inds[1];
                  inds[0] = lcr-1;
                  status = CPXchgsense(env, lp, 1, inds, asense);
                  if (status) {
                    fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
                    fprintf(stderr, "case 103 :: CPXchgsense : %d\n", status);
                    fprintf(stderr, "Failed to invert constraint.\n");
                    exit(1);
                  }
                  double arhs[1];
                  arhs[0] = rhs + 1;
                  status = CPXchgrhs(env, lp, 1, inds, arhs);
                  if (status) {
                    fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
                    fprintf(stderr, "case 103 :: CPXchgrhs : %d\n", status);
                    fprintf(stderr, "Failed to invert constraint.\n");
                    exit(1);
                  }
                }
                if (diversify) {
                  ub = BIG;
                  tl = BIG;
                  dv++;
                  first = TRUE;
                }
                rhs = rhs + ceil(radius / 2);
                diversify = TRUE;
                break;

      case 107: // problem has timed out, a feasible solution has been found
                if (rhs < BIG) {
                  /*printf("rhs < BIG\n");
                  if (round1_cpool->size > 0) {
                    cpx_constraint *outc = cpx_cpool_pop_last(round1_cpool);
                  }
                  printf("last popped\n");*/
                  lcr = CPXgetnumrows(env, lp);
                  // printf("# of rows: %d\n", lcr);
                  if (first) {
                    // printf("ciao\n");
                    status = CPXdelrows(env, lp, lcr-1, lcr-1);
                    if (status) {
                      fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
                      fprintf(stderr, "case 107 :: CPXdelrows : %d\n", status);
                      fprintf(stderr, "Failed to delete constraint.\n");
                      exit(1);
                    }
                  } else {
                    // don't know what that 'refine' is,
                    // so I follow the advice and do NOT execute
                    // the following 'else' branch
                    /*char asense[1];
                    asense[0] = 'G';
                    int inds[1];
                    inds[0] = lcr-1;
                    status = CPXchgsense(env, lp, 1, inds, asense);
                    if (status) {
                      fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
                      fprintf(stderr, "case 107 :: CPXchgsense : %d\n", status);
                      fprintf(stderr, "Failed to invert constraint.\n");
                      exit(1);
                    }
                    int arhs[1];
                    arhs[0] = rhs + 1;
                    status = CPXchgrhs(env, lp, 1, inds, arhs);
                    if (status) {
                      fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
                      fprintf(stderr, "case 107 :: CPXchgrhs : %d\n", status);
                      fprintf(stderr, "Failed to invert constraint.\n");
                      exit(1);
                    }*/
                  } // end if !first
                } // end if rhs < BIG
                // refine ?
                printf("try to get value\n");
                status = CPXgetobjval(env, lp, &objval);
                if (status) {
                  fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
                  fprintf(stderr, "case 107 :: CPXgetobjval : %d\n", status);
                  fprintf(stderr, "Failed to obtain objective value.\n");
                  exit(1);
                }
                // printf("eh?\n");
                if (objval < best_ub) {
                  best_ub = objval;
                  memcpy(&x_opt, &x, sizeof(x));
                }
                // printf("mah\n");
                first      = FALSE;
                diversify  = FALSE;
                ub  = objval;
                rhs = radius;
                // printf("boh\n");
                memcpy(&cur_x_opt, &x, sizeof(x));
                // printf("già\n");
                break;

      case 108: // problem infeasible, no feasible solutions found
                printf("diversify = %d\n", diversify);
                if (diversify) {
                  /*if (round1_cpool->size > 0) {
                    cpx_constraint *outc = cpx_cpool_pop_last(round1_cpool);
                    outc->rhs[0] = outc->rhs[0]+1;
                    if (outc->sense[0] == 'L') {
                      outc->sense[0] = 'G';
                    } else {
                      outc->sense[0] = 'L';
                    }
                    cpx_cpool_insert(round2_cpool, outc);*/
                    lcr = CPXgetnumrows(env, lp);
                    char asense[1];
                    asense[0] = 'G';
                    int inds[1];
                    inds[0] = lcr-1;
                    status = CPXchgsense(env, lp, 1, inds, asense);
                    if (status) {
                      fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
                      fprintf(stderr, "case 108 :: CPXchgsense : %d\n", status);
                      fprintf(stderr, "Failed to invert constraint.\n");
                      exit(1);
                    }
                    double arhs[1];
                    arhs[0] = rhs + 1;
                    status = CPXchgrhs(env, lp, 1, inds, arhs);
                    if (status) {
                      fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
                      fprintf(stderr, "case 108 :: CPXchgrhs : %d\n", status);
                      fprintf(stderr, "Failed to invert constraint.\n");
                      exit(1);
                    }
                  //}
                  ub = BIG;
                  tl = BIG;
                  dv++;
                  rhs = rhs + ceil(radius / 2);
                  first = TRUE;
                } else {
                  /*if (round1_cpool->size > 0) {
                    cpx_constraint *outc = cpx_cpool_pop_last(round1_cpool);
                  }*/
                    lcr = CPXgetnumrows(env, lp);
                    status = CPXdelrows(env, lp, lcr-1, lcr-1);
                    if (status) {
                      fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
                      fprintf(stderr, "case 108 :: CPXdelrows : %d\n", status);
                      fprintf(stderr, "Failed to delete constraint.\n");
                      exit(1);
                    }
                  rhs = rhs - ceil(radius / 2);
                }
                diversify = TRUE;
                break;
    } // end switch

    // check time
    now = clock();
    elapsed_time = time_elapsed(start_time, now);
    if (elapsed_time > total_time_limit || dv > dv_max) {
      termination = TRUE;
    }

    status = CPXaddmipstarts(env, lp, 1, numcols, beg,
        ms_indices, x_opt, NULL, NULL);

  } // end while termination

  printf("Set node time limit to CPLEX: %f\n", node_time_limit);
  status = CPXsetdblparam(env, CPX_PARAM_TILIM, node_time_limit);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching.c :: ");
    fprintf(stderr, "failed to set the timeout, error %d.\n", status);
    exit(1);
  }

  status = CPXmipopt(env, lp);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching.c :: ");
    fprintf(stderr, "CPXmipopt : %d\n", status);
    fprintf(stderr, "Error while solving the problem.\n");
    exit(1);
  }

  // Retrieve objective function final value.
  status = CPXgetobjval(env, lp, &objval);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
    fprintf(stderr, "CPXgetobjval : %d\n", status);
    fprintf(stderr, "Failed to obtain objective value.\n");
    //exit(1);
  }

  ts->z_opt = objval;
  printf("# problem solved!\n");

  assert(numcols == CPXgetnumcols(env, lp));

  solstat = CPXgetstat(env, lp);
  if (solstat == 101 ||
      solstat == 102 ||
      solstat == 103) {
    opt = TRUE;
  } else {
    opt = FALSE;
  }

  if (solstat == 101 ||
      solstat == 102) {
    status = CPXgetx(env, lp, x_opt, 0, numcols-1);
    if (status) {
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_local_branching :: ");
      fprintf(stderr, "CPXgetx : %d\n", status);
      fprintf(stderr, "Failed to get node solution.\n");
      exit(1);
    }
  }

  // Save tour...
  graph_delete(&te->G_OUTPUT); graph_init(&te->G_OUTPUT, n);

  for (k = 0; k < numcols; k++) {

    if (x_opt[k] > 0.9) {
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
