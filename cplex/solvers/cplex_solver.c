#include "cplex_solver.h"


/*
 * cplex_solve_problem
 *
 * - graph *  : pointer to the graph
 * - egraph * : pointer to the egraph object
 * - double * : pointer to incumbent
 *
 * solve a LP problem using CPLEX
 */
void cplex_solve_problem(graph *G, egraph *EG, double *incumbent) {
  //
  // define variables
  //

  int i, j, k;
  // status code returned by cplex callable library functions
  int status = 0;
  // cplex environment to pass to/from cplex callable library
  CPXENVptr env = NULL;
  // partial formulation
  CPXLPptr lp = NULL;
  // use for storing error messages returned by CPLEX
  char errmsg[1024];

  // number of nodes
  int n = G->n;

  int NUMCOLS = (n * (n - 1)) / 2,
      NUMROWS = n,
      NUMNZ = n * (n - 1);

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
  double   objval;         // cost of the solution
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
  cplex_table_populate(&hash_table, G);

  status = cplex_setup_problem(G, &hash_table, env, lp,
              probname, &numrows, &numcols, &objsen,
              &obj, &rhs, &sense,
              &matbeg, &matcnt, &matind, &matval,
              &lb, &ub, &ctype);
  assert(!status);

  //status = CPXsetdblparam(env, CPX_PARAM_CUTUP, heuristic_upper_bound);
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
  int iteration      = 1,       // # of iterations passed
      termination    = FALSE,   // control for the inner cycle
      problem_solved = FALSE,   // control for the mail cycle
      cur_numrows,              // # of rows, as modified by new constraints
      cur_numcols;              // # of cols, as modified by new constraints

  // variables for (sub)cycle detection
  int     mark      = 1,       // label of (sub)tour
          count     = 1,       // size of (sub)tour
          tot_count = 0,       // counter of edges
          pos,                 // position of edge in x array
          starter;             // starting node for each (sub)cycle
  int     indices[n],        // indices array for SEC insertion
          subtour_labels[n]; // subtour labels
  double  coeffs[n];         // coefficients array for SEC insertion

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
    int var, n = n;

    graph_init(&G_CPLEX, n);
    for (var = 1; var <= cur_numcols; var++) {
      if (x[var-1] == 1.0) {
        vertices_from_pos(&hash_table, &i, &j, var);
        graph_insert_edge(&G_CPLEX, i, j, 0.0);
      }
    }
    egraph_init(&EG_CPLEX, 1);
    egraph_copy(EG, &EG_CPLEX);
    graph_to_egraph(&G_CPLEX, &EG_CPLEX); // <- once there was a segfault here...
    egraph_plot(EG, &EG_CPLEX);
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
    for (j = 0; j < n; ++j) {
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

      if (i < n && subtour_labels[i] == 0) {

        printf("%d ", i+1);
        subtour_labels[i] = mark;

        for (j = 0; j < n; ++j) {
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
        if (count == n) {

          printf("got a full cycle!\n");

          // update incumbent
          *incumbent = objval;
          // stop cycles
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
          if (tot_count == n) {

            mark = 0;
            termination = TRUE;

          } else {

            // reset count
            count = 1;
            k = 0;
            printf("let's look for another cycle\n");

            // look for the first non-labelled node
            i = 0;
            while (i < n && subtour_labels[i] != 0) {
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

}

/*
 * cplex_create_problem
 *
 * - CPXENVptr : pointer to the CPLEX environment
 * - CPXLPptr  : pointer to the CPLEX LP problem
 * - char *    : problem name
 *
 * create a new environment and problem
 *
 * return : operation status
 */
int cplex_create_problem(CPXENVptr *env, CPXLPptr *lp, char *probname) {
  int status;

  // Initialize the CPLEX environment
  *env = CPXopenCPLEX(&status);

  // If an error occurs, the status value indicates the reason for
  // failure.  A call to CPXgeterrorstring will produce the text of
  // the error message.  Note that CPXopenCPLEX produces no output,
  // so the only way to see the cause of the error is to use
  // CPXgeterrorstring.  For other CPLEX routines, the errors will
  // be seen if the CPX_PARAM_SCRIND indicator is set to CPX_ON.

  if (*env == NULL) {
    char errmsg[CPXMESSAGEBUFSIZE];
    fprintf(stderr, "Could not open CPLEX environment.\n");
    CPXgeterrorstring(*env, status, errmsg);
    fprintf(stderr, "%s", errmsg);
    exit(1);
  }

  // Create the problem.

  *lp = CPXcreateprob(*env, &status, probname);

  // A returned pointer of NULL may mean that not enough memory
  // was available or there was some other problem.  In the case of
  // failure, an error message will have been written to the error
  // channel from inside CPLEX.  In this example, the setting of
  // the parameter CPX_PARAM_SCRIND causes the error message to
  // appear on stdout.

  if (*lp == NULL) {
    fprintf (stderr, "Failed to create LP.\n");
    exit(1);
  }

  // turn on output to screen if debug mode is chosen
#ifdef DEBUG
  status = CPXsetintparam(*env, CPX_PARAM_SCRIND, CPX_ON);
#else
  status = CPXsetintparam(*env, CPX_PARAM_SCRIND, CPX_OFF);
#endif

  //status = CPXsetintparam(*env, CPX_PARAM_MIPDISPLAY, 2);

  if (status) {
    fprintf (stderr, "Failure to trigger screen indicator, error %d.\n", status);
    exit(1);
  }

  // select LP optimization algorithm
  status = CPXsetintparam(*env, CPX_PARAM_LPMETHOD, CPX_ALG_AUTOMATIC);
  if (status != 0) {
    fprintf(stderr, "Could not select optimization algorithm, error %d\n", status);
    exit(3);
  }

  return status;
} // end cplex_create_problem





/*
 * cplex_setup_problem
 *
 * general parameters:
 * - graph *   : pointer to the graph
 * - table *   : pointer to hash table
 * - CPXENVptr : pointer to the CPLEX environment
 * - CPXLPptr  : pointer to the CPLEX LP problem
 * - char *    : problem name                  [max 16 chars]
 * - int *     : pointer to number of rows
 * - int *     : pointer number of columns
 * - int *     : pointer to problem type       [CPX_MIN/CPX_MAX]
 *
 * constraints parameters:
 * - double ** : object function coefficients  [# : numcols]
 * - double ** : right-hand-side coefficients  [# : numrows]
 * - char **   : sense of the inequalities     [# : numrows]
 * - int **    : indices of the i-th variable constraints
 *                                             [# : numcols]
 * - int **    : number of occurrences of the i-th variable
 *                                             [# : numcols]
 * - int **    : index of the i-th variable    [# : non-zero vars]
 * - double ** : coefficients                  [# : non-zero vars]
 * - double ** : lower bound for the variables [# : numcols]
 * - double ** : upper bound for the variables [# : numcols]
 * - char **   : type of the variables         [# : numcols]
 *
 * set up the cplex data structures
 *
 * return : operation status
 */
int cplex_setup_problem(
            graph *G, cplex_table *hash_table, CPXENVptr env, CPXLPptr lp,
            char *probname_p, int *numcols_p, int *numrows_p, int *objsen_p,
            double **obj_p, double **rhs_p, char **sense_p,
            int **matbeg_p, int **matcnt_p, int **matind_p, double **matval_p,
            double **lb_p, double **ub_p, char **ctype_p) {

  int n = G->n;
  int NUMCOLS = (n * (n - 1)) / 2,
      NUMROWS = n,
      NUMNZ = n * (n - 1);

  char   *zprobname = NULL;  // Problem name <= 16 characters
  double *zobj      = NULL;
  double *zrhs      = NULL;
  char   *zsense    = NULL;
  int    *zmatbeg   = NULL;
  int    *zmatcnt   = NULL;
  int    *zmatind   = NULL;
  double *zmatval   = NULL;
  double *zlb       = NULL;
  double *zub       = NULL;
  char   *zctype    = NULL;
  int     status    = 0;

  zprobname = (char *) malloc(16 * sizeof(char));
  zobj      = (double *) malloc(NUMCOLS * sizeof(double));
  zrhs      = (double *) malloc(NUMROWS * sizeof(double));
  zsense    = (char *) malloc(NUMROWS * sizeof(char));
  zmatbeg   = (int *) malloc(NUMCOLS * sizeof(int));
  zmatcnt   = (int *) malloc(NUMCOLS * sizeof(int));
  zmatind   = (int *) malloc(NUMNZ * sizeof(int));
  zmatval   = (double *) malloc(NUMNZ * sizeof(double));
  zlb       = (double *) malloc(NUMCOLS * sizeof(double));
  zub       = (double *) malloc(NUMCOLS * sizeof(double));
  zctype    = (char *) malloc(NUMCOLS * sizeof(char));

  if (zprobname == NULL || zobj    == NULL ||
      zrhs      == NULL || zsense  == NULL ||
      zmatbeg   == NULL || zmatcnt == NULL ||
      zmatind   == NULL || zmatval == NULL ||
      zlb       == NULL || zub     == NULL ||
      zctype    == NULL                      ) {
    status = 1;
    exit(1);
  }

  strcpy(zprobname, "tsp");

  int i, j;
  int x, y, var;

  for (i = 1; i <= NUMCOLS; i++) {
    vertices_from_pos(hash_table, &x, &y, i);
    zobj[i-1] = graph_get_edge_cost(G, x, y);
  }

  for (i = 0; i < NUMCOLS; i++) {
    zmatbeg[i] = i*2;
  }

  for (i = 0; i < NUMCOLS; i++) {
    zmatcnt[i] = 2;
  }

  for (i = 1; i <= n; i++) {
    for (j = i+1; j <= n; j++) {
      pos_from_vertices(hash_table, i, j, &var);
      zmatind[(var-1)*2] = i-1;
      zmatval[(var-1)*2] = 1.0;
      zmatind[(var-1)*2 + 1] = j-1;
      zmatval[(var-1)*2 + 1] = 1.0;
    }
  }

  // constraints on (binary) variables (edges)
  // x_e \in {0,1} \forall e \in E
  for (i = 0; i < NUMCOLS; i++) {
    zlb[i] = 0.0;
    zub[i] = 1.0;
    zctype[i] = 'B';
  }

  // right-hand-side values - constraints on node degree
  // sum_{e \in \delta(v)} x_e = 2 \forall v \in V
  for (i = 0; i < NUMROWS; i++) {
    zsense[i] = 'E';
    zrhs[i] = 2.0;
  }

  //printf("array created\n");

  if (status) {

    free_and_null((char **) &zprobname);
    free_and_null((char **) &zobj);
    free_and_null((char **) &zrhs);
    free_and_null((char **) &zsense);
    free_and_null((char **) &zmatbeg);
    free_and_null((char **) &zmatcnt);
    free_and_null((char **) &zmatind);
    free_and_null((char **) &zmatval);
    free_and_null((char **) &zlb);
    free_and_null((char **) &zub);
    free_and_null((char **) &zctype);

    exit(1);

  } else {

    //printf("assigning variables\n");

    *numcols_p   = NUMCOLS;
    *numrows_p   = NUMROWS;
    *objsen_p    = CPX_MIN;

    // HOUSTON, we have a problem with strcpy
    //strcpy(probname_p, zprobname);
    *obj_p    = zobj;
    *rhs_p    = zrhs;
    *sense_p  = zsense;
    *matbeg_p = zmatbeg;
    *matcnt_p = zmatcnt;
    *matind_p = zmatind;
    *matval_p = zmatval;
    *lb_p     = zlb;
    *ub_p     = zub;
    *ctype_p  = zctype;

  }

  // now tell cplex everything it needs

  /*printf("%d %d %d\n", NUMCOLS, NUMROWS, CPX_PARAM_SCRIND);
  printf("zobj:\n");
  for (i = 0; i < NUMCOLS; ++i) {
    printf("%f\n", zobj[i]);
  }
  printf("zrhs, zsense:\n");
  for (i = 0; i < NUMROWS; ++i) {
    printf("%f %c\n", zrhs[i], zsense[i]);
  }
  printf("zmatbeg\n");
  for (i = 0; i < NUMCOLS; ++i) {
    printf("%d %d %d %f\n", zmatbeg[i], zmatcnt[i], zmatind[i], zmatval[i]);
  }
  printf("zlb\n");
  for (i = 0; i < NUMCOLS; ++i) {
    printf("%f %f\n", zlb[i], zub[i]);
  }
  printf("parameters seem ok\n");*/
  status = CPXcopylp(env, lp, NUMCOLS, NUMROWS, CPX_MIN,
                     zobj, zrhs, zsense, zmatbeg, zmatcnt, zmatind, zmatval,
                     zlb, zub, NULL);

  if (status) {
    fprintf(stderr, "Failed to copy problem data.\n");
    exit(1);
  }

  status = CPXcopyctype(env, lp, zctype);

  if (status) {
    fprintf(stderr, "Failed to copy ctype\n");
    exit(1);
  }

  return status;
} // end cplex_setup_problem


/*
 * cplex_add_SEC
 *
 * - CPXENVptr : pointer to the CPLEX environment
 * - CPXLPptr  : pointer to the CPLEX LP problem
 * - int       : number of components
 * - int *     : vector of indices
 * - double *  : vector of components
 * - double *  : vector containing the right-hand-side of the constraint
 *
 * add a new SEC constraint to the lp
 *
 * return : operation status
 */
// hash_table has been added for debug!
int cplex_add_SEC(cplex_table *hash_table, CPXENVptr env, CPXLPptr lp,
                  int nonzeros, int *indices, double *coeffs, double *rhs) {
  int status, i,
      rmatbeg[nonzeros];

  char sense[1] = {'L'};

  for (i = 0; i < nonzeros; ++i) {
    rmatbeg[i] = i;
  }

#ifdef DEBUG
  // print more infos for debug
  int x, y;
  for (i = 0; i < nonzeros; ++i) {
    vertices_from_pos(hash_table, &x,&y,indices[i]+1);
    printf("%f;%d = (%d,%d)\n", coeffs[i], indices[i], x, y);
  }
  printf(" <= %f\n", rhs[0]);
  char ch = getchar();
#endif

  status = CPXaddrows(env, lp, 0, 1, nonzeros, rhs, sense,
                      rmatbeg, indices, coeffs, NULL, NULL);

  if (status) {
    fprintf(stderr, "Could not add constraint.\nBye.");
    exit(1);
  }

  return status;
} // end cplex_add_SEC
