#include "bb_solver.h"

void bb_solver(bb_input* input, bb_output* output, bb_status* status, bb_stats* stats) {

  int i, j;

  int n = (*input).G.n;
  // memorizza i puntatori più utilizzati
  graph* G_curr = &(*status).G_curr;
  onetree* OT_curr = &(*status).OT_curr;


  if ((*status).current_call == 0) {

    graph_copy(&(*input).G, G_curr);
    onetree_copy(&(*input).H, &(*status).H_best);
    (*status).z_best = onetree_get_cost(&(*input).H);
    onetree_copy(&(*input).H, &(*output).H_opt);
    (*output).z_opt = (*input).ub;

  }
  (*stats).number_of_calls++;
  (*status).current_call = (*stats).number_of_calls;
  (*status).current_level++;
  (*stats).number_of_levels = ((*status).current_level > (*stats).number_of_levels) ? (*status).current_level : (*stats).number_of_levels;

  if ((*status).current_call == 1) {
    // calcola un 1-albero
    double ub = compute_upper_bound(G_curr, OT_curr, NEAREST_NEIGHBOUR);
    compute_lagrange(G_curr, OT_curr, ub+EPSILON);
    // calcola il costo dell'1-albero con i costi originari dei lati del grafo di input
    (*status).z_curr = 0.0;
    for (i = 1; i <= n; i++) {
      (*status).z_curr += graph_get_edge_cost(&(*input).G, onetree_get_pred(OT_curr, i), i);
    }
  }

  if ((*status).z_curr >= (*status).z_best) { // >= (*status).z_best+EPSILON ? 
    return;
  }

  if (onetree_is_cycle(OT_curr)) {
    onetree_copy(OT_curr, &(*status).H_best);
    (*status).z_best = (*status).z_curr;
    printf("# updated incumbent = %f : current_call = %d : current level = %d\n", (*status).z_best, (*status).current_call, (*status).current_level);
    if ((*status).current_call == 1) {
      onetree_copy(OT_curr, &(*output).H_opt);
      (*output).z_opt = (*status).z_curr;
    }
    return;
  }

  int w, v, u;
  // seleziona un nodo per il branching
  bb_select_node(status, &w);
  // seleziona due lati per il branching
  bb_select_edges(status, &w, &v, &u);

  // se esistono due lati mai forzati e mai vietati, allora procedi con BRANCHING A 3 VIE
  if (v > 0 && u > 0) {
    int k;
    double z1, z2, z3, ub, ub_nn;
    int branch_selection[3];
    bb_backup backup_fc1, backup_fc2, backup_fc3;
    bb_backup_init(&backup_fc1); bb_backup_init(&backup_fc2); bb_backup_init(&backup_fc3);
    bb_backup update_fc1, update_fc2, update_fc3;
    bb_backup_init(&update_fc1); bb_backup_init(&update_fc2); bb_backup_init(&update_fc3);
 
    // vieta il lato {w, v};
    
    /*
    // a | ap_simple | ap_recursive | ap_iterative
    bb_backup_save(&backup_fc1, status, N, N, Y, Y, Y, Y, N);
    bb_constraints_ap_iterative(w, v, 0, BIG, 0.0, status, &backup_fc1, &update_fc1);
    compute_lagrange(G_curr, OT_curr, compute_upper_bound(G_curr, OT_curr, NEAREST_NEIGHBOUR)+EPSILON);
    (*status).z_curr = 0.0;
    for (i = 1; i <= n; i++) {
      z1 = (*status).z_curr += graph_get_edge_cost(&(*input).G, onetree_get_pred(OT_curr, i), i);
    }
    bb_backup_save(&update_fc1, status, Y, Y, Y, Y, N, N, N);
    bb_backup_restore(&backup_fc1, status, N, N, Y, Y, Y, Y, N, Y);
    */

    /*
    // ac | acp_simple | acp_recursive | acp_iterative
    bb_backup_save(&backup_fc1, status, N, N, Y, Y, Y, Y, N);
    if (bb_constraints_acp_iterative(w, v, 0, BIG, 0.0, status, &backup_fc1, &update_fc1) == SUCCESS) {
      compute_lagrange(G_curr, OT_curr, compute_upper_bound(G_curr, OT_curr, NEAREST_NEIGHBOUR)+EPSILON);
      (*status).z_curr = 0.0;
      for (i = 1; i <= n; i++) {
	z1 = (*status).z_curr += graph_get_edge_cost(&(*input).G, onetree_get_pred(OT_curr, i), i);
      }
    }
    else {
      z1 = (*status).z_best;
    }
    bb_backup_save(&update_fc1, status, Y, Y, Y, Y, N, N, N);
    bb_backup_restore(&backup_fc1, status, N, N, Y, Y, Y, Y, N, Y);
    */

    // ace | acep_simple | acep_recursive | acep_iterative
    bb_backup_save(&backup_fc1, status, N, N, Y, Y, Y, Y, Y);
    if (bb_constraints_acep_iterative(w, v, 0, BIG, 0.0, status, &backup_fc1, &update_fc1) == SUCCESS) {
      compute_lagrange(G_curr, OT_curr, compute_upper_bound(G_curr, OT_curr, NEAREST_NEIGHBOUR) + EPSILON);
      (*status).z_curr = 0.0;
      for (i = 1; i <= n; i++) {
	z1 = (*status).z_curr += graph_get_edge_cost(&(*input).G, onetree_get_pred(OT_curr, i), i);
      }
    }
    else {
      z1 = (*status).z_best;
    }
    bb_backup_save(&update_fc1, status, Y, Y, Y, Y, N, N, Y);
    bb_backup_restore(&backup_fc1, status, N, N, Y, Y, Y, Y, Y, Y);


    // forza il lato {w, v} e vieta il lato {w, u};

    /*
    // a | ap_simple | ap_recursive | ap_iterative
    bb_backup_save(&backup_fc2, status, N, N, Y, Y, Y, Y, N);
    bb_constraints_ap_iterative(w, v, u, SMALL, BIG, status, &backup_fc2, &update_fc2);
    compute_lagrange(G_curr, OT_curr, compute_upper_bound(G_curr, OT_curr, NEAREST_NEIGHBOUR)+EPSILON);
    (*status).z_curr = 0.0;
    for (i = 1; i <= n; i++) {
      z2 = (*status).z_curr += graph_get_edge_cost(&(*input).G, onetree_get_pred(OT_curr, i), i);
    }
    bb_backup_save(&update_fc2, status, Y, Y, Y, Y, N, N, N);
    bb_backup_restore(&backup_fc2, status, N, N, Y, Y, Y, Y, N, Y);
    */

    /* 
    // ac | acp_simple | acp_recursive | acp_iterative
    bb_backup_save(&backup_fc2, status, N, N, Y, Y, Y, Y, N);
    if (bb_constraints_acp_iterative(w, v, u, SMALL, BIG, status, &backup_fc2, &update_fc2) == SUCCESS) {
      compute_lagrange(G_curr, OT_curr, compute_upper_bound(G_curr, OT_curr, NEAREST_NEIGHBOUR)+EPSILON);
      (*status).z_curr = 0.0;
      for (i = 1; i <= n; i++) {
	z2 = (*status).z_curr += graph_get_edge_cost(&(*input).G, onetree_get_pred(OT_curr, i), i);
      }
    }
    else {
      z2 = (*status).z_best;
    }
    bb_backup_save(&update_fc2, status, Y, Y, Y, Y, N, N, N);
    bb_backup_restore(&backup_fc2, status, N, N, Y, Y, Y, Y, N, Y);
    */

    // ace | acep_simple | acep_recursive | acep_iterative
    bb_backup_save(&backup_fc2, status, N, N, Y, Y, Y, Y, Y);
    if (bb_constraints_acep_iterative(w, v, u, SMALL, BIG, status, &backup_fc2, &update_fc2) == SUCCESS) {
      compute_lagrange(G_curr, OT_curr, compute_upper_bound(G_curr, OT_curr, NEAREST_NEIGHBOUR) + EPSILON);
      (*status).z_curr = 0.0;
      for (i = 1; i <= n; i++) {
	z2 = (*status).z_curr += graph_get_edge_cost(&(*input).G, onetree_get_pred(OT_curr, i), i);
      }
    }
    else {
      z2 = (*status).z_best;
    }
    bb_backup_save(&update_fc2, status, Y, Y, Y, Y, N, N, Y);
    bb_backup_restore(&backup_fc2, status, N, N, Y, Y, Y, Y, Y, Y);    

    // forza i lati {w, v} e {w, u};

    /*
    // a | ap_simple | ap_recursive | ap_iterative
    bb_backup_save(&backup_fc3, status, N, N, Y, Y, Y, Y, N);
    bb_constraints_ap_iterative(w, v, u, SMALL, SMALL, status, &backup_fc3, &update_fc3);
    compute_lagrange(G_curr, OT_curr, compute_upper_bound(G_curr, OT_curr, NEAREST_NEIGHBOUR)+EPSILON);
    (*status).z_curr = 0.0;
    for (i = 1; i <= n; i++) {
      z3 = (*status).z_curr += graph_get_edge_cost(&(*input).G, onetree_get_pred(OT_curr, i), i);
    }
    bb_backup_save(&update_fc3, status, Y, Y, Y, Y, N, N, N);
    bb_backup_restore(&backup_fc3, status, N, N, Y, Y, Y, Y, N, Y);
    */

    /*
    // ac | acp_simple | acp_recursive | acp_iterative
    bb_backup_save(&backup_fc3, status, N, N, Y, Y, Y, Y, N);
    if (bb_constraints_acp_iterative(w, v, u, SMALL, SMALL, status, &backup_fc3, &update_fc3) == SUCCESS) {
      compute_lagrange(G_curr, OT_curr, compute_upper_bound(G_curr, OT_curr, NEAREST_NEIGHBOUR)+EPSILON);
      (*status).z_curr = 0.0;
      for (i = 1; i <= n; i++) {
	z3 = (*status).z_curr += graph_get_edge_cost(&(*input).G, onetree_get_pred(OT_curr, i), i);
      }
    }
    else {
      z3 = (*status).z_best;
    }
    bb_backup_save(&update_fc3, status, Y, Y, Y, Y, N, N, N);
    bb_backup_restore(&backup_fc3, status, N, N, Y, Y, Y, Y, N, Y);
    */

    // ace | acep_simple | acep_recursive | acep_iterative
    bb_backup_save(&backup_fc3, status, N, N, Y, Y, Y, Y, Y);
    if (bb_constraints_acep_iterative(w, v, u, SMALL, SMALL, status, &backup_fc3, &update_fc3) == SUCCESS) {
      compute_lagrange(G_curr, OT_curr, compute_upper_bound(G_curr, OT_curr, NEAREST_NEIGHBOUR) + EPSILON);
      (*status).z_curr = 0.0;
      for (i = 1; i <= n; i++) {
	z3 = (*status).z_curr += graph_get_edge_cost(&(*input).G, onetree_get_pred(OT_curr, i), i);
      }
    }
    else {
      z3 = (*status).z_best;
    }
    bb_backup_save(&update_fc3, status, Y, Y, Y, Y, N, N, Y);
    bb_backup_restore(&backup_fc3, status, N, N, Y, Y, Y, Y, Y, Y);



    if (z1 >= z2 && z2 >= z3)
      { branch_selection[0] = 1; branch_selection[1] = 2; branch_selection[2] = 3; }
    else if (z1 >= z3 && z3 >= z2)
      { branch_selection[0] = 1; branch_selection[1] = 3; branch_selection[2] = 2; }
    else if (z2 >= z1 && z1 >= z3)
      { branch_selection[0] = 2; branch_selection[1] = 1; branch_selection[2] = 3; }
    else if (z2 >= z3 && z3 >= z1)
      { branch_selection[0] = 2; branch_selection[1] = 3; branch_selection[2] = 1; }
    else if (z3 >= z1 && z1 >= z2)
      { branch_selection[0] = 3; branch_selection[1] = 1; branch_selection[2] = 2; }
    else
      { branch_selection[0] = 3; branch_selection[1] = 2; branch_selection[2] = 1; }

    for (k = 0; k < 3; k++) {
      switch (branch_selection[k]) {

      case 1:
	if (z1 < (*status).z_best) {
	  //bb_backup_restore(&update_fc1, status, Y, Y, Y, Y, N, N, N, Y);
	  bb_backup_restore(&update_fc1, status, Y, Y, Y, Y, N, N, Y, Y); // ENANCHED
	  bb_solver(input, output, status, stats);
	  //bb_backup_restore(&backup_fc1, status, N, N, Y, Y, Y, Y, N, Y);
	  bb_backup_restore(&backup_fc1, status, N, N, Y, Y, Y, Y, Y, Y); // ENANCHED
	  }
	bb_backup_delete(&backup_fc1);
	bb_backup_delete(&update_fc1);
	break;

      case 2:
	if (z2 < (*status).z_best) {
	  //bb_backup_restore(&update_fc2, status, Y, Y, Y, Y, N, N, N, Y);
	  bb_backup_restore(&update_fc2, status, Y, Y, Y, Y, N, N, Y, Y); // ENANCHED
	  bb_solver(input, output, status, stats);
	  //bb_backup_restore(&backup_fc2, status, N, N, Y, Y, Y, Y, N, Y);
	  bb_backup_restore(&backup_fc2, status, N, N, Y, Y, Y, Y, Y, Y); // ENANCHED
	  }
	bb_backup_delete(&backup_fc2);
	bb_backup_delete(&update_fc2);
	break;

      case 3:
	if (z3 < (*status).z_best) {
	  //bb_backup_restore(&update_fc3, status, Y, Y, Y, Y, N, N, N, Y);
	  bb_backup_restore(&update_fc3, status, Y, Y, Y, Y, N, N, Y, Y); // ENANCHED
	  bb_solver(input, output, status, stats);
	  //bb_backup_restore(&backup_fc3, status, N, N, Y, Y, Y, Y, N, Y);
	  bb_backup_restore(&backup_fc3, status, N, N, Y, Y, Y, Y, Y, Y); // ENANCHED
	  }
	bb_backup_delete(&backup_fc3);
	bb_backup_delete(&update_fc3);
	break;

      default:
	break;
      
      }
    }

  }

  // se esiste un solo lato mai forzato e mai vietato, allora procedi con BRANCHING A 2 VIE
  else if (v > 0) {
    int k;
    double z1, z2;
    int branch_selection[2];
    bb_backup backup_fc1, backup_fc2;
    bb_backup_init(&backup_fc1); bb_backup_init(&backup_fc2);
    bb_backup update_fc1, update_fc2;
    bb_backup_init(&update_fc1); bb_backup_init(&update_fc2);

    // vieta il lato {w, v}

    /*
    // a | ap_simple | ap_recursive | ap_iterative
    bb_backup_save(&backup_fc1, status, N, N, Y, Y, Y, Y, N);
    bb_constraints_ap_iterative(w, v, 0, BIG, 0.0, status, &backup_fc1, &update_fc1 );
    compute_lagrange(G_curr, OT_curr, compute_upper_bound(G_curr, OT_curr, NEAREST_NEIGHBOUR)+EPSILON);
    (*status).z_curr = 0.0;
    for (i = 1; i <= n; i++) {
      z1 = (*status).z_curr += graph_get_edge_cost(&(*input).G, onetree_get_pred(OT_curr, i), i);
    }
    bb_backup_save(&update_fc1, status, Y, Y, Y, Y, N, N, N);
    bb_backup_restore(&backup_fc1, status, N, N, Y, Y, Y, Y, N, Y);
    */

    /*
    // ac | acp_simple | acp_recursive | acp_iterative
    bb_backup_save(&backup_fc1, status, N, N, Y, Y, Y, Y, N);
    if (bb_constraints_acp_iterative(w, v, 0, BIG, 0.0, status, &backup_fc1, &update_fc1) == SUCCESS) {
      compute_lagrange(G_curr, OT_curr, compute_upper_bound(G_curr, OT_curr, NEAREST_NEIGHBOUR)+EPSILON);
      (*status).z_curr = 0.0;
      for (i = 1; i <= n; i++) {
	z1 = (*status).z_curr += graph_get_edge_cost(&(*input).G, onetree_get_pred(OT_curr, i), i);
      }
    }
    else {
      z1 = (*status).z_best;
    }
    bb_backup_save(&update_fc1, status, Y, Y, Y, Y, N, N, N);
    bb_backup_restore(&backup_fc1, status, N, N, Y, Y, Y, Y, N, Y);
    */

    // ace | acep_simple | acep_recursive | acep_iterative
    bb_backup_save(&backup_fc1, status, N, N, Y, Y, Y, Y, Y);
    if (bb_constraints_acep_iterative(w, v, 0, BIG, 0.0, status, &backup_fc1, &update_fc1) == SUCCESS) {
      compute_lagrange(G_curr, OT_curr, compute_upper_bound(G_curr, OT_curr, NEAREST_NEIGHBOUR) + EPSILON);
      (*status).z_curr = 0.0;
      for (i = 1; i <= n; i++) {
	z1 = (*status).z_curr += graph_get_edge_cost(&(*input).G, onetree_get_pred(OT_curr, i), i);
      }
    }
    else {
      z1 = (*status).z_best;
    }
    bb_backup_save(&update_fc1, status, Y, Y, Y, Y, N, N, Y);
    bb_backup_restore(&backup_fc1, status, N, N, Y, Y, Y, Y, Y, Y);

    // forza il lato {w, u};

    /*
    // a | ap_simple | ap_recursive | ap_iterative
    bb_backup_save(&backup_fc2, status, N, N, Y, Y, Y, Y, N);
    bb_constraints_ap_iterative(w, v, 0, SMALL, 0.0, status, &backup_fc2, &update_fc2);
    compute_lagrange(G_curr, OT_curr, compute_upper_bound(G_curr, OT_curr, NEAREST_NEIGHBOUR)+EPSILON);
    (*status).z_curr = 0.0;
    for (i = 1; i <= n; i++) {
      z2 = (*status).z_curr += graph_get_edge_cost(&(*input).G, onetree_get_pred(OT_curr, i), i);
    }
    bb_backup_save(&update_fc2, status, Y, Y, Y, Y, N, N, N);
    bb_backup_restore(&backup_fc2, status, N, N, Y, Y, Y, Y, N, Y);
    */

    /*
    // ac | acp_simple | acp_recursive | acp_iterative
    bb_backup_save(&backup_fc2, status, N, N, Y, Y, Y, Y, N);
    if (bb_constraints_acp_iterative(w, v, 0, SMALL, 0.0, status, &backup_fc2, &update_fc2) == SUCCESS) {
      compute_lagrange(G_curr, OT_curr, compute_upper_bound(G_curr, OT_curr, NEAREST_NEIGHBOUR)+EPSILON);
      (*status).z_curr = 0.0;
      for (i = 1; i <= n; i++) {
	z2 = (*status).z_curr += graph_get_edge_cost(&(*input).G, onetree_get_pred(OT_curr, i), i);
      }
    }
    else {
      z2 = (*status).z_best;
    }
    bb_backup_save(&update_fc2, status, Y, Y, Y, Y, N, N, N);
    bb_backup_restore(&backup_fc2, status, N, N, Y, Y, Y, Y, N, Y);
    */

    // ace | acep_simple | acep_recursive | acep_iterative
    bb_backup_save(&backup_fc2, status, N, N, Y, Y, Y, Y, Y);
    if (bb_constraints_acep_iterative(w, v, 0, SMALL, 0.0, status, &backup_fc2, &update_fc2) == SUCCESS) {
      compute_lagrange(G_curr, OT_curr, compute_upper_bound(G_curr, OT_curr, NEAREST_NEIGHBOUR) + EPSILON);
      (*status).z_curr = 0.0;
      for (i = 1; i <= n; i++) {
	z2 = (*status).z_curr += graph_get_edge_cost(&(*input).G, onetree_get_pred(OT_curr, i), i);
      }
    }
    else {
      z2 = (*status).z_best;
    }
    bb_backup_save(&update_fc2, status, Y, Y, Y, Y, N, N, Y);
    bb_backup_restore(&backup_fc2, status, N, N, Y, Y, Y, Y, Y, Y);


    if (z1 >= z2)
      { branch_selection[0] = 1; branch_selection[1] = 2; }
    else
      { branch_selection[0] = 2; branch_selection[1] = 1; }

    for (k = 0; k < 3; k++) {
      switch (branch_selection[k]) {

      case 1:
	if (z1 < (*status).z_best) {
	  //bb_backup_restore(&update_fc1, status, Y, Y, N, N, N, N, N, Y);
	  bb_backup_restore(&update_fc1, status, Y, Y, N, N, N, N, Y, Y); // ENANCHED
	  bb_solver(input, output, status, stats);
	  //bb_backup_restore(&backup_fc1, status, N, N, N, N, Y, Y, N, Y);
	  bb_backup_restore(&backup_fc1, status, N, N, N, N, Y, Y, Y, Y); // ENANCHED
	  }
	bb_backup_delete(&backup_fc1);
	bb_backup_delete(&update_fc1);
	break;


      case 2:
	if (z2 < (*status).z_best) {
	  //bb_backup_restore(&update_fc2, status, Y, Y, N, N, N, N, N, Y);
	  bb_backup_restore(&update_fc2, status, Y, Y, N, N, N, N, Y, Y); // ENANCHED
	  bb_solver(input, output, status, stats);
	  //bb_backup_restore(&backup_fc2, status, N, N, N, N, Y, Y, N, Y);
	  bb_backup_restore(&backup_fc2, status, N, N, N, N, Y, Y, Y, Y); // ENANCHED
	  }
	bb_backup_delete(&backup_fc2);
	bb_backup_delete(&update_fc2);
	break;

      }
    }

  }

  if ((*status).current_call == 1) {
    onetree_copy(&(*status).H_best, &(*output).H_opt);
    (*output).z_opt = (*status).z_best;
  }

  return;
}
