#include "egraph.h"

void egraph_init(egraph* EG, int n) {
  if (n >= 1) {
    (*EG).n = n;
    (*EG).V = (egraph_node*)calloc(n, sizeof(egraph_node));
    (*EG).E = (egraph_edge*)calloc(n * (n + 1) / 2, sizeof(egraph_edge));


    EG->max_x = X_MAX;
    EG->max_y = Y_MAX;
    EG->min_x = X_MIN;
    EG->min_y = Y_MIN;
  }
  else {
    printf("error: egraph_init: %d\n", n);
    exit(EXIT_FAILURE);
  }
}

void egraph_delete(egraph* EG) {

  free((*EG).V);
  free((*EG).E);
  (*EG).V = NULL;
  (*EG).E = NULL;
  (*EG).n = 0;
}

void egraph_copy(egraph* FROM, egraph* TO) {
  int i;

  int n = (*FROM).n;
  egraph_delete(TO);
  egraph_init(TO, n);
  for (i = 0; i < n; i++) {
    (*TO).V[i].x = (*FROM).V[i].x;
    (*TO).V[i].y = (*FROM).V[i].y;
    (*TO).V[i].deg = (*FROM).V[i].deg;
  }
  for (i = 0; i < n * (n + 1) / 2; i++) {
    (*TO).E[i].flag = (*FROM).E[i].flag;
    (*TO).E[i].cost = (*FROM).E[i].cost;
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
    (*EG).V[i].deg = n-1;
    (*EG).V[i].x = (double)(rand()) / ((double)RAND_MAX + 1.0);
    (*EG).V[i].y = (double)(rand()) / ((double)RAND_MAX + 1.0);
  }
  for (i = 0; i < n; i++) {
    for (j = i+1; j < n; j++) {
      (*EG).E[ j*(j+1) / 2 + i].flag = 1;
      (*EG).E[ j*(j+1) / 2 + i].cost = sqrt(pow( (*EG).V[i].x - (*EG).V[j].x, 2 ) + pow( (*EG).V[i].y - (*EG).V[j].y, 2 ));
    }
  }

  EG->max_x =  1.1;
  EG->max_y =  1.1;
  EG->min_x = -0.1;
  EG->min_y = -0.1;
}

void egraph_set_node_x(egraph* EG, int v, double x) {

  if( v >= 1 && v <= (*EG).n) {
    (*EG).V[v-1].x = x;
  }
  else {
    printf("error: egraph_set_node_x: %d\n", v);
    exit(EXIT_FAILURE);
  }
}

void egraph_set_node_y(egraph* EG, int v, double y) {

  if( v >= 1 && v <= (*EG).n) {
    (*EG).V[v-1].y = y;
  }
  else {
    printf("error: egraph_set_node_y: %d\n", v);
    exit(EXIT_FAILURE);
  }
}

double egraph_get_node_x(egraph* EG, int v) {

  if( v >= 1 && v <= (*EG).n) {
    return (*EG).V[v-1].x;
  }
  else {
    printf("error: egraph_get_node_x: %d\n", v);
    exit(EXIT_FAILURE);
  }
}

double egraph_get_node_y(egraph* EG, int v) {

  if( v >= 1 && v <= (*EG).n) {
    return (*EG).V[v-1].y;
  }
  else {
    printf("error: egraph_get_node_y: %d\n", v);
    exit(EXIT_FAILURE);
  }
}

void egraph_insert_edge(egraph* EG, int u, int v, double cost) {
  // printf("%d %d\n", u, v);
  if (!egraph_adjacent_nodes(EG, u, v)) {
    (u > v) ? ( (*EG).E[ u*(u-1)/2 + v-1 ].flag = 1 )    : ( (*EG).E[ v*(v-1)/2 + u-1].flag = 1 );
    (u > v) ? ( (*EG).E[ u*(u-1)/2 + v-1 ].cost = cost ) : ( (*EG).E[ v*(v-1)/2 + u-1].cost = cost );
    (*EG).V[u-1].deg++;
    (*EG).V[v-1].deg++;
  } else {
    printf("error: egraph_insert_edge: (%d, %d)\n", u, v);
    exit(EXIT_FAILURE);
  }
}

void egraph_remove_edge(egraph* EG, int u, int v) {

  if( u >= 1 && v >= 1 && u <= (*EG).n && v <= (*EG).n && u != v && egraph_adjacent_nodes(EG, u, v)) {
    (u > v) ? ( (*EG).E[ u*(u-1)/2 + v-1 ].flag = 0 ) : ( (*EG).E[ v*(v-1)/2 + u-1].flag = 0 );
    (u > v) ? ( (*EG).E[ u*(u-1)/2 + v-1 ].cost = 0.0 ) : ( (*EG).E[ v*(v-1)/2 + u-1].cost = 0.0 );
    (*EG).V[u-1].deg--;
    (*EG).V[v-1].deg--;
  }
  else {
    printf("error: egraph_remove_edge : (%d, %d)\n", u, v);
    exit(EXIT_FAILURE);
  }
}

void egraph_set_edge_cost(egraph* EG, int u, int v, double cost) {

  if( u >= 1 && v >= 1 && u <= (*EG).n && v <= (*EG).n && u != v && egraph_adjacent_nodes(EG, u, v)) {
    (u > v) ? ( (*EG).E[ u*(u-1)/2 + v-1 ].cost = cost ) : ( (*EG).E[ v*(v-1)/2 + u-1 ].cost = cost );
  }
  else {
    printf("error: egraph_set_edge_cost : (%d, %d)\n", u, v);
    exit(EXIT_FAILURE);
  }
}

double egraph_get_edge_cost(egraph* EG, int u, int v) {

  if( u >= 1 && v >= 1 && u <= (*EG).n && v <= (*EG).n && u != v && egraph_adjacent_nodes(EG, u, v)) {
    return (u > v) ? (*EG).E[ u*(u-1)/2 + v-1 ].cost : (*EG).E[ v*(v-1)/2 + u-1 ].cost;
  }
  else {
    printf("error: egraph_get_edge_cost: (%d, %d)\n", u, v);
    exit(EXIT_FAILURE);
  }
}

int egraph_get_node_deg(egraph* EG, int v) {

  if (v >= 1 && v <= (*EG).n) {
    return (*EG).V[v-1].deg;
  }
  else {
    printf("error: egraph_get_node_deg: %d\n", v);
    exit(EXIT_FAILURE);
  }
}

int egraph_adjacent_nodes(egraph* EG, int u, int v) {
  return (u > v) ? (*EG).E[ u*(u-1)/2 + v-1 ].flag : (*EG).E[ v*(v-1)/2 + u-1 ].flag;
}

double egraph_get_cost(egraph* EG) {
  int i, j;

  double c = 0.0;
  int n = (*EG).n;
  for (i = 1; i <= n; i++) {
    for (j = i+1; j <= n; j++) {
      if (egraph_adjacent_nodes(EG, i, j)) {
        c += egraph_get_edge_cost(EG, i, j);
      }
    }
  }
  return c;
}

void egraph_plot(egraph* EG1, egraph* EG2) {
  int i, j, v, n1, n2, eg1_has_some_edge, eg2_has_some_edge;

  n1 = n2 = 0;
  if (EG1 != NULL)
    n1 = (*EG1).n;
  if (EG2 != NULL)
    n2 = (*EG2).n;

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
  fprintf(pipe, "set size square\n");
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
    for (i = 1; i <= n1; i++) {
      fprintf(pipe, "%f %f\n", egraph_get_node_x(EG1, i), egraph_get_node_y(EG1, i));
      fprintf(pipe, "\n");
    }
    fprintf(pipe, "e\n");

    if (eg1_has_some_edge) {
      for (i = 1; i <= n1; i++) {
	for (j = i+1; j <= n1; j++) {
	  if (egraph_adjacent_nodes(EG1, i, j)) {
	    fprintf(pipe, "%f %f\n", egraph_get_node_x(EG1, i), egraph_get_node_y(EG1, i));
	    fprintf(pipe, "%f %f\n", egraph_get_node_x(EG1, j), egraph_get_node_y(EG1, j));
	    fprintf(pipe, "\n");
	  }
	}
      }
      fprintf(pipe, "e\n");
    }
  }

  if (n2 > 0) {
    for (i = 1; i <= n2; i++) {
      fprintf(pipe, "%f %f\n", egraph_get_node_x(EG2, i), egraph_get_node_y(EG2, i));
      fprintf(pipe, "\n");
    }
    fprintf(pipe, "e\n");
  
    if (eg2_has_some_edge) {
      for (i = 1; i <= n2; i++) {
	for (j = i+1; j <= n2; j++) {
	  if (egraph_adjacent_nodes(EG2, i, j)) {
	    fprintf(pipe, "%f %f\n", egraph_get_node_x(EG2, i), egraph_get_node_y(EG2, i));
	    fprintf(pipe, "%f %f\n", egraph_get_node_x(EG2, j), egraph_get_node_y(EG2, j));
	    fprintf(pipe, "\n");
	  }
	}
      }
      fprintf(pipe, "e\n");
    }
  }

  if (n1 > 0) {
    for (i = 1; i <= n1; i++) {
      fprintf(pipe, "%f %f %d", egraph_get_node_x(EG1, i), egraph_get_node_y(EG1, i), i);
      fprintf(pipe, "\n");
    }
    fprintf(pipe, "e\n");
  }
  if (n2 > 0) {
    for (i = 1; i <= n2; i++) {
      fprintf(pipe, "%f %f %d", egraph_get_node_x(EG2, i), egraph_get_node_y(EG2, i), i);
      fprintf(pipe, "\n");
    }
    fprintf(pipe, "e\n");
  }

  fflush(pipe);
}


void egraph_to_graph(egraph* EG, graph* G) {
  int i;

  int n = (*EG).n;
  graph_delete(G);
  graph_init(G, n);
  for (i = 0; i < n; i++) {
    (*G).V[i].deg = (*EG).V[i].deg;
  }
  for (i = 0; i < n * (n + 1) / 2; i++) {
    (*G).E[i].flag = (*EG).E[i].flag;
    (*G).E[i].cost = (*EG).E[i].cost;
  }
}

void graph_to_egraph(graph* G, egraph* EG) {
  int i;

  int n = (*G).n;
  for (i = 0; i < n * (n + 1) / 2; i++) {
    (*EG).E[i].flag = 0;
    (*EG).E[i].cost = 0;
  }
  for (i = 0; i < n; i++) {
    (*EG).V[i].deg = (*G).V[i].deg;
  }
  for (i = 0; i < n * (n + 1) / 2; i++) {
    (*EG).E[i].flag = (*G).E[i].flag;
    (*EG).E[i].cost = (*G).E[i].cost;
  }
}

void tree_to_egraph(tree* T, egraph* EG) {
  int i;

  int n = (*T).n;
  for (i = 0; i < n * (n + 1) / 2; i++) {
    (*EG).E[i].flag = 0;
    (*EG).E[i].cost = 0;
  }
  for (i = 0; i < n; i++) {
    (*EG).V[i].deg = (*T).V[i].deg;
    if ((*T).V[i].pred > 0)
      egraph_insert_edge(EG, (*T).V[i].pred, i+1, (*T).V[i].cost);
  }
}

void onetree_to_egraph(onetree* OT, egraph* EG) {
  int i;

  int n = (*OT).n;
  for (i = 0; i < n * (n + 1) / 2; i++) {
    (*EG).E[i].flag = 0;
    (*EG).E[i].cost = 0;
  }
  for (i = 0; i < n; i++) {
    (*EG).V[i].deg = (*OT).V[i].deg;
    if ((*OT).V[i].pred > 0)
      egraph_insert_edge(EG, (*OT).V[i].pred, i+1, (*OT).V[i].cost);
  }
}
