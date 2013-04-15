#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "graph.h"
#include "onetree.h"
#include "solve_tsp.h"

#include "edge.h"
#include "edgelist.h"
#include <string.h>

graph WORK_GRAPH;

int tour_found_flag;
int current_level;
int number_of_levels; 
int number_of_calls; 

int* number_forced_edges_per_node; // numero di lati forzati per nodo: ad esempio number_forced_edges_per_node[4-1] = numero di lati con costo SMALL tra tutti i lati del tipo {4, v} dove v = 1, ..., n e v != 4. 
int* number_removed_edges_per_node; // numero di lati vietati per node: ad esempio number_removed_edges_per_node[7-1] = numero di lati con costo BIG tra tutti i lati del tipo {7, v} dove v = 1, ..., n e v != 7. 


void solve_tsp(graph* G, onetree* H, double* incumbent, onetree* ONE_TREE, double z, int call_flag) {
  int i, j, u, v, w, current_call;
  double cost_wv, cost_wu;
  //onetree ONE_TREE;
  int n = (*G).n;

  /* ========================== */
  /* Inizializzazione strutture */
  /* ========================== */
  //onetree_init(&ONE_TREE, 1);
  if (call_flag == 0) {
    graph_init(&WORK_GRAPH, 1);
    graph_copy(G, &WORK_GRAPH);
    //onetree_delete(H);
    onetree_copy(ONE_TREE, H); // copio in H l'1-albero passato in input alla prima chiamata, in casi fortunati potrebbe essere un ciclo ottimo!

    tour_found_flag = 0;
    current_level = 1;
    number_of_levels = 1;
    number_of_calls = 1;

    number_forced_edges_per_node = (int*)calloc(n, sizeof(int));
    number_removed_edges_per_node = (int*)calloc(n, sizeof(int));

    call_flag = 1;
  }
  else {
    // riciclo 1-albero passato in input (da euristico o altro: nota: dev'essere un 1-albero non un grafo qualsiasi!
    //onetree_copy(H, &ONE_TREE); // non lo voglio riciclare solamente alla prima chiamata, ma ad ogni chiamata ricorsiva (soprattutto gli 1-alberi prodotti da strong-branching!
    call_flag = 2;
  }

  // aggiornamento di alcune variabili utili per analizzare l'esecuzione dell'algoritmo
  number_of_levels = (current_level > number_of_levels) ? current_level : number_of_levels;
  current_call = number_of_calls;



  /* ================ */
  /* Calcolo 1-albero */
  /* ================ */
  // se si usano gli alberi calcolati per strong-branching, non serve calcolare l'1-albero, viene passato come parametro di input 
  /*
  if (call_flag == 1) {
    //compute_ot(&WORK_GRAPH, &ONE_TREE);
    compute_lagrange(&WORK_GRAPH, &ONE_TREE, *incumbent);
  }
  else if (call_flag == 2) {
    //compute_ot(&WORK_GRAPH, &ONE_TREE);
    compute_lagrange(&WORK_GRAPH, &ONE_TREE, compute_upper_bound(&WORK_GRAPH));
  }
  */

  /* =========================== */
  /* Calcolo costo dell'1-albero */
  /* =========================== */
  // se si usa strong branching si può passare direttamente anche il costo dell'1-albero 
  /*
  onetree_set_edge_cost(ONE_TREE, 1, onetree_get_first_node(ONE_TREE), graph_get_edge_cost(G, 1, onetree_get_first_node(ONE_TREE))); 
  onetree_set_edge_cost(ONE_TREE, 1, onetree_get_second_node(ONE_TREE), graph_get_edge_cost(G, 1, onetree_get_second_node(ONE_TREE)));
  for (i = 2; i <= n; i++) {
    onetree_set_edge_cost(ONE_TREE, onetree_get_pred(ONE_TREE, i), i, graph_get_edge_cost(G, onetree_get_pred(ONE_TREE, i), i));
  }
  z = onetree_get_cost(&ONE_TREE);
  */


  /* ================================================ */
  /* Verifico se è possibile potare il branch corrent */
  /* ================================================ */
  // se si usa strong branching, questa verifica viene fatta prima di selezionare il branch
  /*
  if (z > *incumbent) {
    onetree_delete(&ONE_TREE);
    return;
  }
  */



  /* ====================================== */
  /* Eventuale aggiornamento dell'incumbent */
  /* ====================================== */
  if (is_cycle(ONE_TREE)) {
    printf("# updated incumbent = %f : current_call = %d : current level = %d\n", z, current_call, current_level);
    *incumbent = z;
    onetree_copy(ONE_TREE, H);
    tour_found_flag = 1;
    /*
    if (z < *incumbent || tour_found_flag == 0) {
      printf("# updated incumbent = %f : current_call = %d : current level = %d\n", z, current_call, current_level);
      *incumbent = z;
      onetree_copy(&ONE_TREE, H);
      tour_found_flag = 1;
    }
    else { // z = *incubent AND è già stato trovato (e memorizzato in H) un candidato per il ciclo hamiltoniano ottimo 
      onetree_delete(&ONE_TREE);
      return;
    }
    */
  }



  /* ================================== */
  /* Seleziono un nodo per il branching */
  /* ================================== */
  for (w = 1; w <= n; w++) {
    if (onetree_get_node_deg(ONE_TREE, w) >= 3)
      break;
  }



  /* =================================== */
  /* Seleziono dei lati per il branching */
  /* =================================== */
  for (v = 1; v <= n; v++) {
   if (v != w && onetree_adjacent_nodes(ONE_TREE, w, v) && graph_get_edge_cost(&WORK_GRAPH, w, v) > SMALL && graph_get_edge_cost(&WORK_GRAPH, w, v) < BIG)
      break;
  }
  for (u = 1; u <= n; u++) {
    if (u != w && u != v && onetree_adjacent_nodes(ONE_TREE, w, u) && graph_get_edge_cost(&WORK_GRAPH, w, u) > SMALL && graph_get_edge_cost(&WORK_GRAPH, w, u) < BIG)
      break;
  }

  //onetree_delete(&ONE_TREE);


  // operazioni preliminari prima di avviare il branching: inizializzazione di alcune strutture

  int temp_number_forced_edges_per_node[n];
  int temp_number_removed_edges_per_node[n];

  int state;
  edge old_edge;
  edgelist old_edges;
  edgelist edges_to_be_modified;

  edgelist_init(&old_edges);
  edgelist_init(&edges_to_be_modified);
  for (i = 0; i < n; i++) { // salvo valori correnti relativi al numero di lati forzati e vietati
    temp_number_forced_edges_per_node[i] = number_forced_edges_per_node[i];
    temp_number_removed_edges_per_node[i] = number_removed_edges_per_node[i];
  }

  /* ========= */
  /* Branching */
  /* ========= */
  
  // BRANCHING A 3 VIE
  if ((v >= 1 && v <= n) && (u >= 1 && u <= n)) {


    // FORWARD CHECKING - STRONG BRANCHING
    int k;
    int branch_selection[3];
    double z1, z2, z3;
    onetree FC_ONE_TREE1;
    onetree FC_ONE_TREE2;
    onetree FC_ONE_TREE3;
    onetree_init(&FC_ONE_TREE1, 1);
    onetree_init(&FC_ONE_TREE2, 1);
    onetree_init(&FC_ONE_TREE3, 1);



    // #1: vieto il lato {w, v};
    edgelist_empty(&edges_to_be_modified);
    edgelist_push_last(&edges_to_be_modified, w, v, BIG);
    state = propagate_constraints(&WORK_GRAPH, &edges_to_be_modified, &old_edges);

    if (state == SUCCESS) {
 
      //compute_ot(&WORK_GRAPH, &FC_ONE_TREE1);
      z1 = compute_lagrange(&WORK_GRAPH, &FC_ONE_TREE1, compute_upper_bound(&WORK_GRAPH));
      onetree_set_edge_cost(&FC_ONE_TREE1, 1, onetree_get_first_node(&FC_ONE_TREE1), graph_get_edge_cost(G, 1, onetree_get_first_node(&FC_ONE_TREE1))); 
      onetree_set_edge_cost(&FC_ONE_TREE1, 1, onetree_get_second_node(&FC_ONE_TREE1), graph_get_edge_cost(G, 1, onetree_get_second_node(&FC_ONE_TREE1)));
      for (i = 2; i <= n; i++) {
	onetree_set_edge_cost(&FC_ONE_TREE1, onetree_get_pred(&FC_ONE_TREE1, i), i, graph_get_edge_cost(G, onetree_get_pred(&FC_ONE_TREE1, i), i));
      }
      z1 = onetree_get_cost(&FC_ONE_TREE1);

    }
    else {
      z1 = *incumbent; // in questo modo il branch non verrà selezionato
    }

    while (!edgelist_is_empty(&old_edges)) { // roll back costo lati
      old_edge = edgelist_pop_first(&old_edges);
      graph_set_edge_cost(&WORK_GRAPH, old_edge.x, old_edge.y, old_edge.cost);
    }
    for (i = 0; i < n; i++) { // roll back contatori lati forzati - vietati
      number_forced_edges_per_node[i] = temp_number_forced_edges_per_node[i];
      number_removed_edges_per_node[i] = temp_number_removed_edges_per_node[i];
    }







    // #2: forzo il lato {w, v} e vieto il lato {w, u};
    edgelist_empty(&edges_to_be_modified);
    edgelist_push_last(&edges_to_be_modified, w, v, SMALL);
    edgelist_push_last(&edges_to_be_modified, w, u, BIG);
    state = propagate_constraints(&WORK_GRAPH, &edges_to_be_modified, &old_edges);

    if (state == SUCCESS) {
 
      //compute_ot(&WORK_GRAPH, &FC_ONE_TREE2);
      compute_lagrange(&WORK_GRAPH, &FC_ONE_TREE2, compute_upper_bound(&WORK_GRAPH));
      onetree_set_edge_cost(&FC_ONE_TREE2, 1, onetree_get_first_node(&FC_ONE_TREE2), graph_get_edge_cost(G, 1, onetree_get_first_node(&FC_ONE_TREE2))); 
      onetree_set_edge_cost(&FC_ONE_TREE2, 1, onetree_get_second_node(&FC_ONE_TREE2), graph_get_edge_cost(G, 1, onetree_get_second_node(&FC_ONE_TREE2)));
      for (i = 2; i <= n; i++) {
	onetree_set_edge_cost(&FC_ONE_TREE2, onetree_get_pred(&FC_ONE_TREE2, i), i, graph_get_edge_cost(G, onetree_get_pred(&FC_ONE_TREE2, i), i));
      }
      z2 = onetree_get_cost(&FC_ONE_TREE2);

    }
    else {
      z2 = *incumbent;
    }

    while (!edgelist_is_empty(&old_edges)) {// roll back costo lati
      old_edge = edgelist_pop_first(&old_edges);
      graph_set_edge_cost(&WORK_GRAPH, old_edge.x, old_edge.y, old_edge.cost);
    }
    for (i = 0; i < n; i++) { // roll back contatori lati forzati - vietati
      number_forced_edges_per_node[i] = temp_number_forced_edges_per_node[i];
      number_removed_edges_per_node[i] = temp_number_removed_edges_per_node[i];
    }







    // #3: forzo i lati {w, v} e {w, u}
    edgelist_empty(&edges_to_be_modified);
    edgelist_push_last(&edges_to_be_modified, w, v, SMALL);
    edgelist_push_last(&edges_to_be_modified, w, u, SMALL);
    state = propagate_constraints(&WORK_GRAPH, &edges_to_be_modified, &old_edges);

    if (state == SUCCESS) {
 
      //compute_ot(&WORK_GRAPH, &FC_ONE_TREE3);
      compute_lagrange(&WORK_GRAPH, &FC_ONE_TREE3, compute_upper_bound(&WORK_GRAPH));
      onetree_set_edge_cost(&FC_ONE_TREE3, 1, onetree_get_first_node(&FC_ONE_TREE3), graph_get_edge_cost(G, 1, onetree_get_first_node(&FC_ONE_TREE3))); 
      onetree_set_edge_cost(&FC_ONE_TREE3, 1, onetree_get_second_node(&FC_ONE_TREE3), graph_get_edge_cost(G, 1, onetree_get_second_node(&FC_ONE_TREE3)));
      for (i = 2; i <= n; i++) {
	onetree_set_edge_cost(&FC_ONE_TREE3, onetree_get_pred(&FC_ONE_TREE3, i), i, graph_get_edge_cost(G, onetree_get_pred(&FC_ONE_TREE3, i), i));
      }
      z3 = onetree_get_cost(&FC_ONE_TREE3);

    }
    else {
      z3 = *incumbent;
    }

    while (!edgelist_is_empty(&old_edges)) {  // roll back costo lati
      old_edge = edgelist_pop_first(&old_edges);
      graph_set_edge_cost(&WORK_GRAPH, old_edge.x, old_edge.y, old_edge.cost);
    }
    for (i = 0; i < n; i++) { // roll back contatori lati forzati - vietati
      number_forced_edges_per_node[i] = temp_number_forced_edges_per_node[i];
      number_removed_edges_per_node[i] = temp_number_removed_edges_per_node[i];
    }



    // imposto ordine con cui verranno selezionati i branch
    if (z1 >= z2 >= z3)
      { branch_selection[0] = 1; branch_selection[1] = 2; branch_selection[2] = 3; }
    else if (z1 >= z3 >= z2)
      { branch_selection[0] = 1; branch_selection[1] = 3; branch_selection[2] = 2; }
    else if (z2 >= z1 >= z3)
      { branch_selection[0] = 2; branch_selection[1] = 1; branch_selection[2] = 3; }
    else if (z2 >= z3 >= z1)
      { branch_selection[0] = 2; branch_selection[1] = 3; branch_selection[2] = 1; }
    else if (z3 >= z1 >= z2)
      { branch_selection[0] = 3; branch_selection[1] = 1; branch_selection[2] = 2; }
    else
      { branch_selection[0] = 3; branch_selection[1] = 2; branch_selection[2] = 1; }



    for (k = 0; k < 3; k++) {

      switch (branch_selection[k]) {

      case 1:
	if (z1 < *incumbent) {

	  // branch #1 -> vieta il lato {w, v};
	  edgelist_empty(&edges_to_be_modified);
	  edgelist_push_last(&edges_to_be_modified, w, v, BIG);
	  state = propagate_constraints(&WORK_GRAPH, &edges_to_be_modified, &old_edges); // cerco di propagare i vincoli che voglio imporre sui lati (in questo caso BIG su {w, v}
	  if (state == SUCCESS) {
	    onetree_copy(&FC_ONE_TREE1, ONE_TREE);
	    onetree_delete(&FC_ONE_TREE1);
	    number_of_calls++;
	    current_level++;
	    solve_tsp(G, H, incumbent, ONE_TREE, z1, call_flag);
	    current_level--;
	  }
	  while (!edgelist_is_empty(&old_edges)) { // roll back costo lati
	    old_edge = edgelist_pop_first(&old_edges);
	    graph_set_edge_cost(&WORK_GRAPH, old_edge.x, old_edge.y, old_edge.cost);
	  }
	  for (i = 0; i < n; i++) { // roll back contatori lati forzati - vietati
	    number_forced_edges_per_node[i] = temp_number_forced_edges_per_node[i];
	    number_removed_edges_per_node[i] = temp_number_removed_edges_per_node[i];
	  }

	}
	break;


      case 2:
	if (z2 < *incumbent) {

	  // branch #2 -> forza il lato {w, v} e vieta il lato {w, u};
	  edgelist_empty(&edges_to_be_modified);
	  edgelist_push_last(&edges_to_be_modified, w, v, SMALL);
	  edgelist_push_last(&edges_to_be_modified, w, u, BIG);
	  state = propagate_constraints(&WORK_GRAPH, &edges_to_be_modified, &old_edges);


	  if (state == SUCCESS) {
	    onetree_copy(&FC_ONE_TREE2, ONE_TREE);
	    onetree_delete(&FC_ONE_TREE2);
	    number_of_calls++;
	    current_level++;
	    solve_tsp(G, H, incumbent, ONE_TREE, z2, call_flag);
	    current_level--;
	  }
	  while (!edgelist_is_empty(&old_edges)) { // roll back costo lati
	    old_edge = edgelist_pop_first(&old_edges);
	    graph_set_edge_cost(&WORK_GRAPH, old_edge.x, old_edge.y, old_edge.cost);
	  }
	  for (i = 0; i < n; i++) { // roll back contatori lati forzati - vietati
	    number_forced_edges_per_node[i] = temp_number_forced_edges_per_node[i];
	    number_removed_edges_per_node[i] = temp_number_removed_edges_per_node[i];
	  }

	}
	break;


      case 3:
	if (z3 < *incumbent) {

	  // branch #3 -> forza i lati {w, v} e {w, u}, vieta tutti gli altri lati;
	  edgelist_empty(&edges_to_be_modified);
	  edgelist_push_last(&edges_to_be_modified, w, v, SMALL);
	  edgelist_push_last(&edges_to_be_modified, w, u, SMALL);
	  state = propagate_constraints(&WORK_GRAPH, &edges_to_be_modified, &old_edges);
	  if (state == SUCCESS) {
	    onetree_copy(&FC_ONE_TREE3, ONE_TREE);
	    onetree_delete(&FC_ONE_TREE3);
	    number_of_calls++;
	    current_level++;
	    solve_tsp(G, H, incumbent, ONE_TREE, z3, call_flag);
	    current_level--;
	  }
	  while (!edgelist_is_empty(&old_edges)) {  // roll back costo lati
	    old_edge = edgelist_pop_first(&old_edges);
	    graph_set_edge_cost(&WORK_GRAPH, old_edge.x, old_edge.y, old_edge.cost);
	  }
	  for (i = 0; i < n; i++) { // roll back contatori lati forzati - vietati
	    number_forced_edges_per_node[i] = temp_number_forced_edges_per_node[i];
	    number_removed_edges_per_node[i] = temp_number_removed_edges_per_node[i];
	  }

	}
	break;

      default:
	break;

      } // fine switch

    }

  } // fine caso branching 3 vie

  
  // BRANCHING A 2 VIE
  else if (((v >= 1 && v <= n) && (u < 1 || u > n)) || ((v < 1 || v > n) && (u >= 1 && u <= n))) {


    // FORWARD CHECKING - STRONG BRANCHING
    int k;
    int branch_selection[2];
    double z1, z2;
    onetree FC_ONE_TREE1;
    onetree FC_ONE_TREE2;
    onetree_init(&FC_ONE_TREE1, 1);
    onetree_init(&FC_ONE_TREE2, 1);


    // #1: vieto il lato {w, v};
    edgelist_empty(&edges_to_be_modified);
    edgelist_push_last(&edges_to_be_modified, w, v, BIG);
    state = propagate_constraints(&WORK_GRAPH, &edges_to_be_modified, &old_edges);
    if (state == SUCCESS) {
 
      //compute_ot(&WORK_GRAPH, &FC_ONE_TREE1);
      compute_lagrange(&WORK_GRAPH, &FC_ONE_TREE1, compute_upper_bound(&WORK_GRAPH));
      onetree_set_edge_cost(&FC_ONE_TREE1, 1, onetree_get_first_node(&FC_ONE_TREE1), graph_get_edge_cost(G, 1, onetree_get_first_node(&FC_ONE_TREE1))); 
      onetree_set_edge_cost(&FC_ONE_TREE1, 1, onetree_get_second_node(&FC_ONE_TREE1), graph_get_edge_cost(G, 1, onetree_get_second_node(&FC_ONE_TREE1)));
      for (i = 2; i <= n; i++) {
	onetree_set_edge_cost(&FC_ONE_TREE1, onetree_get_pred(&FC_ONE_TREE1, i), i, graph_get_edge_cost(G, onetree_get_pred(&FC_ONE_TREE1, i), i));
      }
      z1 = onetree_get_cost(&FC_ONE_TREE1);

    }
    else {
      z1 = *incumbent; // in questo modo il branch non verrà selezionato
    }

    while (!edgelist_is_empty(&old_edges)) {  // roll back costo lati
      old_edge = edgelist_pop_first(&old_edges);
      graph_set_edge_cost(&WORK_GRAPH, old_edge.x, old_edge.y, old_edge.cost);
    }
    for (i = 0; i < n; i++) { // roll back contatori lati forzati - vietati
      number_forced_edges_per_node[i] = temp_number_forced_edges_per_node[i];
      number_removed_edges_per_node[i] = temp_number_removed_edges_per_node[i];
    }





    // #2: forzo il lato {w, u};
    edgelist_empty(&edges_to_be_modified);
    edgelist_push_last(&edges_to_be_modified, w, v, SMALL);
    state = propagate_constraints(&WORK_GRAPH, &edges_to_be_modified, &old_edges);

    if (state == SUCCESS) {
 
      //compute_ot(&WORK_GRAPH, &FC_ONE_TREE2);
      compute_lagrange(&WORK_GRAPH, &FC_ONE_TREE2, compute_upper_bound(&WORK_GRAPH));
      onetree_set_edge_cost(&FC_ONE_TREE2, 1, onetree_get_first_node(&FC_ONE_TREE2), graph_get_edge_cost(G, 1, onetree_get_first_node(&FC_ONE_TREE2))); 
      onetree_set_edge_cost(&FC_ONE_TREE2, 1, onetree_get_second_node(&FC_ONE_TREE2), graph_get_edge_cost(G, 1, onetree_get_second_node(&FC_ONE_TREE2)));
      for (i = 2; i <= n; i++) {
	onetree_set_edge_cost(&FC_ONE_TREE2, onetree_get_pred(&FC_ONE_TREE2, i), i, graph_get_edge_cost(G, onetree_get_pred(&FC_ONE_TREE2, i), i));
      }
      z2 = onetree_get_cost(&FC_ONE_TREE2);

    }
    else {
      z2 = *incumbent;
    }

    while (!edgelist_is_empty(&old_edges)) {  // roll back costo lati
      old_edge = edgelist_pop_first(&old_edges);
      graph_set_edge_cost(&WORK_GRAPH, old_edge.x, old_edge.y, old_edge.cost);
    }
    for (i = 0; i < n; i++) { // roll back contatori lati forzati - vietati
      number_forced_edges_per_node[i] = temp_number_forced_edges_per_node[i];
      number_removed_edges_per_node[i] = temp_number_removed_edges_per_node[i];
    }




    // imposto ordine con cui verranno selezionati i branch
    if (z1 >= z2)
      { branch_selection[0] = 1; branch_selection[1] = 2; }
    else
      { branch_selection[0] = 2; branch_selection[1] = 1; }



    for (k = 0; k < 3; k++) {

      switch (branch_selection[k]) {

      case 1: 
	if (z1 < *incumbent) {

	  // branch #1 -> vieta il lato {w, v};
	  edgelist_empty(&edges_to_be_modified);
	  edgelist_push_last(&edges_to_be_modified, w, v, BIG);
	  state = propagate_constraints(&WORK_GRAPH, &edges_to_be_modified, &old_edges);
	  if (state == SUCCESS) {
	    onetree_copy(&FC_ONE_TREE1, ONE_TREE);
	    onetree_delete(&FC_ONE_TREE1);
	    number_of_calls++;
	    current_level++;
	    solve_tsp(G, H, incumbent, ONE_TREE, z1, call_flag);
	    current_level--;
	  }
	  while (!edgelist_is_empty(&old_edges)) {  // roll back costo lati
	    old_edge = edgelist_pop_first(&old_edges);
	    graph_set_edge_cost(&WORK_GRAPH, old_edge.x, old_edge.y, old_edge.cost);
	  }
	  for (i = 0; i < n; i++) { // roll back contatori lati forzati - vietati
	    number_forced_edges_per_node[i] = temp_number_forced_edges_per_node[i];
	    number_removed_edges_per_node[i] = temp_number_removed_edges_per_node[i];
	  }

	}
	break;

      case 2:
	if (z2 < *incumbent) {

	  // branch #2 -> forza il lato {w, u};
	  edgelist_empty(&edges_to_be_modified);
	  edgelist_push_last(&edges_to_be_modified, w, v, SMALL);
	  state = propagate_constraints(&WORK_GRAPH, &edges_to_be_modified, &old_edges);
	  if (state == SUCCESS) {
	    onetree_copy(&FC_ONE_TREE2, ONE_TREE);
	    onetree_delete(&FC_ONE_TREE2);
	    number_of_calls++;
	    current_level++;
	    solve_tsp(G, H, incumbent, ONE_TREE, z2, call_flag);
	    current_level--;
	  }
	  while (!edgelist_is_empty(&old_edges)) {  // roll back costo lati
	    old_edge = edgelist_pop_first(&old_edges);
	    graph_set_edge_cost(&WORK_GRAPH, old_edge.x, old_edge.y, old_edge.cost);
	  }
	  for (i = 0; i < n; i++) { // roll back contatori lati forzati - vietati
	    number_forced_edges_per_node[i] = temp_number_forced_edges_per_node[i];
	    number_removed_edges_per_node[i] = temp_number_removed_edges_per_node[i];
	  }

	}
	break;

      default:
	break;


      } // fine switch
    }


  } // fine caso branching 2 vie

  edgelist_delete(&old_edges);
  edgelist_delete(&edges_to_be_modified);

  if (call_flag == 1) {
    graph_delete(&WORK_GRAPH);
    free(number_forced_edges_per_node);
    free(number_removed_edges_per_node);
    printf("# number of levels = %d\n# number of calls = %d\n", number_of_levels, number_of_calls);
  }
}













/* ritorna 1 se OT è un ciclo, 0 altrimenti;
 * - si assume OT connesso (altrimenti non è detto che OT sia un ciclo anche se la funzione ritorna 1;
 */
int is_cycle(onetree* OT) {
  int i, n_forced, n_removed;
  int n = (*OT).n;
 
  for (i = 1; i <= n; i++) {
    if (onetree_get_node_deg(OT, i) != 2)
      return 0;
  }
  return 1;
}












int propagate_constraints(graph* G, edgelist* edges_to_be_modified, edgelist* old_edges) { // old_edges dev'essere vuota alla prima chiamata
  edge e, old_e, mod_e;
  edgelist_node* current_node;

  int i, j;
  double c;

  edgelist modified_edges;
  edgelist temp_list;
  edgelist_init(&modified_edges);
  edgelist_init(&temp_list);

  int n = (*G).n;
  int m = edgelist_get_size(edges_to_be_modified);

  current_node = edgelist_get_first(edges_to_be_modified);

  for (i = 0; i < m; i++) {
    e = edgelist_get_element(edges_to_be_modified, current_node);
    edgelist_push_last(old_edges, e.x, e.y, graph_get_edge_cost(G, e.x, e.y));

    // applico il nuovo lato (o meglio il suo nuovo costo, BIG o SMALL)
    graph_set_edge_cost(G, e.x, e.y, e.cost);
    if (e.cost == BIG) {
      number_removed_edges_per_node[e.x-1]++;
      number_removed_edges_per_node[e.y-1]++;
    }
    else if (e.cost == SMALL) {
      number_forced_edges_per_node[e.x-1]++;
      number_forced_edges_per_node[e.y-1]++;
    }

    // aggiungo il lato modificato alla lista modified_edges
    edgelist_push_last(&modified_edges, e.x, e.y, e.cost);
    current_node = edgelist_get_next(edges_to_be_modified, current_node);   
  }




  while (!edgelist_is_empty(&modified_edges)) {



    // per ogni lato appena modificato effettuo un check per vedere se ci sono casi impossibili
    while (!edgelist_is_empty(&modified_edges)) {
      mod_e = edgelist_pop_first(&modified_edges); // il lato appena modificato è del tipo:
                                                   //
                                                   // [mod_e.x]----SMALL----[mod_e.y] 
                                                   // 
                                                   // oppure 
                                                   //                                                   
                                                   // [mod_e.x]-----BIG-----[mod_e.y]

      if (number_forced_edges_per_node[mod_e.x-1] > 2 || number_removed_edges_per_node[mod_e.x-1] > n-3) // più di 2 lati forzati incidenti nel nodo mod_e.x, oppure più di n-3 lati rimossi nel nodo mod_e.x
	return FAILURE;

      if (number_forced_edges_per_node[mod_e.y-1] > 2 || number_removed_edges_per_node[mod_e.y-1] > n-3) // più di 2 lati forzati incidenti nel nodo mod_e.y, oppure più di n-3 lati rimossi nel nodo mod_e.y
	return FAILURE;

      // presenza 3-cicli di lati forzati
      if (mod_e.cost <= SMALL) {
	for (i = 1; i <= n; i++) {
	  if (i != mod_e.x && i != mod_e.y) {
	    if (graph_get_edge_cost(G, i, mod_e.x) <= SMALL && graph_get_edge_cost(G, i, mod_e.y) <= SMALL)
	      return FAILURE;
	  }
	}
      }

      edgelist_push_last(&temp_list, mod_e.x, mod_e.y, mod_e.cost); // inserisco ogni lato della lista modified_edges in una lista temporanea: in questo modo, nella fase successiva
    }                                        // potrò processare ogni lato modificato e inserire i nuovi lati dedotti nella lista vuota modified_edges che verrà usata nell'iterazione successiva




    // effettuo deduzione vincoli
    while (!edgelist_is_empty(&temp_list)) {
      e = edgelist_pop_first(&temp_list);

     
      if (number_forced_edges_per_node[e.x-1] == 2) {  // se il numero di lati forzati incidenti nel nodo e.x è pari a 2, allora vieta tutti gli altri lati
	for (j = 1; j <= n; j++) {
	  if (j != e.x) {
	    c = graph_get_edge_cost(G, e.x, j);
	    if (c > SMALL && c < BIG) { // vieta il lato solo se il lato non è già vietato o già forzato

	      edgelist_push_last(old_edges, e.x, j, c); // salva il costo del lato originale
	      graph_set_edge_cost(G, e.x, j, BIG);
	      number_removed_edges_per_node[e.x-1]++;
	      number_removed_edges_per_node[j-1]++;

	      edgelist_push_last(&modified_edges, e.x, j, BIG); // inserisci il nuovo lato vietato nella lista dei lati modificati

	    }
	  }
	}
      }
      else if (number_removed_edges_per_node[e.x-1] == n-3) { // se il numero di lati rimossi incidenti nel nodo e.x è pari a n-3, allora forza i rimanenti due lati
	for (j = 1; j <= n; j++) {
	  if (j != e.x) {
	    c = graph_get_edge_cost(G, e.x, j);
	    if (c > SMALL && c < BIG) {

	      edgelist_push_last(old_edges, e.x, j, c);
	      graph_set_edge_cost(G, e.x, j, SMALL);
	      number_forced_edges_per_node[e.x-1]++;
	      number_forced_edges_per_node[j-1]++;

	      edgelist_push_last(&modified_edges, e.x, j, SMALL);

	    }
	  }
	}
      }

      if (number_forced_edges_per_node[e.y-1] == 2) {  // se il numero di lati forzati incidenti nel nodo e.y è pari a 2, allora vieta tutti gli altri lati
	for (i = 1; i <= n; i++) {
	  if (i != e.y) {
	    c = graph_get_edge_cost(G, i, e.y);
	    if (c > SMALL && c < BIG) {

	      edgelist_push_last(old_edges, i, e.y, c);
	      graph_set_edge_cost(G, i, e.y, BIG);
	      number_removed_edges_per_node[i-1]++;
	      number_removed_edges_per_node[e.y-1]++;

	      edgelist_push_last(&modified_edges, i, e.y, BIG);

	    }
	  }
	}
      }
      else if (number_removed_edges_per_node[e.y-1] == n-3) {  // se il numero di lati rimossi incidenti nel nodo e.y è pari a n-3, allora forza i rimanenti due lati
	for (i = 1; i <= n; i++) {
	  if (i != e.y) {
	    c = graph_get_edge_cost(G, i, e.y);
	    if (c > SMALL && c < BIG) {

	      edgelist_push_last(old_edges, i, e.y, c);
	      graph_set_edge_cost(G, i, e.y, SMALL);
	      number_forced_edges_per_node[i-1]++;
	      number_forced_edges_per_node[e.y-1]++;

	      edgelist_push_last(&modified_edges, i, e.y, SMALL);

	    }
	  }
	}
      }

    } // fine loop deduzione nuovi vincoli

  } // loop principale
  return SUCCESS;
}
