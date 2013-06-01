#include "egraph.h"



void egraph_init(egraph* EG, int n) {

  if (n == 0) {
    EG->n = 0;
    EG->V = NULL;
    EG->E = NULL;
  }
  else if (n >= 1) {
    EG->n = n;
    EG->V = (egraph_node*)calloc(n, sizeof(egraph_node));
    EG->E = (egraph_edge*)calloc(n * (n + 1) / 2, sizeof(egraph_edge));

#ifdef DEBUG
    if (n < 0) {
      printf("error: egraph_init\n");
      exit(1);
    }
#endif

    EG->max_x = X_MAX;
    EG->max_y = Y_MAX;
    EG->min_x = X_MIN;
    EG->min_y = Y_MIN;
  }

}



void egraph_delete(egraph* EG) {

  free(EG->V);
  free(EG->E);
  EG->V = NULL;
  EG->E = NULL;
  EG->n = 0;

}



void egraph_copy(egraph* FROM, egraph* TO) {

  int i;
  int n = FROM->n;

  egraph_delete(TO);
  egraph_init(TO, n);
  for (i = 0; i < n; i++) {
    TO->V[i].x = FROM->V[i].x;
    TO->V[i].y = FROM->V[i].y;
    TO->V[i].deg = FROM->V[i].deg;
  }
  for (i = 0; i < n * (n + 1) / 2; i++) {
    TO->E[i].flag = FROM->E[i].flag;
    TO->E[i].cost = FROM->E[i].cost;
  }

  TO->max_x = FROM->max_x;
  TO->max_y = FROM->max_y;
  TO->min_x = FROM->min_x;
  TO->min_y = FROM->min_y;

}



void egraph_random(egraph* EG) {

  int i, j;
  int n = (*EG).n;

  for (i = 0; i < n; i++) {
    EG->V[i].deg = n-1;
    EG->V[i].x = (double)(rand()) / ((double)RAND_MAX + 1.0);
    EG->V[i].y = (double)(rand()) / ((double)RAND_MAX + 1.0);
  }
  for (i = 0; i < n; i++) {
    for (j = i+1; j < n; j++) {
      EG->E[ j*(j+1) / 2 + i].flag = 1;
      EG->E[ j*(j+1) / 2 + i].cost = sqrt(pow( EG->V[i].x - EG->V[j].x, 2 ) + pow( EG->V[i].y - EG->V[j].y, 2 ));
    }
  }

  EG->max_x = 1.1;
  EG->max_y = 1.1;
  EG->min_x = -0.1;
  EG->min_y = -0.1;

}



void egraph_insert_edge(egraph* EG, int u, int v, double cost) {

#ifdef DEBUG
  if (u < 0 || v < 0 || u >= EG->n || v >= EG->n || u == v || egraph_adjacent_nodes(EG, u, v)) {
    printf("error: egraph_insert_edge: %d %d\n", u, v);
    exit(1);
  }
#endif

  (u > v) ? ( EG->E[ u*(u+1)/2 + v ].flag = 1 ) : ( EG->E[ v*(v+1)/2 + u].flag = 1 );
  (u > v) ? ( EG->E[ u*(u+1)/2 + v ].cost = cost ) : ( EG->E[ v*(v+1)/2 + u].cost = cost );
  EG->V[u].deg++;
  EG->V[v].deg++;
      
}



void egraph_remove_edge(egraph* EG, int u, int v) {

#ifdef DEBUG
  if (u < 0 || v < 0 || u >= EG->n || v >= EG->n || u == v || !egraph_adjacent_nodes(EG, u, v)) {
    printf("error: egraph_remove_edge: %d %d\n", u, v);
    exit(1);
  }
#endif

  (u > v) ? ( EG->E[ u*(u+1)/2 + v ].flag = 0 ) : ( EG->E[ v*(v+1)/2 + u].flag = 0 );
  (u > v) ? ( EG->E[ u*(u+1)/2 + v ].cost = 0.0 ) : ( EG->E[ v*(v+1)/2 + u].cost = 0.0 );
  EG->V[u].deg--;
  EG->V[v].deg--;

}



void egraph_set_edge_cost(egraph* EG, int u, int v, double cost) {

#ifdef DEBUG
  if ((u < 0)      || (v < 0)      ||
      (u >= EG->n) || (v >= EG->n) ||
      (u == v)     ||
      !egraph_adjacent_nodes(EG, u, v)) {
    printf("error: egraph_set_edge_cost: %d %d\n", u, v);
    exit(1);
  }
#endif

  (u > v) ? ( EG->E[ u*(u+1)/2 + v ].cost = cost ) : ( EG->E[ v*(v+1)/2 + u ].cost = cost );

}



double egraph_get_edge_cost(egraph* EG, int u, int v) {

#ifdef DEBUG
  if (u < 0 || v < 0 || u >= EG->n || v >= EG->n || u == v || !egraph_adjacent_nodes(EG, u, v)) {
    printf("error: egraph_get_edge_cost: %d %d\n", u, v);
    exit(1);
  }
#endif

  return (u > v) ? EG->E[ u*(u+1)/2 + v ].cost : EG->E[ v*(v+1)/2 + u ].cost;

}



int egraph_adjacent_nodes(egraph* EG, int u, int v) {

#ifdef DEBUG
  if (u < 0 || v < 0 || u >= EG->n || v >= EG->n || u == v) {
    printf("error: egraph_adjacent_nodes: %d %d\n", u, v);
    exit(1);
  }
#endif

  return (u > v) ? EG->E[ u*(u+1)/2 + v ].flag : EG->E[ v*(v+1)/2 + u ].flag;
 
}



double egraph_get_cost(egraph* EG) {

  int i, j;
  double c = 0.0;
  int n = EG->n;

  for (i = 0; i < n; i++) {
    for (j = i+1; j < n; j++) {
      if (egraph_adjacent_nodes(EG, i, j)) {
        c += egraph_get_edge_cost(EG, i, j);
      }
    }
  }
  return c;

}



void egraph_plot(egraph* EG1, egraph* EG2, char* title) {

  int i, j, n1, n2, eg1_has_some_edge, eg2_has_some_edge;

  n1 = n2 = 0;
  if (EG1 != NULL)
    n1 = EG1->n;
  if (EG2 != NULL)
    n2 = EG2->n;

  eg1_has_some_edge = eg2_has_some_edge = 0;
  if (n1 > 0) {
    for (i = 0; i < n1 * (n1 + 1) / 2; i++) {
      if ((*EG1).E[i].flag == 1) {
	eg1_has_some_edge = 1;
	break;
      }
    }
  }
  if (n2 > 0) {
    for (i = 0; i < n2 * (n2 + 1) / 2; i++) {
      if ((*EG2).E[i].flag == 1) {
	eg2_has_some_edge = 1;
	break;
      }
    }
  }

  FILE* pipe = popen("gnuplot -persist", "w");

  fprintf(pipe, "set multiplot\n");
  //fprintf(pipe, "set size square\n");
  if (title != NULL) fprintf(pipe, "set title \"%s\"\n", title);
  fprintf(pipe, "set xrange [%.3f:%.3f]\n", EG1->min_x, EG1->max_x);
  fprintf(pipe, "set yrange [%.3f:%.3f]\n", EG1->min_y, EG1->max_y);
  fprintf(pipe, "set xlabel 'X'\n");
  fprintf(pipe, "set ylabel 'Y'\n");
  //fprintf(pipe, "unset xtics\n");
  //fprintf(pipe, "unset ytics\n");

  fprintf(pipe, "set style line 1 linetype 1 linecolor rgb \"grey\" pointtype 7\n");
  fprintf(pipe, "set style line 2 linetype 1 linecolor rgb \"grey\" linewidth 1\n");
  fprintf(pipe, "set style line 3 linetype 1 linecolor rgb \"red\" pointtype 7\n");
  fprintf(pipe, "set style line 4 linetype 1 linecolor rgb \"red\" linewidth 1\n");

  if (n1 > 0 && n2 > 0) {
    fprintf(pipe, "plot '-' using 1:2 with points linestyle 1 notitle");
    if (eg1_has_some_edge)
      fprintf(pipe, ", '' using 1:2 with lines linestyle 2 notitle");
    fprintf(pipe, ", '' using 1:2 with points linestyle 3 notitle");
    if (eg2_has_some_edge)
      fprintf(pipe, ", '' using 1:2 with lines linestyle 4 notitle");
    fprintf(pipe, ", '' using 1:2:3 with labels offset 0.5,0.5 notitle, '' using 1:2:3 with labels offset 0.5,0.5 notitle\n");
  }
  else if (n1 > 0 && n2 == 0) {
    fprintf(pipe, "plot '-' using 1:2 with points linestyle 1 notitle");
    if (eg1_has_some_edge)
      fprintf(pipe, ", '' using 1:2 with lines linestyle 2 notitle");
    fprintf(pipe, ", '' using 1:2:3 with labels offset 0.5,0.5 notitle\n");
  }
  else if (n1 == 0 && n2 > 0) {
    fprintf(pipe, "plot '-' using 1:2 with points linestyle 3 notitle");
    if (eg2_has_some_edge)
      fprintf(pipe, ", '' using 1:2 with lines linestyle 4 notitle");
    fprintf(pipe, ", '' using 1:2:3 with labels offset 0.5,0.5 notitle\n");
  }

  if (n1 > 0) {
    for (i = 0; i < n1; i++) {
      fprintf(pipe, "%f %f\n", EG1->V[i].x, EG1->V[i].y);
      fprintf(pipe, "\n");
    }
    fprintf(pipe, "e\n");

    if (eg1_has_some_edge) {
      for (i = 0; i < n1; i++) {
        for (j = i+1; j < n1; j++) {
          if (egraph_adjacent_nodes(EG1, i, j)) {
            fprintf(pipe, "%f %f\n", EG1->V[i].x, EG1->V[i].y);
            fprintf(pipe, "%f %f\n", EG1->V[j].x, EG1->V[j].y);
            fprintf(pipe, "\n");
          }
        }
      }
      fprintf(pipe, "e\n");
    }
  }

  if (n2 > 0) {
    for (i = 0; i < n2; i++) {
      fprintf(pipe, "%f %f\n", EG2->V[i].x, EG2->V[i].y);
      fprintf(pipe, "\n");
    }
    fprintf(pipe, "e\n");
  
    if (eg2_has_some_edge) {
      for (i = 0; i < n2; i++) {
        for (j = i+1; j < n2; j++) {
          if (egraph_adjacent_nodes(EG2, i, j)) {
            fprintf(pipe, "%f %f\n", EG2->V[i].x, EG2->V[i].y);
            fprintf(pipe, "%f %f\n", EG2->V[j].x, EG2->V[j].y);
            fprintf(pipe, "\n");
          }
        }
      }
      fprintf(pipe, "e\n");
    }
  }

  if (n1 > 0) {
    for (i = 0; i < n1; i++) {
      fprintf(pipe, "%f %f %d",  EG1->V[i].x, EG2->V[i].y, i+1);
      fprintf(pipe, "\n");
    }
    fprintf(pipe, "e\n");
  }
  if (n2 > 0) {
    for (i = 0; i < n2; i++) {
      fprintf(pipe, "%f %f %d", EG2->V[i].x, EG2->V[i].y, i+1);
      fprintf(pipe, "\n");
    }
    fprintf(pipe, "e\n");
  }

  fflush(pipe);

}



void egraph_to_graph(egraph* EG, graph* G) {

  int i;
  int n = EG->n;

  graph_delete(G);
  graph_init(G, n);

  for (i = 0; i < n; i++) {
    G->V[i].deg = EG->V[i].deg;
  }
  for (i = 0; i < n * (n + 1) / 2; i++) {
    G->E[i].flag = EG->E[i].flag;
    G->E[i].cost = EG->E[i].cost;
    G->E[i].constr = FREE;
  }

}



void graph_to_egraph(graph* G, egraph* EG) {

#ifdef DEBUG
  if (G->n != EG->n) {
    printf("error: graph_to_egraph\n");
    exit(1);
  }
#endif

  int i;
  int n = G->n;

  for (i = 0; i < n; i++) {
    EG->V[i].deg = G->V[i].deg;
  }
  for (i = 0; i < n * (n + 1) / 2; i++) {
    EG->E[i].flag = G->E[i].flag;
  }

}



void cycle_to_egraph(cycle* C, egraph* EG) {

#ifdef DEBUG
  if (C->n != EG->n) {
    printf("error: cycle_to_egraph\n");
    exit(1);
  }
#endif

  int i;
  int n = C->n;

  for (i = 0; i < n * (n + 1) / 2; i++) {
    EG->E[i].flag = 0;
    EG->E[i].cost = 0;
  }
  for (i = 0; i < n; i++) {
    egraph_insert_edge(EG, C->nodes[i], C->nodes[(i+1)%n], 0.0);
  }

}



void tree_to_egraph(tree* T, egraph* EG) {

#ifdef DEBUG
  if (T->n != EG->n) {
    printf("error: tree_to_egraph\n");
    exit(1);
  }
#endif

  int i;
  int n = T->n;

  for (i = 0; i < n * (n + 1) / 2; i++) {
    EG->E[i].flag = 0;
    EG->E[i].cost = 0.0;
  }
  for (i = 0; i < n; i++) {
    EG->V[i].deg = T->V[i].deg;
    if (T->V[i].pred >= 0)
      egraph_insert_edge(EG, T->V[i].pred, i, 0.0);
  }

}



void onetree_to_egraph(onetree* OT, egraph* EG) {

#ifdef DEBUG
  if (OT->n != EG->n) {
    printf("error: onetree_to_egraph\n");
    exit(1);
  }
#endif

  int i;
  int n = OT->n;

  for (i = 0; i < n * (n + 1) / 2; i++) {
    EG->E[i].flag = 0;
    EG->E[i].cost = 0.0;
  }
  for (i = 0; i < n; i++) {
    EG->V[i].deg = OT->V[i].deg;
    if (OT->V[i].pred >= 0)
      egraph_insert_edge(EG, OT->V[i].pred, i, 0.0);
  }

}



void graph_plot(graph* G, egraph* EG, char* title) {

  egraph EG_TMP;
  egraph_init(&EG_TMP, 0);
  egraph_copy(EG, &EG_TMP);
  graph_to_egraph(G, &EG_TMP);
  egraph_plot(EG, &EG_TMP, title);
  egraph_delete(&EG_TMP);

}



void tree_plot(tree* T, egraph* EG, char* title) {

  egraph EG_TMP;
  egraph_init(&EG_TMP, 0);
  egraph_copy(EG, &EG_TMP);
  tree_to_egraph(T, &EG_TMP);
  egraph_plot(EG, &EG_TMP, title);
  egraph_delete(&EG_TMP);

}



void onetree_plot(onetree* OT, egraph* EG, char* title) {

  egraph EG_TMP;
  egraph_init(&EG_TMP, 0);
  egraph_copy(EG, &EG_TMP);
  onetree_to_egraph(OT, &EG_TMP);
  egraph_plot(EG, &EG_TMP, title);
  egraph_delete(&EG_TMP);

}


 
void cycle_plot(cycle* C, egraph* EG, char* title) {

  egraph EG_TMP;
  egraph_init(&EG_TMP, 0);
  egraph_copy(EG, &EG_TMP);
  cycle_to_egraph(C, &EG_TMP);
  egraph_plot(EG, &EG_TMP, title);
  egraph_delete(&EG_TMP);

}



/*
 * print egraph as (diagonal) matrix of costs
 */
void egraph_print(egraph *EG) {
  int i, j;
  for (i = 0; i < EG->n; ++i) {
    for (j = 0; j < i; ++j) {
     printf("%f ", egraph_get_edge_cost(EG, i, j));
    }
    printf("\n");
  }

}

