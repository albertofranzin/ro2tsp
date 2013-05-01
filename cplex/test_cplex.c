#include "cplex_solver.h"

// maximum length of variable/constraint names
#define MAXNAME 64
#define INFINITY 1e30
#define EPSILON 1e-6
#define MAXITN 10
#define TRUE 1
#define FALSE 0

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

int main (int argc, char const* argv[]) {

    // return code
    int ret = 0;
    // status code returned by cplex callable library functions
    int status = 0;
    // cplex environment to pass to/from cplex callable library
    CPXENVptr env = NULL;
    // partial formulation
    CPXLPptr  ip = NULL;
    // use for storing error messages returned by CPLEX
    char errmsg[1024];
    // counters
    int i, j, k;
    // number of vertices in the graph
    int m;
    // number of variables in the problem (=m^2)
    int n;

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
        }
        else {
            srand(time(NULL));
        }
        egraph_random(&EG);
        egraph_to_graph(&EG, &G);
        printf("@ Euclidean TSP\n# random instance\n# number of nodes = %d\n# seed = %ld\n\n", pars->number_of_nodes, pars->seed);
    }


    m = pars->number_of_nodes;
    n = m*m;

    // distance matrix
    double** D = (double**) malloc(m * sizeof(double*));
    float v;

    for(i = 0; i < m-1; i++) {
        //fprintf(stdout, "input distances (%d,j) with j>%d\n", i, i);
        D[i] = (double*) malloc(m * sizeof(double));
        D[i][i] = INFINITY;
        for(j = i + 1; j < m; j++) {
            //fscanf(stdin, "%f", &v);
            D[i][j] = (double) graph_get_edge_cost(&G,i+1,j+1);
        }
    }
    D[m-1] = (double*) malloc(m * sizeof(double));
    // make distance matrix symmetric
    for(i = 0; i < m - 1; i++) {
        for(j = i + 1; j < m; j++) {
            D[j][i] = D[i][j];
        }
    }

    // print distance matrix
    fprintf(stderr, "distance matrix is:\n");
    for(i = 0; i < m; i++) {
        for(j = 0; j < m; j++) {
            if (i == j) {
                fprintf(stderr, " inf");
            } else {
                fprintf(stderr, " %.2f", D[i][j]);
            }
        }
        fprintf(stderr, "\n");
    }

    // until here, everything seems ok

      // flatten to costs
  double* c = (double*) malloc(n * sizeof(double));
  k = 0;
  for(i = 0; i < m; i++) {
    for(j = 0; j < m; j++) {
      c[k] = D[i][j];
      k++;
    }
  }

  printf("yo yo ma\n");

    // initialize the CPLEX environment 
    env = CPXopenCPLEX(&status);
    if(status){
        fprintf(stderr, "Error: failed to initialize CPLEX environment\n");
        exit(1);
    }

    // turn on output to screen
    status = CPXsetintparam(env, CPX_PARAM_SCRIND, CPX_OFF);
    status = CPXsetintparam(env, CPX_PARAM_MIPDISPLAY, 2);


    char *probname = NULL;
    int numcols;
    int numrows;
    int objsen;
    //double *obj = NULL;
    double *rhs = NULL;
    char *sense = NULL;
    int *matbeg = NULL;
    int *matcnt = NULL;
    int *matind = NULL;
    double *matval = NULL;
    double *lb = NULL;
    double *ub = NULL;
    char *ctype = NULL;

    int solstat;
    //double *x = NULL;
    CPXLPptr lp = NULL;

    int iteration = 1;

    // create the problem
    lp = CPXcreateprob(env, &status, "tsp");
    if (lp == NULL) {
        fprintf(stderr, "%s, failed to create problem\n", argv[0]);
        exit(4);
    }

    double *obj =   (double *) malloc (m * (m-1) * sizeof(double) / 2);

    for (i = 0; i < m; ++i) {
        for (j = 0; j < i; ++j) {
            obj[i * m + j] = c[i * m + j];
        }
    }

    status = CPXchgobjsen (env, lp, CPX_MIN);
    printf("obj done\n");

    // variable lower bounds 
    double* xL = (double*) malloc(m * (m-1) * sizeof(double) / 2);
    // variable upper bounds
    double* xU = (double*) malloc(m * (m-1) * sizeof(double) / 2);
    // variable types
    ctype = (char*) malloc(m * (m-1) * sizeof(char) / 2);
    for(j = 0; j < m * (m-1) / 2; j++) {
        xL[j] = 0;
        xU[j] = 1;
        ctype[j] = 'B';  // variables are binary
    }

    status = CPXnewcols (env, lp, m * (m-1) / 2 , obj, xL, xU, NULL, ctype);

    printf("variable bounds done\n");

    /*status = setproblemdata (&probname, &numcols, &numrows, &objsen, &obj,
        &rhs, &sense, &matbeg, &matcnt, &matind, &matval,
        &lb, &ub, &ctype);

    if (status){
        fprintf(stderr, "Error: setproblemdata failed at iteration %d", iteration);
        exit(1);
    }*/

    printf("lp problem created\n");


    //printf("cplex: solving to LP optimality, max time %d sec\n", MAXTIME);
    status = CPXlpopt(env, lp);

    if (status) {
        if (status == CPXERR_PRESLV_INForUNBD) {
            //;
            //error("cplex: infeasible or unbounded\n");
            fprintf(stderr, "cheavaca\n");
        } else {
            fprintf(stderr, "cplex: failed to optimize, code %d\n", status); 
        }
    }

    printf("problem solved (hopefully)\n");

    // decommentare nel caso
    /*// Set time limit for solving the problem
    CPXsetdblparam(env, CPX_PARAM_TILIM, MAXTIME);

    // Set relative tolerance on IP solution
    CPXsetdblparam(env, CPX_PARAM_EPGAP, 1E-8);*/

    /* The size of the problem should be obtained by asking CPLEX */
    numrows = CPXgetnumrows(env, lp);
    numcols = CPXgetnumcols(env, lp);

    printf("numrows = %d, numcols = %d\n", numrows, numcols);

    double *x =     (double *) malloc (numcols * sizeof(double)),
           *slack = (double *) malloc (numrows * sizeof(double)),
           *dj =    (double *) malloc (numcols * sizeof(double)),
           *pi =    (double *) malloc (numrows * sizeof(double));


    if ( x     == NULL ||
        slack == NULL ||
        dj    == NULL ||
        pi    == NULL   ) {
        status = CPXERR_NO_MEMORY;
        fprintf (stderr, "Could not allocate memory for solution.\n");
        exit(5);
    }

    printf("space for solution allocated\n");


    status = CPXsolution (env, lp, &solstat, &obj, x, pi, slack, dj);
    if ( status ) {
        fprintf (stderr, "Failed to obtain solution.\n");
        exit(6);
    }


    printf ("\nSolution status = %d\n", solstat);
    printf ("Solution value  = %f\n\n", obj);

    for (i = 0; i < numrows; i++) {
        printf ("Row %d:  Slack = %10f  Pi = %10f\n", i, slack[i], pi[i]);
    }

    for (j = 0; j < numcols; j++) {
        printf ("Column %d:  Value = %10f  Reduced cost = %10f\n",
                j, x[j], dj[j]);
    } 


  /*printf("ma ma yo\n");

  // variable lower bounds 
  double* xL = (double*) malloc(n * sizeof(double));
  // variable upper bounds
  double* xU = (double*) malloc(n * sizeof(double));
  // variable types
  ctype = (char*) malloc(n * sizeof(char));
  for(j = 0; j < n; j++) {
    xL[j] = 0;
    xU[j] = 1;
    ctype[j] = 'B';  // variables are binary
  }

  printf("ma yo\n");

  // TODO:
  // create the variables

  // add constraints in ip
  double* coeffs = (double*) malloc(n * sizeof(double));
  int* indices = (int*) malloc(n * sizeof(int));
  int nz = 0;

  printf("nz\n");

  // constraints 1: for all i, sum_j x_{ij} = 1
  for(i = 0; i < m; i++) {
    nz = 0;
    for(j = 0; j < m; j++) {
      if (i != j) {
        coeffs[nz] = 1.0;
        printf("%d %d\n", i, j);
        indices[nz] = Flatten2DIndices(i,j,m);
        nz++;
      }
    }
    sense = 'E';  // equation
    rhs = 1.0;    // =1
    printf("adding constraint %d\n", i);
    AddConstraint(env, ip, nz, coeffs, indices, sense, rhs);
    printf("constraint added %d\n", i);
  }

  printf("yayo\n");

  // TODO:
  // constraints 2: for all j, sum_i x_{ij} = 1
  for(j = 0; j < m; j++) {
    // (compute nz, coeffs, indices, sense, rhs)
    AddConstraint(env, ip, nz, coeffs, indices, sense, rhs);
  }

  printf("ya\n");

  // select LP optimization algorithm
  status = CPXsetintparam(env, CPX_PARAM_LPMETHOD, CPX_ALG_AUTOMATIC);
  if (status != 0) {
    fprintf(stderr, "%s: could not select optimization algorithm, error %d\n", 
      argv[0], status);
    exit(3);
  }

  printf("hey\n");

  // do it!
  fprintf(stdout, "************ TSP: algorithmic progress ****************\n");
  int termination = FALSE;
  double objval;
   x = (double*) malloc(n * sizeof(double));
  int currentvertex;
  int* successor = (int*) malloc(m * sizeof(int));
  int* cycle = (int*) malloc(m * sizeof(int));
  int solstat = 0;
  int itncount = 1;
  while(!termination) {

    // optimize 
    status = CPXmipopt(env, ip);
    if (status != 0) {
      fprintf(stderr, "%s: failed to call optimization algorithm on ip\n", 
          argv[0]);
      exit(10);
    }
    solstat = CPXgetstat(env, ip);
    if (solstat != 101) {
      fprintf(stdout, "  couldn't find optimal solution, status = %d\n", 
          solstat);
      termination = TRUE;
      break;
    }
    // get ip solution
    for(i = 0; i < n; i++) {
      x[i] = 0;
    }
    CPXgetmipx(env, ip, x, 0, n - 1);
    CPXgetmipobjval(env, ip, &objval);
    for(i = 0; i < n; i++) {
      x[i] = rint(x[i]);
    }
    fprintf(stdout, "iteration %d: current solution with cost %.2f:\n", 
        itncount, objval);

    // TODO:
    // verify if there are disjoint cycles
    // (compute successors)
    // (find a cycle, let k be its cardinality)

    if (k >= m) {
      // cycle is hamiltonian, optimal solution
      fprintf(stdout, " )\n    is the optimal solution\n");
      termination = TRUE;
    } else {
      // solution is union of disjoint cycles, break the cycle
      fprintf(stdout, " )\n    is a disjoint cycle, breaking\n");

      // TODO:
      // add the constraint sum_{i!=j in cycle} x[i][j] <= k - 1
      // (compute nz, coeffs, indices, sense, rhs)
      AddConstraint(env, ip, nz, coeffs, indices, sense, rhs);
    }
    itncount++;
  }
  fprintf(stdout, "*******************************************************\n");

  // free storage
  free(cycle);
  free(successor);
  free(x);
  CPXfreeprob(env, &ip);
  free(coeffs);
  free(indices);
  free(ctype);
  free(xU);
  free(xL);
  free(c);
  for(i = m-1; i >= 0; i--) {
    free(D[i]);
  }
  free(D);*/

    return ret;
}

