#include "graph.h"

void initGraph(graph* G, int n) {
  int i;

  (*G).n = n;
  (*G).V = (node*)malloc(sizeof(node) * n);
  (*G).E = (edge*)malloc(sizeof(edge) * n * (n + 1) / 2);
  /*for (i = 0; i < n; i++)
    (*G).V[i].x = (*G).V[i].y = (*G).V[i].deg = 0;
  for (i = 0; i < n * (n + 1) / 2; i++)
    (*G).E[i].flag = (*G).E[i].cost = 0;/**/
  // since all the fields are set to 0, a badass malloc should be faster
  memset((*G).V, 0, sizeof(node) * n);
  memset((*G).E, 0, sizeof(edge) * n * (n + 1) / 2);/**/
}

void deleteGraph(graph* G) {
  free((*G).V);
  free((*G).E);
  (*G).n = 0;
  (*G).V = NULL;
  (*G).E = NULL;
}

void randomGraph(graph* G) {
  int i, j;
  int n = (*G).n;

  deleteGraph(G);
  initGraph(G, n);

  for (i = 0; i < n; i++) {
    (*G).V[i].deg = n-1;
    (*G).V[i].x = (double)(rand()) / ((double)RAND_MAX + 1.0);
    (*G).V[i].y = (double)(rand()) / ((double)RAND_MAX + 1.0);
  }
  for (i = 0; i < n; i++) {
    for (j = i+1; j < n; j++) {
      (*G).E[ j*(j+1) / 2 + i].flag = 1;
      (*G).E[ j*(j+1) / 2 + i].cost = sqrt(pow( (*G).V[i].x - (*G).V[j].x, 2 ) + pow( (*G).V[i].y - (*G).V[j].y, 2 ));
    }
  }
}

void copyGraph(graph* FROM, graph* TO) {
  int i;

  int n = (*FROM).n;
  deleteGraph(TO);
  initGraph(TO, n);
  for (i = 0; i < n; i++) {
    (*TO).V[i].x = (*FROM).V[i].x;
    (*TO).V[i].y = (*FROM).V[i].y;
    (*TO).V[i].deg = (*FROM).V[i].deg;
  }
  for (i = 0; i < n * (n + 1) / 2;i++) {
    (*TO).E[i].flag = (*FROM).E[i].flag;
    (*TO).E[i].cost = (*FROM).E[i].cost;
  }
}

void insert_edge(graph* G, int u, int v) {
  if (adjacent(G, u, v))
    return;
  (u > v) ? ( (*G).E[ u*(u-1)/2 + v-1 ].flag = 1 ) : ( (*G).E[ v*(v-1)/2 + u-1].flag = 1 );
  (*G).V[u-1].deg++;
  (*G).V[v-1].deg++;
}

void remove_edge(graph* G, int u, int v) {
  if (!adjacent(G, u, v))
    return;
  (u > v) ? ( (*G).E[ u*(u-1)/2 + v-1 ].flag = 0 ) : ( (*G).E[ v*(v-1)/2 + u-1].flag = 0 );
  (u > v) ? ( (*G).E[ u*(u-1)/2 + v-1 ].cost = 0 ) : ( (*G).E[ v*(v-1)/2 + u-1].cost = 0 );
  (*G).V[u-1].deg--;
  (*G).V[v-1].deg--;
}

inline
void set_node_x(graph* G, int v, double x) {
  (*G).V[v-1].x = x;
}

inline
void set_node_y(graph* G, int v, double y) {
  (*G).V[v-1].y = y;
}

inline
double get_node_x(graph* G, int v) {
  return (*G).V[v-1].x;
}

inline
double get_node_y(graph* G, int v) {
  return (*G).V[v-1].y;
}

inline
int get_node_deg(graph* G, int v) {
  return (*G).V[v-1].deg;
}

inline
void set_edge_cost(graph* G, int u, int v, double cost) {
  (u > v) ? ( (*G).E[ u*(u-1)/2 + v-1 ].cost = cost ) : ( (*G).E[ v*(v-1)/2 + u-1 ].cost = cost );
}

inline
double get_edge_cost(graph* G, int u, int v) {
  return (u > v) ? (*G).E[ u*(u-1)/2 + v-1 ].cost : (*G).E[ v*(v-1)/2 + u-1 ].cost;
}

inline
int adjacent(graph* G, int u, int v) {
  return (u > v) ? (*G).E[ u*(u-1)/2 + v-1 ].flag : (*G).E[ v*(v-1)/2 + u-1 ].flag;
}

double get_graph_cost(graph* G) {
  int i, j;
  double c;

  int n = (*G).n;
  c = 0;
  for (i = 1; i <= n; i++) {
    for (j = 1; j < i; j++) {
      if (adjacent(G, i, j)) {
        c += get_edge_cost(G, i, j);
      }
    }
  }
  return c;
}

void plotGraph(graph* G1, graph* G2, char* opt1, char* opt2) {
  int i, j, v, n1, n2;

  if (G1 != NULL) {
    n1 = (*G1).n;
  }
  if (G2 != NULL) {
    n2 = (*G2).n;
  }

  int g1_has_some_edge = 0;
  int g2_has_some_edge = 0;

  if (G1 != NULL) {
    for (i = 0; i < n1 * (n1 + 1) / 2; i++) {
      if ((*G1).E[i].flag == 1) {
	g1_has_some_edge = 1;
	break;
      }
    }
  }
  if (G2 != NULL) {
    for (i = 0; i < n2 * (n2 + 1) / 2; i++) {
      if ((*G2).E[i].flag == 1) {
	g2_has_some_edge = 1;
	break;
      }
    }
  }

  FILE* pipe = popen("gnuplot -persist", "w");

  fprintf(pipe, "set multiplot\n");
  fprintf(pipe, "set size square\n");
  fprintf(pipe, "set xrange [-0.010:1.010]\n");
  fprintf(pipe, "set yrange [-0.010:1.010]\n");
  fprintf(pipe, "set xlabel 'X'\n");
  fprintf(pipe, "set ylabel 'Y'\n");
  fprintf(pipe, "unset xtics\n");
  fprintf(pipe, "unset ytics\n");

  if (strcmp(opt1, "default") == 0) {
    fprintf(pipe, "set style line 1 linetype 1 linecolor rgb \"grey\" pointtype 7\n");
    fprintf(pipe, "set style line 2 linetype 1 linecolor rgb \"grey\" linewidth 1\n");
    fprintf(pipe, "set style line 3 linetype 1 linecolor rgb \"red\" pointtype 7\n");
    fprintf(pipe, "set style line 4 linetype 1 linecolor rgb \"red\" linewidth 1\n");
  }
  if (strcmp(opt1, "mstree") == 0) {
    fprintf(pipe, "set style line 1 linetype 1 linecolor rgb \"grey\" pointtype 7\n");
    fprintf(pipe, "set style line 2 linetype 1 linecolor rgb \"grey\" linewidth 1\n");
    fprintf(pipe, "set style line 3 linetype 1 linecolor rgb \"red\" pointtype 7\n");
    fprintf(pipe, "set style line 4 linetype 1 linecolor rgb \"red\" linewidth 1\n");
    fprintf(pipe, "set style line 5 linetype 1 linecolor rgb \"black\" pointtype 7\n");
  }
  if (strcmp(opt1, "onetree") == 0) {
    fprintf(pipe, "set style line 1 linetype 1 linecolor rgb \"grey\" pointtype 7\n");
    fprintf(pipe, "set style line 2 linetype 1 linecolor rgb \"grey\" linewidth 1\n");
    fprintf(pipe, "set style line 3 linetype 1 linecolor rgb \"red\" pointtype 7\n");
    fprintf(pipe, "set style line 4 linetype 1 linecolor rgb \"red\" linewidth 1\n");
    fprintf(pipe, "set style line 5 linetype 1 linecolor rgb \"blue\" pointtype 7 linewidth 1\n");
    fprintf(pipe, "set style line 6 linetype 1 linecolor rgb \"black\" pointtype 7\n");
  }

  if (G1 != NULL) {
    fprintf(pipe, "plot '-' using 1:2 with points linestyle 1 notitle");
    if (g1_has_some_edge)
      fprintf(pipe, ", '' using 1:2 with lines linestyle 2 notitle");
    if (G2 != NULL) {
      fprintf(pipe, ", '' using 1:2 with points linestyle 3 notitle");
      if (g2_has_some_edge)
	fprintf(pipe, ", '' using 1:2 with lines linestyle 4 notitle");
    }
  }
  else if (G2 != NULL) {
    fprintf(pipe, "plot '-' using 1:2 with points linestyle 3 notitle");
    if (g2_has_some_edge)
	fprintf(pipe, ", '' using 1:2 with lines linestyle 4 notitle");
  }

  if (strcmp(opt1, "mstree") == 0) {
    fprintf(pipe, ", '' using 1:2 with points linestyle 5 pointsize 2 notitle");
  }
  if (strcmp(opt1, "onetree") == 0) {
    if (g2_has_some_edge)
      fprintf(pipe, ", '' using 1:2 with linespoints linestyle 5 notitle");
    fprintf(pipe, ", '' using 1:2 with points linestyle 6 pointsize 2 notitle");
  }

  if (G1 != NULL)
    fprintf(pipe, ", '' using 1:2:3 with labels offset 0.5,0.5 notitle");
  if (G2 != NULL)
    fprintf(pipe, ", '' using 1:2:3 with labels offset 0.5,0.5 notitle");
  fprintf(pipe, "\n");

  if (G1 != NULL) {
    for (i = 1; i <= n1; i++) {
      fprintf(pipe, "%f %f\n", get_node_x(G1, i), get_node_y(G1, i));
      fprintf(pipe, "\n");
    }
    fprintf(pipe, "e\n");
  }
  if (G1 != NULL && g1_has_some_edge) {
    for (i = 1; i <= n1; i++) {
      for (j = i+1; j <= n1; j++) {
	if (adjacent(G1, i, j)) {
	  fprintf(pipe, "%f %f\n", get_node_x(G1, i), get_node_y(G1, i));
	  fprintf(pipe, "%f %f\n", get_node_x(G1, j), get_node_y(G1, j));
	  fprintf(pipe, "\n");
	}
      }
    }
    fprintf(pipe, "e\n");
  }

 if (G2 != NULL) {
    for (i = 1; i <= n2; i++) {
      fprintf(pipe, "%f %f\n", get_node_x(G2, i), get_node_y(G2, i));
      fprintf(pipe, "\n");
    }
    fprintf(pipe, "e\n");
  }
  if (G2 != NULL && g2_has_some_edge) {
    for (i = 1; i <= n2; i++) {
      for (j = i+1; j <= n2; j++) {
	if (adjacent(G2, i, j)) {
	  fprintf(pipe, "%f %f\n", get_node_x(G2, i), get_node_y(G2, i));
	  fprintf(pipe, "%f %f\n", get_node_x(G2, j), get_node_y(G2, j));
	  fprintf(pipe, "\n");
	}
      }
    }
    fprintf(pipe, "e\n");
  }

  if (strcmp(opt1, "mstree") == 0) {
    fprintf(pipe, "%f %f\n", get_node_x(G2, atoi(opt2)), get_node_y(G2, atoi(opt2)));
    fprintf(pipe, "e\n");
  }

  if (strcmp(opt1, "onetree") == 0) {

    graph P;
    initGraph(&P, 1);
    for (v = 2; v <= n2; v++) {
      if (adjacent(G2, 1, v))
	break;
    }
    remove_edge(G2, 1, v);
    compute_path(G2, &P, 1, v, 0);

    for (i = 1; i <= n2; i++) {
      for (j = i+1; j <= n2; j++) {
	if (adjacent(&P, i, j)) {
	  fprintf(pipe, "%f %f\n", get_node_x(&P, i), get_node_y(&P, i));
	  fprintf(pipe, "%f %f\n", get_node_x(&P, j), get_node_y(&P, j));
	  fprintf(pipe, "\n");
	}
      }
    }
    fprintf(pipe, "%f %f\n", get_node_x(G2, 1), get_node_y(G2, 1));
    fprintf(pipe, "%f %f\n", get_node_x(G2, v), get_node_y(G2, v));
    fprintf(pipe, "e\n");
    insert_edge(G2, 1, v);
    deleteGraph(&P);

    fprintf(pipe, "%f %f\n", get_node_x(G2, 1), get_node_y(G2, 1));
    fprintf(pipe, "e\n");
  }

  if (G1 != NULL) {
    for (i = 1; i <= n1; i++) {
      fprintf(pipe, "%f %f %d", get_node_x(G1, i), get_node_y(G1, i), i);
      fprintf(pipe, "\n");
    }
    fprintf(pipe, "e\n");
  }

  if (G2 != NULL) {
    for (i = 1; i <= n2; i++) {
      fprintf(pipe, "%f %f %d", get_node_x(G2, i), get_node_y(G2, i), i);
      fprintf(pipe, "\n");
    }
    fprintf(pipe, "e\n");
  }

  fflush(pipe);
}

void print_graph(graph *G) {
  int i, j, n = G->n;

  for (i = 1; i <= n; ++i) {
    for (j = 1; j < i; ++j) {
      printf("%f ", get_edge_cost(G, i, j));
    }
    printf("\n");
  }
}
