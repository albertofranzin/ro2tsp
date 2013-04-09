#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "graph.h"
#include "onetree.h"
#include "compute_lagrange.h"

#define VERY_SMALL -1000000000

double compute_lagrange(graph* G, onetree* OT, double ub, int k, double alpha, int h, int max_iter) {
  onetree ONE_TREE;
  graph G_TEMP;


  int i, j, counter, max_iter_counter;
  double square_norm, L, L_best, step_size;
  int n = (*G).n;

  double mult[n+1];
  double subgrad[n+1];

  memset(mult, '0', sizeof(double) * (n+1));

  onetree_init(&ONE_TREE, 1);
  graph_init(&G_TEMP, 1);
  onetree_delete(OT);


  L_best = VERY_SMALL;
  counter = max_iter_counter = 0;
  graph_copy(G, &G_TEMP);

  while (max_iter_counter < max_iter) {

    /* calcolo 1-albero usando i costi originali modificati dai moltiplicatori lagrangiani correnti.
     */
    compute_ot(&G_TEMP, &ONE_TREE);

    /* calcolo costo 1-albero usando i costi originali;
     * - sommo 2 * (sommatoria tutti moltiplicatori) al costo dell'1-albero con i costi modificati per ottenere il costo originale dell'1-albero.
     */
    L = onetree_get_cost(&ONE_TREE);
    for (i = 2; i <= n; i++) 
      L += 2 * mult[i];
    
   
    /* aggiorno contatore iterazioni in cui il valore L non è migliorato.
     */
    if (L > L_best) {
      L_best = L;
      counter = 1;
      onetree_copy(&ONE_TREE, OT);
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

    /* calcolo le componenti del vettore subgradiente,
     * calcolo norma al quadrato del vettore subgradiente
     */
    square_norm = 0;
    for (i = 2; i <= n; i++) {
      subgrad[i] = 2 - onetree_get_node_deg(&ONE_TREE, i);
      square_norm += subgrad[i] * subgrad[i];
    }

    /* STOP: interrompi ciclo se il subgradiente è il vettore nullo.
     */
    if (square_norm == 0)
      break;

    /* calcolo l'ampiezza del passo di avanzamento attuale.
     */
    step_size = alpha * (ub - L) / square_norm;
  
    /* calcolo i nuovi moltiplicatori lagrangiani.
     */
    for (i = 2; i <= n; i++) {
      mult[i] += step_size * (subgrad[i] / sqrt(square_norm));
      //mult[i] = ((mult[i] + step_size * (subgrad[i] / sqrt(square_norm))) > 0) ? (mult[i] + step_size * (subgrad[i] / sqrt(square_norm))) : 0;
    }

    /* aggiorno i costi dei lati tenendo conto dei nuovi moltiplicatori.
     */
    for (i = 2; i <= n; i ++) {
      for (j = i+1; j <= n; j++) {
        graph_set_edge_cost(&G_TEMP, i, j, graph_get_edge_cost(G, i, j) - mult[i] - mult[j]);
      }
    }

    for (j = 2; j <= n; j++) {
      graph_set_edge_cost(&G_TEMP, 1, j, graph_get_edge_cost(G, 1, j) - mult[j]);
    }

    max_iter_counter++;
  } 

  onetree_delete(&ONE_TREE);
  graph_delete(&G_TEMP);
  

  for (i = 1; i <= n; i++) {
    for (j = i+1; j <= n; j++)
      onetree_set_edge_cost(OT, i, j, graph_get_edge_cost(G, i, j));
  }
  return L_best;

}