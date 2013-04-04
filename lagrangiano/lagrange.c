#include <stdlib.h>
#include "graph.h"
#include "lagrange.h"

// manca da inserire parametro alpha
// manca da gestire distruzione grafo originale G (va copiato d qualche altra parte o il lavoro va fatto su una copia)
// cambia nome subgradient o solve subgradient al posto di lagrange
double lagrange(graph* G, graph* H, int K, double ub, double alpha) {
  int i, j, counter, tot_cnt = 0;
  int n = (*G).n;
  graph T;
  double* mult = (double*)malloc(sizeof(double) * (n+1)); // cambiare -> mult[n]; nota: basterebbe n-1 anzichè n+1
  double* subgrad = (double*)malloc(sizeof(double) * (n+1)); // cambiare -> subgrad[n]; nota: basterebbe n-1 anzichè n+1
  double square_norm;
  double step;
  double L;
  double best_L;

  initGraph(&T, 1);
  deleteGraph(H);
  initGraph(H, n);

  for (i = 0; i < n+1; i++) { // cambiare -> memset
    mult[i] = 0;
    subgrad[i] = 0;
  }

  best_L = 0;

  while (1) {
    // calcolo 1-albero con costi attuali (costi originari più moltiplicatori attuali).
    compute_ot(G, &T);

    // calcolo il costo dell'1-albero, che è il costo della funzione L(lambda) con i moltiplicatori (mult ovvero i lambda) attuali
    L = get_graph_cost(&T);
  
    printf("L = %f\n", L);

    // aggiorno contatore iterazioni in cui L non è migliorato
    if (L > best_L) {
      best_L = L;
      counter = 1;
      copyGraph(&T, H);
    }
    else {
      counter++;
    }

	if (counter > K) {
		alpha = alpha / 2;
		counter = 1;
	}

    // condizione di STOP: se L non è migliorato nelle ultime K iterazioni
    if (tot_cnt > 6*K) {
      break;
	}

    // calcolo vettore subgradiente: s
    for (i = 2; i <= n; i++)
      subgrad[i] = 2 - get_node_deg(&T, i);

    // calcolo norma al quadrato del vettore subgradiente: ||s||^2.
    square_norm = 0;
    for (i = 2; i <= n; i++)
      square_norm += pow(subgrad[i], 2);

    // condizione di STOP: se il subgradiente è il vettore nullo, allora stop.
    // nota: il controllo si può effettuare prima, subito dopo aver calcolato il vettore subgradiente, non serve la norma
    if (square_norm == 0)
      break;

    // calcolo il passo
    step = alpha * (ub - L) / square_norm;
  
    // calcolo i nuovi moltiplicatori lagrangiani
    for (i = 2; i <= n; i++)
      mult[i] += step * (subgrad[i] / sqrt(square_norm)); // va bene così credo, i moltiplicatori non sono vincolati ad essere non negativi essendo i vincoli rilassati del tsp delle uguaglianze
    //mult[i] = ((mult[i] + step * (subgrad[i] / sqrt(square_norm))) > 0) ? (mult[i] + step * (subgrad[i] / sqrt(square_norm))) : 0; // divido anche per la norma del gradiente: in questo modo si avanza nella direzione del gradiente per una distanza esattamente pari allo step (ma non son sicuro che sia giusto, trovate versioni differenti)

    /*
    plotGraph(&T, NULL, "default", NULL);
    for (i = 2; i <= n; i++)
      printf("%.2f   ", mult[i]);
    printf("\n");
    
    for (i = 2; i <= n; i++)
      printf("%d   ", get_node_deg(&T, i));
    printf("\n");
    */

    // aggiorno i costi dei lati tenendo conto dei nuovi moltiplicatori
    for (i = 2; i <= n; i ++) {
      for (j = i+1; j <= n; j++)
	set_edge_cost(G, i, j, get_edge_cost(G, i, j) - mult[i] - mult[j]);
    }
    // aggiorno costo lati incidenti in 1 a parte
    for (j = 2; j <= n; j++)
      set_edge_cost(G, 1, j, get_edge_cost(G, 1, j) - mult[j]);


  } 

  deleteGraph(&T);
  free(mult);
  free(subgrad);

  return best_L;

}

double plot_lagrange(graph* G, graph* H, int K, double ub, double alpha) {
  int i, j, counter, tot_cnt = 0;
  int n = (*G).n;
  graph T;
  double* mult = (double*)malloc(sizeof(double) * (n+1)); // cambiare -> mult[n]; nota: basterebbe n-1 anzichè n+1
  double* subgrad = (double*)malloc(sizeof(double) * (n+1)); // cambiare -> subgrad[n]; nota: basterebbe n-1 anzichè n+1
  double square_norm;
  double step;
  double L;
  double best_L;





  FILE* pipe = popen("gnuplot -persist", "w");

  fprintf(pipe, "set multiplot\n");
  fprintf(pipe, "set size square\n");
  fprintf(pipe, "set xrange [-0.000:%.3f]\n", K+20.0);
  fprintf(pipe, "set yrange [-0.000:%.3f]\n", ub+1);
  fprintf(pipe, "set xlabel 'X'\n");
  fprintf(pipe, "set ylabel 'Y'\n");
  //fprintf(pipe, "unset xtics\n");
  //fprintf(pipe, "unset ytics\n");

  fprintf(pipe, "set style line 1 linetype 1 linecolor rgb \"red\" pointtype 9\n");
  fprintf(pipe, "set style line 2 linetype 1 linecolor rgb \"black\" linewidth 1\n");
  fprintf(pipe, "set style line 3 linetype 1 linecolor rgb \"blue\" linewidth 1\n");

  double x_axis_pos = 0;

  initGraph(&T, 1);
  deleteGraph(H);
  initGraph(H, n);

  for (i = 0; i < n+1; i++) { // cambiare -> memset
    mult[i] = 0;
    subgrad[i] = 0;
  }

  best_L = 0;

  fprintf(pipe, "plot '-' using 1:2 with points linestyle 1 notitle, '' using 1:2 with lines linestyle 2 notitle, '' using 1:2 with lines linestyle 3 notitle\n");

  while (1) {
    // calcolo 1-albero con costi attuali (costi originari più moltiplicatori attuali).
    compute_ot(G, &T);

    // calcolo il costo dell'1-albero, che è il costo della funzione L(lambda) con i moltiplicatori (mult ovvero i lambda) attuali
    L = get_graph_cost(&T);
  
    fprintf(pipe, "%f %f\n", x_axis_pos, L);
    x_axis_pos += 1;

    // aggiorno contatore iterazioni in cui L non è migliorato
    if (L > best_L) {
      best_L = L;
      counter = 1;
      copyGraph(&T, H);
    }
    else {
      counter++;
    }

	tot_cnt++;

    // condizione di STOP: se L non è migliorato nelle ultime K iterazioni
    if (counter > K) {
		alpha = alpha / 2.0;
		counter = 1;
    }

	if (tot_cnt > 6*K) {
		break;
	}

    // calcolo vettore subgradiente: s
    for (i = 2; i <= n; i++)
      subgrad[i] = 2 - get_node_deg(&T, i);

    // calcolo norma al quadrato del vettore subgradiente: ||s||^2.
    square_norm = 0;
    for (i = 2; i <= n; i++)
      square_norm += pow(subgrad[i], 2);

    // condizione di STOP: se il subgradiente è il vettore nullo, allora stop.
    // nota: il controllo si può effettuare prima, subito dopo aver calcolato il vettore subgradiente, non serve la norma
    if (square_norm == 0)
      break;

    // calcolo il passo
    step = alpha * (ub - L) / square_norm;
  
    // calcolo i nuovi moltiplicatori lagrangiani
    for (i = 2; i <= n; i++)
      mult[i] += step * (subgrad[i] / sqrt(square_norm)); // va bene così credo, i moltiplicatori non sono vincolati ad essere non negativi essendo i vincoli rilassati del tsp delle uguaglianze
    //mult[i] = ((mult[i] + step * (subgrad[i] / sqrt(square_norm))) > 0) ? (mult[i] + step * (subgrad[i] / sqrt(square_norm))) : 0; // divido anche per la norma del gradiente: in questo modo si avanza nella direzione del gradiente per una distanza esattamente pari allo step (ma non son sicuro che sia giusto, trovate versioni differenti)

    /*
    plotGraph(&T, NULL, "default", NULL);
    for (i = 2; i <= n; i++)
      printf("%.2f   ", mult[i]);
    printf("\n");
    
    for (i = 2; i <= n; i++)
      printf("%d   ", get_node_deg(&T, i));
    printf("\n");
    */

    // aggiorno i costi dei lati tenendo conto dei nuovi moltiplicatori
    for (i = 2; i <= n; i ++) {
      for (j = i+1; j <= n; j++)
	set_edge_cost(G, i, j, get_edge_cost(G, i, j) - mult[i] - mult[j]);
    }
    // aggiorno costo lati incidenti in 1 a parte
    for (j = 2; j <= n; j++)
      set_edge_cost(G, 1, j, get_edge_cost(G, 1, j) - mult[j]);


  }

  fprintf(pipe, "e\n");

  fprintf(pipe, "%f %f\n", 0.0, ub);
  fprintf(pipe, "%f %f\n", x_axis_pos, ub);
  fprintf(pipe, "e\n");

  fprintf(pipe, "%f %f\n", 0.0, best_L);
  fprintf(pipe, "%f %f\n", x_axis_pos, best_L);
  fprintf(pipe, "e\n");


  fflush(pipe);

  deleteGraph(&T);
  free(mult);
  free(subgrad);
  return best_L;

}
