#include "cplex_solver.h"


// sparsify constraint format and add it to CPLEX problem
void AddConstraint(CPXENVptr env, CPXLPptr lp, int nz, 
       double* coeffs, int* ind, char sense, double rhs) {
  int status = 0;
  // add constraints in row-wise sparse format
  int rmatbeg[2];
  // nonzeroes stored in rmatval starting at position:
  rmatbeg[0] = 0;
  // nonzeroes stored in rmatval ending at position:
  rmatbeg[1] = nz-1;
  // create the constraints
  printf("aaaaa\n");
  status = CPXaddrows(env, lp, 0, 1, nz, &rhs, &sense, rmatbeg, ind, coeffs, NULL, NULL);
  printf("uuuuu\n");
  if (status != 0) {
    fprintf(stderr, "pooling-slp: could not create constraints, error %d\n", 
      status);
    exit(3);
  }
}

// take row, column and width and produce a one-dimensional index
int Flatten2DIndices(int i, int j, int m) {
  return i*m + j;
}

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

  // number of vertices in the graph
  int m;
  // number of variables in the problem (=m^2)
  int n;

  m = pars->number_of_nodes;
  n = m*m;

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
  status = cplex_create_problem(env, lp, probname);
  assert(!status);

  printf("lp problem created\n");

  // MOVE TO SOMEWHERE ELSE
  // turn on output to screen
  //status = CPXsetintparam(env, CPX_PARAM_SCRIND, CPX_OFF);
  //status = CPXsetintparam(env, CPX_PARAM_SCRIND, CPX_ON);
  //status = CPXsetintparam(env, CPX_PARAM_MIPDISPLAY, 2);
  if (status) {
    fprintf (stderr, "Failure to turn on screen indicator, error %d.\n", status);
    exit(1);
  }


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

  printf("problem filled in\n");


  //
  // solve
  //

  // control for the solver cycle
  int iteration   = 1,
      termination = FALSE,
      cur_numrows, cur_numcols;
  int subtour_labels[G.n];

  while (!termination) {

    //printf("cplex: solving to LP optimality, max time %d sec\n", MAXTIME);
    //status = CPXlpopt(env, lp);

    // Optimize the problem and obtain solution.
    status = CPXmipopt(env, lp);
    if ( status ) {
      fprintf (stderr, "Failed to optimize MIP.\n");
      exit(1);
    }

    printf("problem solved (hopefully) at iteration %d\n", iteration);


    // Write the output to the screen.
    solstat = CPXgetstat(env, lp);
    status  = CPXgetobjval(env, lp, &objval);
    if (status) {
      fprintf (stderr,"No MIP objective value available.  Exiting...\n");
      exit(1);
    }

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
    printf("numrows = %d, numcols = %d\n", numrows, numcols);


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

    /*
    // DISEGNA IL GRAFO
    graph G_CPLEX;
    egraph EG_CPLEX;
    int var;

    graph_init(&G_CPLEX, n);
    for (var = 1; var <= cur_numcols; var++) {
      if (x[var-1] == 1.0) {
        get_edge_from_var(&i, &j, &var, &hash_table);
        graph_insert_edge(&G_CPLEX, i, j, 0.0);
      }
    }
    egraph_init(&EG_CPLEX, 1);
    egraph_copy(&EG, &EG_CPLEX);
    graph_to_egraph(&G_CPLEX, &EG_CPLEX);
    egraph_plot(&EG, &EG_CPLEX);
    egraph_delete(&EG_CPLEX);
    graph_delete(&G_CPLEX);
    */

    //
    // look for cycles
    //

    for(i = 0; i < cur_numcols; i++) {
      x[i] = rint(x[i]);
    }

    memset(subtour_labels, 0, sizeof(subtour_labels));

    // find a tour
    i = 0;
    int mark = 1, count = 0;
    int found_one = TRUE;

    while (found_one) {
      found_one = FALSE;

      if (subtour_labels[i] == 0) {
        printf("%d ", i);
        found_one = TRUE;
        subtour_labels[i] = mark;
        for (j = 0; j < m; ++j) {
          int k = (i > j) ? i*m + j : j * m + i;
          if (x[k] == 1) {
            i = k;
            count++;
            break;
          }
        }
      } else {
        printf("\n");
        // if full cycle, we're done
        if (count == m) {
          printf("got a cycle!\n");
          termination = TRUE;
          break;
        }

        // indices
        double* indices = (double*) malloc(count * sizeof(double));

        int k = 0;
        for(j = 0; j < count; j++) {
          if (subtour_labels[j] == mark) {
            indices[k++] = j;
          }
        }

        // add constraint in the correct way, porcatroia...

        mark++;
      }
    }

  }

  return 0;
}

