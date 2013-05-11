#include "compute_lagrange.h"

double compute_lagrange(graph* G, onetree* OT, double ub) {
  int i, j, v, num_of_iterations, time_since_improvement;
  double alpha, square_norm, step_size, z, z_best;

  graph G_tmp;
  onetree OT_tmp;

  int n = (*G).n;

  // spreco una cella per evitare di decrementare gli indici dei nodi
  // prima di accedere ad una determinata cella
  // (i nodi sono indicizzati da 1 a n, le celle da 0 a n)
  double* multipliers = (double*)calloc(n+1, sizeof(double));
  double* subgradient = (double*)calloc(n+1, sizeof(double));

  onetree_delete(OT);
  // taglia 3, visto che più avanti verrà sovrascritto con un copy
  onetree_init(OT, 3);

  // taglia 3, visto che più avanti verrà sovrascritto con un copy
  onetree_init(&OT_tmp, 3);

  graph_init(&G_tmp, 1);
  graph_copy(G, &G_tmp);

  int flag = 0;

  alpha = ALPHA;
  num_of_iterations = 0;
  time_since_improvement = 0;

  while (num_of_iterations < MAX_NUM_ITERATIONS) {
    num_of_iterations++;
    //printf("ciao %d\n", num_of_iterations);

    // calcola 1-albero utilizzando come costi dei lati i costi originari di G
    // modificati dai moltiplicatori lagrangiani
    compute_ot(&G_tmp, &OT_tmp);

    // calcola costo 1-albero usando come costi dei lati i costi originari di G
    z = onetree_get_cost(&OT_tmp);
    for (i = 2; i <= n; i++) 
      z += 2 * multipliers[i];
    
    // se necessario aggiorna z_best e memorizza il miglior 1-albero
    // fin'ora calcolato
    if (z > z_best || flag == 0) {
      flag = 1;
      z_best = z;
      onetree_copy(&OT_tmp, OT);
      // azzera il contatore delle iterazioni senza miglioramento
      // del valore z della funzione lagrangiana
      time_since_improvement = 0;
    }
    else {
      // incrementa il contatore delle iterazioni senza miglioramento
      // del valore z della funzione lagrangiana
      time_since_improvement++;
    }

    // interrompe il ciclo se z non è migliorato nelle ultime SLACK_TIME iterazioni
    if (time_since_improvement >= MAX_NUM_ITERATIONS_NO_IMPROV)
      break;

    // dimezza alpha se z non è migliorato nelle ultime
    // ALPHA_HALVING_TIME iterazioni
    if (time_since_improvement >= ALPHA_HALVING_TIME)
      alpha /= 2.0;

    square_norm = 0.0;
    for (i = 2; i <= n; i++) {
      // calcola le componenti del vettore subgradiente
      subgradient[i] = 2 - onetree_get_node_deg(&OT_tmp, i);
      // calcola la norma al quadrato del vettore subgradiente
      square_norm += subgradient[i] * subgradient[i];
    }

    // interrompe il ciclo se il vettore subgradiente è il vettore nullo
    if (square_norm == 0.0)
      break;

    // calcola l'ampiezza del passo di avanzamento
    step_size = alpha * (ub - z) / square_norm;
  
    // calcola i nuovi moltiplicatori lagrangiani
    for (i = 2; i <= n; i++) {
      multipliers[i] += step_size * (subgradient[i] / sqrt(square_norm));
    }

    // aggiorna i costi dei lati tenendo conto dei nuovi moltiplicatori
    for (i = 2; i <= n; i ++) {
      for (j = i+1; j <= n; j++) {
        // aggiorno il costi dei lati non incidenti in 1
        graph_set_edge_cost(&G_tmp, i, j,
              graph_get_edge_cost(G, i, j) - multipliers[i] - multipliers[j]);
      }
    }
    for (j = 2; j <= n; j++) {
      // aggiorno il costo dei lati incidenti in 1
      graph_set_edge_cost(&G_tmp, 1, j,
              graph_get_edge_cost(G, 1, j) - multipliers[j]);
    }
  } 


  onetree_delete(&OT_tmp);
  graph_delete(&G_tmp);
  free(subgradient);
  free(multipliers);

  // ripristina i costi originari dei lati sul miglior 1-albero calcolato
  // e memorizzato in OT
  for (i = 1; i <= n; i++) {
    onetree_set_edge_cost(OT, onetree_get_pred(OT, i), i,
              graph_get_edge_cost(G, onetree_get_pred(OT, i), i));
  }

  return z_best;
}
