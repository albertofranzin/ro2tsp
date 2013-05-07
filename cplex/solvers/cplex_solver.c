#include "cplex_solver.h"

void cplex_solve_problem(graph* G, egraph* EG, double *incumbent) {
  int i, j, pos;

  int status;
  CPXENVptr env = NULL;
  CPXLPptr lp = NULL;
  char* probname = "tsp";






  status = cplex_create_problem(&env, &lp, probname);
  printf("lp problem created\n");







   /* Turn on output to the screen */
   //status = CPXsetintparam (env, CPX_PARAM_SCRIND, CPX_ON);
   /* Turn on data checking */
   //status = CPXsetintparam (env, CPX_PARAM_DATACHECK, CPX_ON);






  int n = (*G).n;
  int numcols = n * (n - 1) / 2;
  //int numrows = n;

  cplex_table hash_table;
  cplex_table_init(&hash_table, numcols);
  cplex_table_populate(&hash_table, G);

  status = cplex_setup_problem(env, lp, G, &hash_table);
  printf("problem filled in\n");



  // variabili per memorizzazione soluzione
  //int cur_numrows;
  int cur_numcols;
  double objval;
  double x[numcols];
  //double slack[numrows];



  // variabili per ricerca sottocicli
  int neighbour1[n];
  int neighbour2[n];

  int starter;
  int current;
  int next;
  int mark;
  int num_of_cycles;
  int termination;
  int subtour_labels[n];
  int successors[n]; // bisogna tenere conto di chi sia il successore di ogni nodo di un sottociclo, in modo tale da conoscere rapidamente quali sono i lati del sottociclo: questa informazione ci serve per costruire i SEC!

  int count;
  int vertices[n];



  num_of_cycles = 0;
  while (num_of_cycles != 1) {

    // #1 RISOLUZIONE PROBLEMA CON CPLEX
    status = CPXmipopt(env, lp);


    // #2 MEMORIZZAZIONE SOLUZIONE
    //cur_numrows = CPXgetnumrows(env, lp);
    cur_numcols = CPXgetnumcols(env, lp);

    //printf("numrows = %d, numcols = %d\n", cur_numrows, cur_numcols);
    printf("numcols = %d\n", cur_numcols);

    status  = CPXgetobjval(env, lp, &objval);
    printf("Solution value  = %f\n\n", objval);

    status = CPXgetx(env, lp, x, 0, cur_numcols-1);
    //status = CPXgetslack(env, lp, slack, 0, cur_numrows-1);

    // #3 RICERCA SOTTOCICLI
    for(i = 0; i < cur_numcols; i++) {
      // attenzione: occhio a come arrotonda e verifica
      // se ci sono soluzioni buone ma con valore di qualche lato < 0.5
      x[i] = rint(x[i]);
    }


    // memorizzazione neighbours (ogni nodo ne ha due)
    for (i = 0; i < n; i++) {
      neighbour1[i] = 0;
      neighbour2[i] = 0;
    }

    // alla fine del ciclo neighbour1[i-1] e neighbour2[i-1]
    // conterranno i due nodi vicini al nodo i
    for (pos = 1; pos <= cur_numcols; pos++) {
      if (x[pos-1] == 1.0) { // oppure meglio if > .0
        vertices_from_pos(&hash_table, &i, &j, pos);

        if (neighbour1[i-1] == 0)
          neighbour1[i-1] = j;
        else
          neighbour2[i-1] = j;

        if (neighbour1[j-1] == 0)
          neighbour1[j-1] = i;
        else
          neighbour2[j-1] = i;

      }
    }

    // assegnazione delle labels ai nodi
    // (nodi appartenenti allo stesso ciclo hanno la stessa label)
    mark = 1;
    starter = 1;
    termination = FALSE;
    for (i = 0; i < n; i++) {
      subtour_labels[i] = 0;
    }


    while (!termination) {
    
      // come fase iniziale, non marchiamo lo starter,
      // sennò nel ciclo while che seguirà non si potrà mai ritornare in esso
      // visto che di volta in volta si salta su un nodo non marcato
      current = neighbour1[starter-1];
      successors[starter-1] = current; // regitr. succ.
      printf("%d ", starter);
      printf("%d ", current);

      subtour_labels[current-1] = mark;
      next = (neighbour1[current-1] != starter)
                    ? neighbour1[current-1]
                    : neighbour2[current-1];
      successors[current-1] = next; // regitr. succ.

      while (next != starter) {
        printf("%d ", next);
        subtour_labels[next-1] = mark;
        current = next;
        next = (subtour_labels[neighbour1[current-1]-1] == 0) 
                    ? neighbour1[current-1]
                    : neighbour2[current-1];
        successors[current-1] = next;
      }

      printf("\n");
      subtour_labels[starter-1] = mark;
      successors[current-1] = starter; // regitr. succ.


      termination = TRUE;
      for (starter = 1; starter <= n; starter++) {
        printf("%d ", starter);
        if (subtour_labels[starter-1] == 0) {
          mark++;
          termination = FALSE;
          break;
        }
      }
    }
    num_of_cycles = mark;


    // #4 AGGIUNTA NUOVI VINCOLI SEC
    for (mark = 1; mark <= num_of_cycles; mark++) {
      // per ogni label, cerco tutti i vertici che hanno quella stessa label
      // e aggiungo un SEC su quei vertici
      count = 0;
      for (i = 1; i <= n; i++) {
        if (subtour_labels[i-1] == mark) {
          //printf("%d ", i);
          vertices[count] = i;
          count++;
        }
      }
      //printf("\n");
      if (count < n) {
        status = cplex_add_SEC(env, lp, &hash_table, vertices, successors, count);
      }
    }

  }

  /*
  // richiama mipopt
  // risolvi problema con cplex
  status = CPXmipopt(env, lp);

  // soluzione
  cur_numrows = CPXgetnumrows(env, lp);
  cur_numcols = CPXgetnumcols(env, lp);

  printf("numrows = %d, numcols = %d\n", cur_numrows, cur_numcols);

  status  = CPXgetobjval(env, lp, &objval);
  printf("Solution value  = %f\n\n", objval);

  status = CPXgetx(env, lp, x, 0, cur_numcols-1);
  status = CPXgetslack(env, lp, slack, 0, cur_numrows-1);
  */
  /* Finally, write a copy of the problem to a file. */
  //status = CPXwriteprob (env, lp, "./lpex1.lp", NULL);

  // plotta grafo
  graph Gtmp1;
  egraph EGtmp1;
  int pos1;

  graph_init(&Gtmp1, n);
  egraph_init(&EGtmp1, n);

  for (pos1 = 1; pos1 <= cur_numcols; pos1++) {
    if (x[pos1-1] == 1.0) {
      vertices_from_pos(&hash_table, &i, &j, pos1);
      graph_insert_edge(&Gtmp1, i, j, 0.0);
    }
  }
  egraph_copy(EG, &EGtmp1);
  graph_to_egraph(&Gtmp1, &EGtmp1); // <- once there was a segfault here...
  egraph_plot(EG, &EGtmp1);
  egraph_delete(&EGtmp1);
  graph_delete(&Gtmp1);

}

void solve_kruskal(graph* G, egraph* EG, double *incumbent) {
   int i, j, pos;

  int status;
  CPXENVptr env = NULL;
  CPXLPptr lp = NULL;
  char* probname = "tsp";

  status = cplex_create_problem(&env, &lp, probname);
  printf("lp problem created\n");

   /* Turn on output to the screen */
   status = CPXsetintparam (env, CPX_PARAM_SCRIND, CPX_ON);
   /* Turn on data checking */
   status = CPXsetintparam (env, CPX_PARAM_DATACHECK, CPX_ON);

  int n = (*G).n;
  int numcols = n * (n - 1) / 2;
  //int numrows = n;

  cplex_table hash_table;
  cplex_table_init(&hash_table, numcols);
  cplex_table_populate(&hash_table, G);

  status = cplex_setup_problem(env, lp, G, &hash_table);
  printf("problem filled in\n");



  // variabili per memorizzazione soluzione
  //int cur_numrows;
  int cur_numcols;
  double objval;
  double x[numcols];
  //double slack[numrows];



  // variabili per ricerca sottocicli
  int neighbour1[n];
  int neighbour2[n];

  int starter;
  int current;
  int next;
  int mark;
  int num_of_cycles;
  int termination;
  int subtour_labels[n];
  int successors[n];
  // bisogna tenere conto di chi sia il successore di ogni nodo di
  // un sottociclo, in modo tale da conoscere rapidamente quali sono i lati
  //  del sottociclo: questa informazione ci serve per costruire i SEC!

  int count;
  int vertices[n];



  num_of_cycles = 0;
  while (num_of_cycles != 1) {

    // #1 RISOLUZIONE PROBLEMA CON CPLEX
    status = CPXmipopt(env, lp);


    // #2 MEMORIZZAZIONE SOLUZIONE
    //cur_numrows = CPXgetnumrows(env, lp);
    cur_numcols = CPXgetnumcols(env, lp);

    //printf("numrows = %d, numcols = %d\n", cur_numrows, cur_numcols);
    printf("numcols = %d\n", cur_numcols);

    status  = CPXgetobjval(env, lp, &objval);
    printf("Solution value  = %f\n\n", objval);

    status = CPXgetx(env, lp, x, 0, cur_numcols-1);
    //status = CPXgetslack(env, lp, slack, 0, cur_numrows-1);

    // #3 RICERCA SOTTOCICLI
    for(i = 0; i < cur_numcols; i++) {
      // attenzione: occhio a come arrotonda e verifica
      // se ci sono soluzioni buone ma con valore di qualche lato < 0.5
      x[i] = rint(x[i]);
    }

    int chosen[n];
    int vs[n], ss[n];
    int k = 0;
    for (i = 0; i < cur_numcols; ++i) {
      if (x[i] >= 0.9) {
        chosen[k++] = i+1;
      }
    }

    for (i = 0; i < n; ++i) {
      subtour_labels[i] = i+1;
    }

    int flag[n];
    int u,v, comp1, comp2;
    memset(flag, 0, sizeof(flag));

    for (i = 1; i <= n-2 ; ++i) {

      vertices_from_pos(&hash_table, &u, &v, chosen[0]);
      double min_cost = graph_get_edge_cost(G, u, v);
      vs[0] = u;
      ss[0] = v;
      int min_index = 0;
      for (j = 1; j < n; ++j) {

        vertices_from_pos(&hash_table, &u, &v, chosen[j]);
        vs[j] = u;
        ss[j] = v;
        if (flag[j] == 0 && graph_get_edge_cost(G, u, v) < min_cost) {
          min_cost = graph_get_edge_cost(G, u, v);
          min_index = j;
        }
      }

      printf("chosen: %d\n", chosen[min_index]);
      vertices_from_pos(&hash_table, &u, &v, chosen[min_index]);
      flag[min_index] = 1;

      comp1 = subtour_labels[u-1];
      comp2 = subtour_labels[v-1];

      for (j = 1; j <= n; ++j) {
        if (subtour_labels[j-1] == comp2) {
          subtour_labels[j-1] = comp1;
        }
      }

      printf("adding constraint:\n");
      int count = 0;
      for (i = 0; i < n; i++) {
        if (subtour_labels[i-1] == comp1) {
          printf("%d ", i);
          vertices[count] = vs[i];
          successors[count] = ss[i];
          count++;
        }
      }
      printf("\n");
      if (count < n) {
        status = cplex_add_SEC(env, lp, &hash_table, vertices, successors, count);
      }


    }  // end for

    // memorizzazione neighbours (ogni nodo ne ha due)
    /*for (i = 0; i < n; i++) {
      neighbour1[i] = 0;
      neighbour2[i] = 0;
    }

    // alla fine del ciclo neighbour1[i-1] e neighbour2[i-1]
    // conterranno i due nodi vicini al nodo i
    for (pos = 1; pos <= cur_numcols; pos++) {
      if (x[pos-1] == 1.0) { // oppure meglio if > .0
        vertices_from_pos(&hash_table, &i, &j, pos);

        if (neighbour1[i-1] == 0)
          neighbour1[i-1] = j;
        else
          neighbour2[i-1] = j;

        if (neighbour1[j-1] == 0)
          neighbour1[j-1] = i;
        else
          neighbour2[j-1] = i;

      }
    }

    // assegnazione delle labels ai nodi
    // (nodi appartenenti allo stesso ciclo hanno la stessa label)
    mark = 1;
    starter = 1;
    termination = FALSE;
    for (i = 0; i < n; i++) {
      subtour_labels[i] = 0;
    }


    while (!termination) {
    
      // come fase iniziale, non marchiamo lo starter,
      // sennò nel ciclo while che seguirà non si potrà mai ritornare in esso
      // visto che di volta in volta si salta su un nodo non marcato
      current = neighbour1[starter-1];
      successors[starter-1] = current; // regitr. succ.
      printf("%d ", starter);
      printf("%d ", current);

      subtour_labels[current-1] = mark;
      next = (neighbour1[current-1] != starter)
                    ? neighbour1[current-1]
                    : neighbour2[current-1];
      successors[current-1] = next; // regitr. succ.

      while (next != starter) {
        printf("%d ", next);
        subtour_labels[next-1] = mark;
        current = next;
        next = (subtour_labels[neighbour1[current-1]-1] == 0) 
                    ? neighbour1[current-1]
                    : neighbour2[current-1];
        successors[current-1] = next;
      }

      printf("\n");
      subtour_labels[starter-1] = mark;
      successors[current-1] = starter; // regitr. succ.


      termination = TRUE;
      for (starter = 1; starter <= n; starter++) {
        printf("%d ", starter);
        if (subtour_labels[starter-1] == 0) {
          mark++;
          termination = FALSE;
          break;
        }
      }
    }
    num_of_cycles = mark;


    // #4 AGGIUNTA NUOVI VINCOLI SEC
    for (mark = 1; mark <= num_of_cycles; mark++) {
      // per ogni label, cerco tutti i vertici che hanno quella stessa label
      // e aggiungo un SEC su quei vertici
      count = 0;
      for (i = 1; i <= n; i++) {
        if (subtour_labels[i-1] == mark) {
          //printf("%d ", i);
          vertices[count] = i;
          count++;
        }
      }
      //printf("\n");
      if (count < n) {
        status = cplex_add_SEC(env, lp, &hash_table, vertices, successors, count);
      }
    }*/

  }

  /*
  // richiama mipopt
  // risolvi problema con cplex
  status = CPXmipopt(env, lp);

  // soluzione
  cur_numrows = CPXgetnumrows(env, lp);
  cur_numcols = CPXgetnumcols(env, lp);

  printf("numrows = %d, numcols = %d\n", cur_numrows, cur_numcols);

  status  = CPXgetobjval(env, lp, &objval);
  printf("Solution value  = %f\n\n", objval);

  status = CPXgetx(env, lp, x, 0, cur_numcols-1);
  status = CPXgetslack(env, lp, slack, 0, cur_numrows-1);
  */
  /* Finally, write a copy of the problem to a file. */
  //status = CPXwriteprob (env, lp, "./lpex1.lp", NULL);

  // plotta grafo
  /*graph Gtmp1;
  egraph EGtmp1;
  int pos1;

  graph_init(&Gtmp1, n);
  egraph_init(&EGtmp1, n);

  for (pos1 = 1; pos1 <= cur_numcols; pos1++) {
    if (x[pos1-1] == 1.0) {
      vertices_from_pos(&hash_table, &i, &j, pos1);
      graph_insert_edge(&Gtmp1, i, j, 0.0);
    }
  }
  egraph_copy(EG, &EGtmp1);
  graph_to_egraph(&Gtmp1, &EGtmp1); // <- once there was a segfault here...
  egraph_plot(EG, &EGtmp1);
  egraph_delete(&EGtmp1);
  graph_delete(&Gtmp1);*/
}

int cplex_create_problem(CPXENVptr *env, CPXLPptr *lp, char *probname) {
  int status;

  *env = CPXopenCPLEX(&status);

  *lp = CPXcreateprob(*env, &status, probname);

  status = CPXsetintparam(*env, CPX_PARAM_LPMETHOD, CPX_ALG_AUTOMATIC);

  return status;

}

int cplex_setup_problem(CPXENVptr env, CPXLPptr lp, graph* G, cplex_table* hash_table) {
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

  status = CPXaddrows(env, lp, 0, rcnt, nzcnt, rhs, sense, rmatbeg, rmatind, rmatval, NULL, NULL);

  return status;
}


int cplex_add_SEC(CPXENVptr env, CPXLPptr lp, cplex_table *hash_table,
      int* vertices, int* successors, int n) {
  int i, pos, status;

  int rmatind[n];
  for (i = 0; i < n; i++) {
    pos_from_vertices(hash_table, vertices[i], successors[vertices[i]-1], &pos);
    rmatind[i] = pos-1;
  }

  double rmatval[n];
  for (i = 0; i < n; i++) {
    rmatval[i] = 1.0;
  }

  double rhs[1];
  rhs[0] = n-1;

  char sense[1];
  sense[0] = 'L';

  int rmatbeg[1];
  rmatbeg[0] = 0;

  status = CPXaddrows(env, lp, 0, 1, n, rhs, sense, rmatbeg, rmatind, rmatval, NULL, NULL);

  return status;

}
