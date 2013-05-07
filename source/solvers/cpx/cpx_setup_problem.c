#include "cpx_setup_problem.h"

int cpx_setup_problem(CPXENVptr env, CPXLPptr lp, graph* G, cpx_table* hash_table) {
  int i, j, k, pos, status;

  int n = (*G).n;
  int ccnt = n * (n - 1) / 2;
  double obj[ccnt];
  double lb[ccnt];
  double ub[ccnt];
  char xtype[ccnt];

  // funzione obiettivo
  for (i = 1; i <= n; i++) {
    for (j = i+1; j <= n; j++) {
      pos_from_vertices(hash_table, i, j, &pos);
      obj[pos-1] = graph_get_edge_cost(G, i, j);
    }
  }

  // bounds (visto che saranno variabili binarie, si può in teoria saltare)
  for (i = 0; i < ccnt; i++) {
    lb[i] = 0.0;
    ub[i] = 1.0;
  }

  // tipo di variabili
  for (i = 0; i < ccnt; i++) {
    xtype[i] = 'B';
  }

  status = CPXnewcols(env, lp, ccnt, obj, lb, ub, xtype, NULL);

  status = CPXchgobjsen(env, lp, CPX_MIN);


  // constraints
  int rcnt = n;
  int nzcnt = n * (n - 1);
  double rhs[rcnt];
  char sense[rcnt];
  int rmatbeg[rcnt];
  int rmatind[nzcnt];
  double rmatval[nzcnt];

  for (i = 0; i < rcnt; i++) {
    rhs[i] = 2.0;
    sense[i] = 'E';
    rmatbeg[i] = i * (n-1);
  }

  for (i = 0; i < n; i++) {
    k = 0;
    for (j = 0; j < n; j++) {
      if (j != i) {
	pos_from_vertices(hash_table, i+1, j+1, &pos);
	rmatind[i * (n-1) + k] = pos-1; // attenzione: gli indici delle variabili passati con questo metodo si assumono numerati a partire da 0
	k++;
      }
    }
  }

  for (i = 0; i < nzcnt; i++) {
    rmatval[i] = 1.0;
  }

  status = CPXaddrows(env, lp, 0, rcnt, nzcnt, rhs, sense, rmatbeg, rmatind, rmatval, NULL, NULL); //  nomi variabili e vincoli? le var si potrebbero chiamare x_i_j per capire che si riferiscono al lato (i, j) 

  return status;
}
