#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "compute_lagrange.h"

double compute_lagrange(graph* G, double ub, int k, double alpha, int h, int max_iter) {
  graph ONE_TREE;
  graph G_TEMP;


  int i, j, counter, max_iter_counter;
  double square_norm, L, L_best, step_size;
  int n = (*G).n;

  double mult[n+1];
  double subgrad[n+1];

  memset(mult, '0', sizeof(double) * (n+1));

  initGraph(&ONE_TREE, 1);
  initGraph(&G_TEMP, 1);


  L_best = 0;
  counter = max_iter_counter = 0;
  copyGraph(G, &G_TEMP);

  while (max_iter_counter < max_iter) {

    /* calcolo 1-albero usando i costi originali modificati dai moltiplicatori lagrangiani correnti.
     */
    compute_ot(&G_TEMP, &ONE_TREE);

    /* calcolo costo 1-albero usando i costi originali;
     * - sommo 2 * (sommatoria tutti moltiplicatori) al costo dell'1-albero con i costi modificati per ottenere il costo originale dell'1-albero.
     */
    L = get_graph_cost(&ONE_TREE);
    for (i = 2; i <= n; i++) 
      L += 2 * mult[i];
    
   
    /* aggiorno contatore iterazioni in cui il valore L non è migliorato.
     */
    if (L > L_best) {
      L_best = L;
      counter = 1;
    }
    else {
      counter++;
    }

    /* STOP: interrompi ciclo se L non è migliorato nelle ultime k iterazioni.
     */
    if (counter > k)
      break;

    /* riduci (dimezza) alpha se L non è migliorato nelle ultime h iterazioni.
     */
    if (counter > h)
      alpha /= 2;

    /* calcolo le componenti del vettore subgradiente.
     */
    for (i = 2; i <= n; i++)
      subgrad[i] = 2 - get_node_deg(&ONE_TREE, i);

    /* calcolo norma al quadrato del vettore subgradiente
     */
    square_norm = 0;
    for (i = 2; i <= n; i++)
      square_norm += pow(subgrad[i], 2);

    /* STOP: interrompi ciclo se il subgradiente è il vettore nullo.
     */
    if (square_norm == 0)
      break;

    /* calcolo l'ampiezza del passo di avanzamento attuale.
     */
    step_size = (ub - L) / square_norm;
  
    /* calcolo i nuovi moltiplicatori lagrangiani.
     */
    for (i = 2; i <= n; i++) {
      mult[i] += step_size * (subgrad[i] / sqrt(square_norm));
      //mult[i] = ((mult[i] + step_size * (subgrad[i] / sqrt(square_norm))) > 0) ? (mult[i] + step_size * (subgrad[i] / sqrt(square_norm))) : 0;
    }

    /* aggiorno i costi dei lati tenendo conto dei nuovi moltiplicatori.
     */
    for (i = 2; i <= n; i ++) {
      for (j = i+1; j <= n; j++)
	set_edge_cost(&G_TEMP, i, j, get_edge_cost(G, i, j) - mult[i] - mult[j]);
    }
    for (j = 2; j <= n; j++)
      set_edge_cost(&G_TEMP, 1, j, get_edge_cost(G, 1, j) - mult[j]);

    max_iter_counter++;
  } 

  deleteGraph(&ONE_TREE);
  deleteGraph(&G_TEMP);

  return L_best;

}

double compute_and_plot_lagrange(graph* G, double ub, int k, double alpha, int h, int max_iter) {
  graph ONE_TREE;
  graph BEST_ONE_TREE;
  graph G_TEMP;


  int i, j, counter, max_iter_counter;
  double square_norm, L, L_best, step_size;
  int n = (*G).n;

  double mult[n+1];
  double subgrad[n+1];

  memset(mult, '0', sizeof(double) * (n+1));


  initGraph(&ONE_TREE, 1);
  initGraph(&BEST_ONE_TREE, 1);
  initGraph(&G_TEMP, n);


  double width = max_iter; // larghezza finestra grafico
  double height = ub+1; // altezza finestra grafico
  FILE* pipe = popen("gnuplot -persist", "w");

  fprintf(pipe, "set multiplot\n");
  fprintf(pipe, "set size square\n");
  fprintf(pipe, "set xrange [-0.000:%.3f]\n", width);
  fprintf(pipe, "set yrange [-0.000:%.3f]\n", height);
  fprintf(pipe, "set xlabel 'X'\n");
  fprintf(pipe, "set ylabel 'Y'\n");

  fprintf(pipe, "set style line 1 linetype 1 linecolor rgb \"red\" pointtype 9\n");
  fprintf(pipe, "set style line 2 linetype 1 linecolor rgb \"black\" linewidth 1\n");
  fprintf(pipe, "set style line 3 linetype 1 linecolor rgb \"blue\" linewidth 1\n");

  fprintf(pipe, "plot '-' using 1:2 with points linestyle 1 notitle, '' using 1:2 with lines linestyle 2 notitle, '' using 1:2 with lines linestyle 3 notitle\n");

  L_best = 0;
  counter = max_iter_counter = 0;
  copyGraph(G, &G_TEMP);

  while (max_iter_counter < max_iter) {

    // costruisco 1-albero e calcolo costo
    compute_ot(&G_TEMP, &ONE_TREE);
    L = get_graph_cost(&ONE_TREE);
    for (i = 2; i <= n; i++) 
      L += 2 * mult[i];

    // aggiorno contatori
    if (L > L_best) {
      L_best = L;
      counter = 1;
      copyGraph(&ONE_TREE, &BEST_ONE_TREE);
      /* stampo nuovo valore L_best trovato
       */
      printf("best lower bound up to now: %f\n", L_best); 
    }
    else {
      counter++;
    }

    /* stampo coordinate L.
     */
    fprintf(pipe, "%f %f\n", max_iter_counter + 0.0, L);


    // STOP
    if (counter > k)
      break;

    // aggiorno alpha
    if (counter > h)
      alpha /= 2;

    for (i = 2; i <= n; i++)
      subgrad[i] = 2 - get_node_deg(&ONE_TREE, i);

    square_norm = 0;

    for (i = 2; i <= n; i++)
      square_norm += pow(subgrad[i], 2);    

    // STOP
    if (square_norm == 0)
      break;

    // aggiorno moltiplicatori
    step_size = (ub - L) / square_norm;  
    for (i = 2; i <= n; i++) {
      mult[i] += step_size * (subgrad[i] / sqrt(square_norm));
      //mult[i] = ((mult[i] + step_size * (subgrad[i] / sqrt(square_norm))) > 0) ? (mult[i] + step_size * (subgrad[i] / sqrt(square_norm))) : 0;
    }

    // aggiorno costi
    for (i = 2; i <= n; i ++) {
      for (j = i+1; j <= n; j++)
	set_edge_cost(&G_TEMP, i, j, get_edge_cost(G, i, j) - mult[i] - mult[j]);
    }
    for (j = 2; j <= n; j++)
      set_edge_cost(&G_TEMP, 1, j, get_edge_cost(G, 1, j) - mult[j]);

    max_iter_counter++;
  } 

  /* stampo miglior lower bound e numero di iterazioni effettuate
   */
  printf("number of iterations: %d\n", max_iter_counter);
  printf("best lower bound computed: %f\n", L_best);

  fprintf(pipe, "e\n");

  /* stampo linea upper bound 'ub'.
   */
  fprintf(pipe, "%f %f\n", 0.0, ub);
  fprintf(pipe, "%f %f\n", max_iter_counter + 0.0, ub);
  fprintf(pipe, "e\n");

  /* stampo linea miglior (maggiore) valore della funzione lagrangiana trovato.
   */
  fprintf(pipe, "%f %f\n", 0.0, L_best);
  fprintf(pipe, "%f %f\n", max_iter_counter + 0.0, L_best);
  fprintf(pipe, "e\n");

  /* disegno grafico del miglior 1-albero trovato, ovvero l'1-albero su G con costo maggiore.
   */
  plotGraph(G, &BEST_ONE_TREE, "onetree", NULL);

  fflush(pipe);

  deleteGraph(&ONE_TREE);
  deleteGraph(&BEST_ONE_TREE);
  deleteGraph(&G_TEMP);

  return L_best;

}
