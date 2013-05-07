#include "cpx_solver.h"

void cpx_solver(graph* G, graph* H) {
  int i, j, k, pos;

  int status;
  CPXENVptr env = NULL;
  CPXLPptr lp = NULL;
  char* probname = "problema";

  status = cpx_create_problem(&env, &lp, probname);

  /* Turn on output to the screen */
  //status = CPXsetintparam (env, CPX_PARAM_SCRIND, CPX_ON);
  /* Turn on data checking */
  //status = CPXsetintparam (env, CPX_PARAM_DATACHECK, CPX_ON);

  int n = (*G).n;
  int numcols = n * (n - 1) / 2;

  cpx_table hash_table;
  cpx_table_init(&hash_table, numcols);
  cpx_table_populate(&hash_table, G);

  status = cpx_setup_problem(env, lp, G, &hash_table);

  // variabili per memorizzazione soluzione
  int cur_numcols;
  double objval;
  double x[numcols];
  
  // variabili per ricerca sottocicli
  int my_mark;
  int num_of_subtours;
  int edge_indexes[n];
  int edge_marks[n];

  int num_of_secs = 0;

  num_of_subtours = n;
  while (num_of_subtours > 1) {

    // #1 RISOLUZIONE PROBLEMA CON CPLEX
    status = CPXmipopt(env, lp);

    // #2 MEMORIZZAZIONE SOLUZIONE
    cur_numcols = CPXgetnumcols(env, lp);
    status  = CPXgetobjval(env, lp, &objval);
    status = CPXgetx(env, lp, x, 0, cur_numcols-1);

    k = 0;
    for (i = 0; i < cur_numcols; i++) {
      if (x[i] > 0.9) { // oppure x[i] == 1.0 dopo aver arrotondato le x[i] all'intero più vicino
	edge_indexes[k] = i+1;
	k++;
      }
    }

    // #3 RICERCA SOTTOCICLI
    num_of_subtours = cpx_mark_subtours(&hash_table, edge_indexes, edge_marks, n);

    // #4 AGGIUNTA NUOVI VINCOLI SEC
    for (my_mark = 1; my_mark <= num_of_subtours; my_mark++) {
      status = cpx_add_sec(env, lp, edge_indexes, edge_marks, n, my_mark);
    }

    num_of_secs += num_of_subtours;
    printf("# number of new SECs added = %d : total number of SECs = %d\n", num_of_subtours, num_of_secs);

  }

  graph_delete(H);
  graph_init(H, n);
  for (pos = 1; pos <= cur_numcols; pos++) {

    if (x[pos-1] > 0.9) {
      vertices_from_pos(&hash_table, &i, &j, pos);
      graph_insert_edge(H, i, j, graph_get_edge_cost(G, i, j));
    }
  }

}
