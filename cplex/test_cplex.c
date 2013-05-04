#include "solvers/cplex_solver.h"
#include "algos/compute_upper_bound.h"
#include "data/cycle.h"


int main (int argc, char *argv[]) {

  int i, j, k;
  char* opt;

  parameters* pars = getParameters();

  /* ======================== */
  /* parse command line input */
  /* ======================== */


  for (i = 1; i < argc; i++) {

      opt = argv[i];

      if (strcmp(opt, "-n") == 0 || strcmp(opt, "--number") == 0)
          pars->number_of_nodes = atoi(argv[++i]);

      if (strcmp(opt, "-s") == 0 || strcmp(opt, "--seed") == 0)
          pars->seed = atol(argv[++i]);

      if (strcmp(opt, "-f") == 0 || strcmp(opt, "--file") == 0) {
          // will ignore other options
          pars->tsp_file_option = TRUE;
          pars->random_instance_option = FALSE;
          pars->tsp_file = argv[++i];
      }

      if (strcmp(opt, "-h") == 0 || strcmp(opt, "--help") == 0) {
          // print help and exit
          print_helper_menu();
          return 0;
      }

  }


  /* ================= */
  /* inizializza grafo */
  /* ================= */

  graph G;
  graph_init(&G, 1);

  egraph EG;
  egraph_init(&EG, pars->number_of_nodes);

  if (pars->tsp_file_option == TRUE) {
    read_tsp_from_file(&EG, pars);
    egraph_to_graph(&EG, &G);
    printf("@ Euclidean TSP\n# tsplib instance\n# number of nodes = %d\n# tsplib file = %s\n\n", pars->number_of_nodes, pars->tsp_file);
  }
  else if (pars->random_instance_option == TRUE) {
    if (pars->random_seed_option == FALSE || pars->seed >= 0) {
        srand(pars->seed);
    } else {
        srand(time(NULL));
    }

    egraph_random(&EG);
    egraph_to_graph(&EG, &G);
    printf("@ Euclidean TSP\n# random instance\n# number of nodes = %d\n# seed = %ld\n\n", pars->number_of_nodes, pars->seed);
  }


  printf("creating a cycle\n");
  cycle C;
  cycle_init(&C, 1);
  printf("cycle created\n");

  double heuristic_upper_bound;
  heuristic_upper_bound = compute_upper_bound(&G, &C, NEAREST_NEIGHBOUR);
  printf("@ Nearest Neighbour Heuristic\n# upper bound = %f\n", heuristic_upper_bound);

  /**/heuristic_upper_bound = heur2opt(&G, &C, heuristic_upper_bound);
  printf("@ 2-opt\nupper bound = %f\n", heuristic_upper_bound);/**/

  /////////////////////////////////////////////////////////////////
  //
  //   let the cplex part begin
  //
  /////////////////////////////////////////////////////////////////

  //
  // define variables
  //

  // status code returned by cplex callable library functions
  int status = 0;
  // cplex environment to pass to/from cplex callable library
  CPXENVptr env = NULL;
  // partial formulation
  CPXLPptr lp = NULL;
  // use for storing error messages returned by CPLEX
  char errmsg[1024];


  int NUMCOLS = (G.n * (G.n - 1)) / 2,
      NUMROWS = G.n,
      NUMNZ = G.n * (G.n - 1);

  // Declare pointers for the variables and arrays that will contain
  // the data which define the LP problem.  The cplex_setup_problem() routine
  // allocates space for the problem data.
  char    *probname = "tsp";
  int      numcols;
  int      numrows;
  int      objsen;
  double  *obj = NULL;
  double  *rhs = NULL;
  char    *sense = NULL;
  int     *matbeg = NULL;
  int     *matcnt = NULL;
  int     *matind = NULL;
  double  *matval = NULL;
  double  *lb = NULL;
  double  *ub = NULL;
  char    *ctype = NULL;

  // Declare and allocate space for the variables and arrays where we will
  // store the optimization results including the status, objective value,
  // variable values, and row slacks.
  int      solstat;
  double   objval;
  double   x[NUMCOLS];
  double   slack[NUMROWS];


  //
  // create problem
  //

  // if returns, means it's ok
  status = cplex_create_problem(&env, &lp, probname);
  assert(!status);

  printf("lp problem created\n");


  //
  // fill in problem data
  //

  cplex_table hash_table;
  cplex_table_init(&hash_table, NUMCOLS);
  cplex_table_populate(&hash_table, &G);

  status = cplex_setup_problem(&G, &hash_table, env, lp,
              probname, &numrows, &numcols, &objsen,
              &obj, &rhs, &sense,
              &matbeg, &matcnt, &matind, &matval,
              &lb, &ub, &ctype);
  assert(!status);

  status = CPXsetdblparam(env, CPX_PARAM_CUTUP, heuristic_upper_bound);
  if (status) {
    fprintf (stderr, "Failed to set an upper bound.\n");
    exit(1);
  }

  printf("problem filled in\n");


  //
  // solve
  //

  // iterate the following steps:
  // - solve the problem
  // - look at the cycle(s):
  //   - if there is only one cycle, we're done, and exit
  //   - else, parse the subcycles and add the apt SEC constraint

  // control for the solver cycle
  int iteration   = 1,          // # of iterations passed
      termination = FALSE,      // control for the inner cycle
      problem_solved = FALSE,   // control for the mail cycle
      cur_numrows,              // # of rows, as modified by new constraints
      cur_numcols;              // # of cols, as modified by new constraints

  // variables for (sub)cycle detection
  int     mark      = 1,       // label of (sub)tour
          count     = 1,       // size of (sub)tour
          tot_count = 0,       // counter of edges
          pos,                 // position of edge in x array
          starter;             // starting node for each (sub)cycle
  int     indices[G.n],        // indices array for SEC insertion
          subtour_labels[G.n]; // subtour labels
  double  coeffs[G.n];         // coefficients array for SEC insertion

  while (!problem_solved) {

    // Optimize the problem and obtain solution.
    status = CPXmipopt(env, lp);
    if (status) {
      fprintf (stderr, "Failed to optimize MIP.\n");
      exit(1);
    }

    // Write the output to the screen.
    solstat = CPXgetstat(env, lp);
    status  = CPXgetobjval(env, lp, &objval);
    if (status) {
      fprintf (stderr,"No MIP objective value available.  Exiting...\n");
      exit(1);
    }

    printf("problem solved (hopefully) at iteration %d\n", iteration);
    printf("Solution status = %d\n", solstat);
    printf("Solution value  = %f\n\n", objval);

    // decommentare nel caso
    /*// Set time limit for solving the problem
    CPXsetdblparam(env, CPX_PARAM_TILIM, MAXTIME);

    // Set relative tolerance on IP solution
    CPXsetdblparam(env, CPX_PARAM_EPGAP, 1E-8);*/

    // The size of the problem should be obtained by asking CPLEX
    cur_numrows = CPXgetnumrows(env, lp);
    cur_numcols = CPXgetnumcols(env, lp);
    printf("numrows = %d, numcols = %d\n", cur_numrows, cur_numcols);


    status = CPXgetx(env, lp, x, 0, cur_numcols-1);
    if (status) {
      fprintf(stderr, "Failed to get optimal integer x.\n");
      exit(1);
    }

    status = CPXgetslack(env, lp, slack, 0, cur_numrows-1);
    if (status) {
      fprintf(stderr, "Failed to get optimal slack values.\n");
      exit(1);
    }

#ifdef DEBUG
    /*
    for (i = 0; i < cur_numrows; i++) {
      printf ("Row %d:  Slack = %10f\n", i, slack[i]);
    }

    for (j = 0; j < cur_numcols; j++) {
      printf ("Column %d:  Value = %10f\n", j, x[j]);
    }

    for (j = 0; j < cur_numcols; j++) {
      printf ("Column %d:  Value = %10f\n", j, x[j]);
    }
    */
#endif

    /** /
    // DISEGNA IL GRAFO
    printf("plot graph\n");
    graph G_CPLEX;
    egraph EG_CPLEX;
    int var;

    graph_init(&G_CPLEX, n);
    for (var = 1; var <= cur_numcols; var++) {
      if (x[var-1] == 1.0) {
        vertices_from_pos(&hash_table, &i, &j, var);
        graph_insert_edge(&G_CPLEX, i, j, 0.0);
      }
    }
    egraph_init(&EG_CPLEX, 1);
    egraph_copy(&EG, &EG_CPLEX);
    graph_to_egraph(&G_CPLEX, &EG_CPLEX); // <- segfault here
    egraph_plot(&EG, &EG_CPLEX);
    egraph_delete(&EG_CPLEX);
    graph_delete(&G_CPLEX);
    / **/

    //
    // look for cycles
    //

    printf("here you should have seen the graph... have you?\n");

    for(i = 0; i < cur_numcols; i++) {
      x[i] = rint(x[i]);
    }


    // find a tour
    memset(subtour_labels, 0, sizeof(subtour_labels));
    memset(indices, 0, sizeof(indices));
    //memset(coeffs, 1., sizeof(coeffs));
    for (j = 0; j < G.n; ++j) {
      coeffs[j] = 1.0;
    }

    i = 0;
    k = 0;
    mark        = 1;
    count       = 1;
    tot_count   = 0;
    starter     = 0;
    termination = FALSE;
    while (!termination) {

      if (i < G.n && subtour_labels[i] == 0) {

        printf("%d ", i+1);
        subtour_labels[i] = mark;

        for (j = 0; j < G.n; ++j) {
          if (subtour_labels[j] == 0) {
            pos_from_vertices(&hash_table, i+1, j+1, &pos);
            if (x[pos-1] == 1.0) {
              indices[k++] = pos-1;
              count++;
              i = j;
              break;
            }
          }
        }

      } else {

        printf("\n");
        printf("count = %d, tot_count = %d\n", count, tot_count+count);

        // if full cycle, we're done
        if (count == G.n) {

          printf("got a full cycle!\n");
          termination = TRUE;
          problem_solved = TRUE;
          break;

        } else {

          // insert last edge (the one going back to starting node)
          pos_from_vertices(&hash_table, i+1, starter+1, &pos);
          indices[k] = pos-1;
          //count++;

          // adjust the counter of passed nodes
          tot_count += count;

          // create temporary arrays for the constraint insertion
          // length is the size of subcycle
          int    idx[count];
          double cfs[count],
                 rhs[1] = {count-1};

          memcpy(&idx, &indices, sizeof(idx));
          memcpy(&cfs, &coeffs, sizeof(cfs));

          // insert constraint
          status = cplex_add_SEC(&hash_table, env, lp, count, idx, cfs, rhs);
          assert(!status);

          // exit?
          if (tot_count == G.n) {

            mark = 0;
            termination = TRUE;

          } else {

            // reset count
            count = 1;
            k = 0;
            printf("let's look for another cycle\n");

            // look for the first non-labelled node
            i = 0;
            while (i < G.n && subtour_labels[i] != 0) {
              i++;
            }
            starter = i;
            // go to next subcycle
            mark++;

          }

        } // end subcycle analysis

      } // end outer if-else

    } // end while (!termination)

    iteration++;
  } // end while (!problem_solved)

  return 0;
}

