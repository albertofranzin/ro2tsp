#include "cplex_solver.h"

/*************************************************
 *
 * hash table (commodity)
 *
 *************************************************/

/*
 * cplex_table_init
 *
 * create a table starting from a graph
 */
void cplex_table_init(cplex_table* CPX_TAB, int size) {
  if (size < 1) {
    printf("error: table_init\n");
  }

  (*CPX_TAB).size = size;
  (*CPX_TAB).n = 0;
  (*CPX_TAB).entries = (int**)malloc(sizeof(int*) * size);

  int i;
  for (i = 0; i < size; i++) {
    (*CPX_TAB).entries[i]  = (int*)calloc(3, sizeof(int));
  }
}

/*
 * cplex_table_delete
 *
 * delete a table
 */
void cplex_table_delete(cplex_table* CPX_TAB) {
  int i;

  for (i = 0; i < (*CPX_TAB).size; i++) {
    free((*CPX_TAB).entries[i]);
  }

  free((*CPX_TAB).entries);
  (*CPX_TAB).size = 0;
  (*CPX_TAB).n = 0;
}

/*
 * cplex_table_populate
 *
 * hash (v1, v2)->(pos)
 *
 * table * : hash table
 * int * : pointer to index of vertex 1 (not modified)
 * int * : pointer to index of vertex 2 (not modified)
 * int * : pointer to index of position (to be modified)
 *
 * le posizioni dei lati (i, j) sono assegnate come segue (sia n=5)
 * lato: (1,2) (1,3) (1,4) (1,5) (2,3) (2,4) (2,5) (3,4) (3,5) ...
 * pos :   1     2     3     4     5     6     7     8     9   ...
 */
void cplex_table_populate(cplex_table* CPX_TAB, graph* G) {

  if ((*CPX_TAB).size != ((*G).n * ( (*G).n - 1))  /  2) {
    printf("error: table_populate\n");
    exit(EXIT_FAILURE);
  }

  int i, j, pos;
  int n = (*G).n;

  (*CPX_TAB).n = n;

  for (i = 1; i <= n; i++) {
    for (j = i+1; j <= n; j++) {
      // assumendo che i nodi del grafo siano 0, 1, 2, ..., n-1 si ha:
      // pos = n * (i + 1) - ((i * (i + 1)) / 2) - n + j - i - 1
      // nel nostro caso i nodi del grafo sono indicizzati 1, 2, ..., n
      // inoltre vogliamo che pos parta da 1 e non da 0
      // pos = 1 + n * i ((i * (i - 1)) / 2) - n + j - i - 1
      pos = n * i - ((i * (i - 1)) / 2) - n + j - i; 
      (*CPX_TAB).entries[pos-1][0] = i;
      (*CPX_TAB).entries[pos-1][1] = j;
      (*CPX_TAB).entries[pos-1][2] = pos;
    }
  }

} // end cplex_table_populate

/*
 * vertices_from_pos
 *
 * hash (pos)->(v1, v2)
 *
 * table * : hash table
 * int * : pointer to index of vertex 1 (to be modified)
 * int * : pointer to index of vertex 2 (to be modified)
 * int * : pointer to index of position (not modified)
 */
void vertices_from_pos(cplex_table* CPX_TAB, int* x, int* y, int* pos) {
  if (*x < 1 || *x > (*CPX_TAB).n || *y < 1 || *y > (*CPX_TAB).n || *x == *y) {
    printf("error: pos_from_vertices\n");
    exit(EXIT_FAILURE);
  }
  if (*x < *y) {
    *pos = (*CPX_TAB).n * *x     -     ((*x * (*x - 1)) / 2) - (*CPX_TAB).n     +      *y - *x;
  } else {
    *pos = (*CPX_TAB).n * *y     -     ((*y * (*y - 1)) / 2) - (*CPX_TAB).n     +      *x - *y;
  }
}

/*
 * pos_from_vertices
 *
 * hash (pos)<-(v1, v2)
 *
 * table * : hash table
 * int * : pointer to index of vertex 1 (not modified)
 * int * : pointer to index of vertex 2 (not modified)
 * int * : pointer to index of position (to be modified)
 */
void pos_from_vertices(cplex_table* CPX_TAB, int* x, int* y, int* pos) {
  if (*pos < 1 || *pos > (*CPX_TAB).size) {
    printf("error: vertices_from_pos\n");
    exit(EXIT_FAILURE);
  }

  *x = (*CPX_TAB).entries[*pos-1][0];
  *y = (*CPX_TAB).entries[*pos-1][1];
}





/*************************************************
 *
 * cplex-specific methods
 *
 *************************************************/

/*
 * cplex_create_problem
 *
 * - CPXENVptr : pointer to the CPLEX environment
 * - CPXLPptr : pointer to the CPLEX LP problem
 *
 * create a new environment and problem
 *
 * return : operation status
 */
int cplex_create_problem(CPXENVptr env, CPXLPptr lp) {
  int status;

  // Initialize the CPLEX environment
  env = CPXopenCPLEX (&status);

  // If an error occurs, the status value indicates the reason for
  // failure.  A call to CPXgeterrorstring will produce the text of
  // the error message.  Note that CPXopenCPLEX produces no output,
  // so the only way to see the cause of the error is to use
  // CPXgeterrorstring.  For other CPLEX routines, the errors will
  // be seen if the CPX_PARAM_SCRIND indicator is set to CPX_ON.

  if (env == NULL) {
    char errmsg[CPXMESSAGEBUFSIZE];
    fprintf(stderr, "Could not open CPLEX environment.\n");
    CPXgeterrorstring(env, status, errmsg);
    fprintf(stderr, "%s", errmsg);
    exit(1);
  }

  // Create the problem.

  lp = CPXcreateprob(env, &status, "tsp");

  // A returned pointer of NULL may mean that not enough memory
  // was available or there was some other problem.  In the case of
  // failure, an error message will have been written to the error
  // channel from inside CPLEX.  In this example, the setting of
  // the parameter CPX_PARAM_SCRIND causes the error message to
  // appear on stdout.

  if (lp == NULL) {
    fprintf (stderr, "Failed to create LP.\n");
    exit(1);
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

  int n = (*G).n;
  int NUMCOLS, NUMROWS, NUMNZ;
  NUMCOLS = (n * (n-1)) / 2;
  NUMROWS = n;
  NUMNZ   = (n-1) * n;

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
    get_edge_from_var(&x, &y, &i, hash_table);
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
    get_var_from_edge(&i, &j, &var, hash_table);
    zmatind[(var-1)*2] = i-1;
    zmatval[(var-1)*2] = 1.0;
    zmatind[(var-1)*2 + 1] = j-1;
    zmatval[(var-1)*2 + 1] = 1.0;
   }
  }

  for (i = 0; i < NUMCOLS; i++) {
    zlb[i] = 0.0;
    zub[i] = 1.0;
  }

  for (i = 0; i < NUMCOLS; i++) {
    zctype[i] = 'B';
  }

  // The right-hand-side values don't fit nicely on a line above.
  // So put them here.
  for (i = 0; i < NUMROWS; i++) {
    zsense[i] = 'E';
    zrhs[i] = 2.0;
  }

  if (status) {
    free_and_null ((char **) &zprobname);
    free_and_null ((char **) &zobj);
    free_and_null ((char **) &zrhs);
    free_and_null ((char **) &zsense);
    free_and_null ((char **) &zmatbeg);
    free_and_null ((char **) &zmatcnt);
    free_and_null ((char **) &zmatind);
    free_and_null ((char **) &zmatval);
    free_and_null ((char **) &zlb);
    free_and_null ((char **) &zub);
    free_and_null ((char **) &zctype);
  } else {
    *numcols_p   = NUMCOLS;
    *numrows_p   = NUMROWS;
    *objsen_p    = CPX_MIN;

    *probname_p  = zprobname;
    *obj_p       = zobj;
    *rhs_p       = zrhs;
    *sense_p     = zsense;
    *matbeg_p    = zmatbeg;
    *matcnt_p    = zmatcnt;
    *matind_p    = zmatind;
    *matval_p    = zmatval;
    *lb_p        = zlb;
    *ub_p        = zub;
    *ctype_p     = zctype;
  }

  return status;
} // end cplex_setup_problem



/*
 * cplex_create_obj_function
 *
 * - CPXENVptr : pointer to the CPLEX environment
 * - CPXLPptr : pointer to the CPLEX LP problem
 * - int : number of coefficients
 * - double * : coefficients of the objective function
 *
 * return : operation status
 */
int cplex_create_obj_function(CPXENVptr env, CPXLPptr lp,
          int numcols, double *objval) {
  int status;

  return status;
} // end cplex_create_obj_function




/*
 * cplex_add_SEC
 *
 * - CPXENVptr : pointer to the CPLEX environment
 * - CPXLPptr  : pointer to the CPLEX LP problem
 * - int       : number of components
 * - int *     : vector of indices
 * - double *  : vector of components
 * - double    : right-hand-side of the constraint
 *
 * add a new SEC constraint to the lp
 *
 * return : operation status
 */
int cplex_add_SEC(CPXENVptr env, CPXLPptr lp, int numcols,
          int * indices, double *coeffs, double rhs) {
  int status;

  return status;
} // end cplex_add_SEC