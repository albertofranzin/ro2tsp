#include "cpx_setup_problem.h"

int cpx_setup_problem(CPXENVptr   env,
                      CPXLPptr    lp,
                      graph      *G,
                      cpx_table  *hash_table)
{
  int i, j, k,
      ind, status;

  int    n    = G->n,
         ccnt = n * (n - 1) / 2;

  double obj[ccnt];
  double lb[ccnt];
  double ub[ccnt];
  char   xtype[ccnt];

  //printf("CPXchgprobtype\n");
  CPXchgprobtype(env, lp, CPXPROB_MILP); // forse non serve

  // Objective function.
  //printf("obj func\n");
  for (i = 0; i < n; i++) {
    for (j = i+1; j < n; j++) {
      indx_from_vertices(hash_table, i, j, &ind);
      obj[ind] = graph_get_edge_cost(G, i, j);
    }
  }

  //printf("type of variables\n");
  // Type of variables.
  for (i = 0; i < ccnt; i++) {
    lb[i]    = 0.0;
    ub[i]    = 1.0;
    xtype[i] = 'B';
  }

  //printf("newcols\n");
  status = CPXnewcols(env, lp, ccnt, obj, lb, ub, xtype, NULL);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c ::");
    fprintf(stderr, " CPXnewcols.\n");
    fprintf(stderr, "Error while inserting a constraint: %d.\n", status);
    exit(1);
  }

  //printf("CPXchgobjsen\n");
  status = CPXchgobjsen(env, lp, CPX_MIN);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c ::");
    fprintf(stderr, " CPXchgobjsen.\n");
    fprintf(stderr, "Error while setting problem type: %d.\n", status);
    exit(1);
  }

  // Add constraints.
  //printf("1\n");
  int     rcnt = n;
  //("2\n");
  int     nzcnt = n * (n - 1);
  //printf("3\n");
  double  rhs[rcnt];
  //printf("4\n");
  char    sense[rcnt];
  //printf("5\n");
  int     rmatbeg[rcnt];
  //printf("6\n");
  int     rmatind[nzcnt];
  //printf("7\n");
  double  rmatval[nzcnt];
  //printf("8\n");

  //printf("ahnhanahna\n");
  for (i = 0; i < rcnt; i++) {
    //printf(",, %d\n", i);
    rhs[i]     = 2.0;
    sense[i]   = 'E';
    rmatbeg[i] = i * (n-1);
  }

  //printf("mah\n");
  for (i = 0; i < n; i++) {
    k = 0;
    for (j = 0; j < n; j++) {
      if (j != i) {
        indx_from_vertices(hash_table, i, j, &ind);
        rmatind[i * (n-1) + k] = ind;
        // attenzione: gli indici delle variabili passati con questo metodo
        // si assumono numerati a partire da 0
        k++;
      }
    }
  }

  //printf("boh\n");
  for (i = 0; i < nzcnt; i++) {
    rmatval[i] = 1.0;
  }

  //printf("addrows\n");
  // nomi variabili e vincoli? le var si potrebbero chiamare x_i_j
  // per capire che si riferiscono al lato (i, j)
  status = CPXaddrows(env, lp, 0, rcnt, nzcnt, rhs, sense,
                      rmatbeg, rmatind, rmatval, NULL, NULL);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c ::");
    fprintf(stderr, " CPXaddrows\n");
    fprintf(stderr, "Error while inserting a new constraint: %d.\n", status);
    exit(1);
  }

  return status;
}
