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
    (*status).z_best = input->ub;//onetree_get_cost(&(*input).H);
    onetree_copy(&(*input).H, &(*output).H_opt);
    (*output).z_opt = (*input).ub;

  }
  (*stats).number_of_calls++;
  (*status).current_call = (*stats).number_of_calls;
  (*status).current_level++;
  (*stats).number_of_levels = ((*status).current_level > (*stats).number_of_levels) ? (*status).current_level : (*stats).number_of_levels;

  // calcola un 1-albero
  double ub = compute_upper_bound(G_curr, OT_curr, NEAREST_NEIGHBOUR);
  compute_lagrange(G_curr, OT_curr, ub + EPSILON);
  //compute_ot(G_curr, OT_curr);

  // calcola il costo dell'1-albero con i costi originari dei lati del grafo di input
  (*status).z_curr = 0.0;
  for (i = 1; i <= n; i++) {
    (*status).z_curr += graph_get_edge_cost(&(*input).G, onetree_get_pred(OT_curr, i), i);
  }

  if ((*status).z_curr >= (*status).z_best) { // >= (*status).z_best +EPSILON ? 
    return;
  }

  printf("node: %d\n", status->current_call);

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
    bb_backup backup_1, backup_2, backup_3;
    bb_backup_init(&backup_1); bb_backup_init(&backup_2); bb_backup_init(&backup_3);



    // BRANCH #1 : vieta il lato {w, v};

    /*
    // a | ap_simple | ap_recursive | ap_iterative
    bb_backup_save(&backup_1, status, N, N, Y, Y, Y, Y, N);
    bb_constraints_ap_iterative(w, v, 0, BIG, 0.0, status, &backup_1, NULL);
    bb_solver(input, output, status, stats);
    bb_backup_restore(&backup_1, status, N, N, Y, Y, Y, Y, N, Y);
    bb_backup_delete(&backup_1);
    */

    /*
    // ac | acp_simple | acp_recursive | apc_iterative
    bb_backup_save(&backup_1, status, N, N, Y, Y, Y, Y, N);
    if (bb_constraints_acp_iterative(w, v, 0, BIG, 0.0, status, &backup_1, NULL) == SUCCESS) {
      bb_solver(input, output, status, stats);
    }
    bb_backup_restore(&backup_1, status, N, N, Y, Y, Y, Y, N, Y);
    bb_backup_delete(&backup_1);
    */

    // ace | acep_simple | acep_recursive | acep_iterative
    bb_backup_save(&backup_1, status, N, N, Y, Y, Y, Y, N);
    if (bb_constraints_acp_iterative(w, v, 0, BIG, 0.0, status, &backup_1, NULL) == SUCCESS) {
      bb_solver(input, output, status, stats);
    }
    bb_backup_restore(&backup_1, status, N, N, Y, Y, Y, Y, N, Y);
    bb_backup_delete(&backup_1);
    


    // BRANCH #2 : forza il lato {w, v} e vieta il lato {w, u};
    
    /*
    // a | ap_simple | ap_recursive | ap_iterative
    bb_backup_save(&backup_2, status, N, N, Y, Y, Y, Y, N);
    bb_constraints_ap_iterative(w, v, u, SMALL, BIG, status, &backup_2, NULL);
    bb_solver(input, output, status, stats);
    bb_backup_restore(&backup_2, status, N, N, Y, Y, Y, Y, N, Y);
    bb_backup_delete(&backup_2);
    */

    /*
    // ac | acp_simple | acp_recursive | apc_iterative
    bb_backup_save(&backup_2, status, N, N, Y, Y, Y, Y, N);
    if (bb_constraints_acp_iterative(w, v, u, SMALL, BIG, status, &backup_2, NULL) == SUCCESS) {
     bb_solver(input, output, status, stats);
    }
    bb_backup_restore(&backup_2, status, N, N, Y, Y, Y, Y, N, Y);
    bb_backup_delete(&backup_2);
    */

    // ace | acep_simple | acep_recursive | acep_iterative
    bb_backup_save(&backup_2, status, N, N, Y, Y, Y, Y, N);
    if (bb_constraints_acp_iterative(w, v, u, SMALL, BIG, status, &backup_2, NULL) == SUCCESS) {
      bb_solver(input, output, status, stats);
    }
    bb_backup_restore(&backup_2, status, N, N, Y, Y, Y, Y, N, Y);
    bb_backup_delete(&backup_2);
    

    // BRANCH #3 : forza i lati {w, v} e {w, u};
   
    /*
    // a | ap_simple | ap_recursive | ap_iterative
    bb_backup_save(&backup_3, status, N, N, Y, Y, Y, Y, N);
    bb_constraints_ap_iterative(w, v, u, SMALL, SMALL, status, &backup_3, NULL);
    bb_solver(input, output, status, stats);
    bb_backup_restore(&backup_3, status, N, N, Y, Y, Y, Y, N, Y);
    bb_backup_delete(&backup_3);
    */

    /*
    // ac | acp_simple | acp_recursive | apc_iterative
    bb_backup_save(&backup_3, status, N, N, Y, Y, Y, Y, N);
    if (bb_constraints_acp_iterative(w, v, u, SMALL, SMALL, status, &backup_3, NULL) == SUCCESS) {
      bb_solver(input, output, status, stats);
    }
    bb_backup_restore(&backup_3, status, N, N, Y, Y, Y, Y, N, Y);
    bb_backup_delete(&backup_3);
    */

    // ace | acep_simple | acep_recursive | acep_iterative
    bb_backup_save(&backup_3, status, N, N, Y, Y, Y, Y, N);
    if (bb_constraints_acp_iterative(w, v, u, SMALL, SMALL, status, &backup_3, NULL) == SUCCESS) {
      bb_solver(input, output, status, stats);
    }
    bb_backup_restore(&backup_3, status, N, N, Y, Y, Y, Y, N, Y);
    bb_backup_delete(&backup_3);
        

  }


  // se esiste un solo lato mai forzato e mai vietato, allora procedi con BRANCHING A 2 VIE
  else if (v > 0) {
    bb_backup backup_1, backup_2;
    bb_backup_init(&backup_1); bb_backup_init(&backup_2);


    // BRANCH #1 : vieta il lato {w, v}
 
    /*
    // a | ap_simple | ap_recursive | ap_iterative
    bb_backup_save(&backup_1, status, N, N, Y, Y, Y, Y, N);
    bb_constraints_ap_iterative(w, v, 0, BIG, 0.0, status, &backup_1, NULL);
    bb_solver(input, output, status, stats);
    bb_backup_restore(&backup_1, status, N, N, Y, Y, Y, Y, N, Y);
    bb_backup_delete(&backup_1);
    */

    /*
    // ac | acp_simple | acp_recursive | apc_iterative
    bb_backup_save(&backup_1, status, N, N, Y, Y, Y, Y, N);
    if (bb_constraints_acp_iterative(w, v, 0, BIG, 0.0, status, &backup_1, NULL) == SUCCESS) {
      bb_solver(input, output, status, stats);
    }
    bb_backup_restore(&backup_1, status, N, N, Y, Y, Y, Y, N, Y);
    bb_backup_delete(&backup_1);
    */

    // ace | acep_simple | acep_recursive | acep_iterative
    bb_backup_save(&backup_1, status, N, N, Y, Y, Y, Y, N);
    if (bb_constraints_acp_iterative(w, v, 0, BIG, 0.0, status, &backup_1, NULL) == SUCCESS) {
      bb_solver(input, output, status, stats);
    }
    bb_backup_restore(&backup_1, status, N, N, Y, Y, Y, Y, N, Y);
    bb_backup_delete(&backup_1);


    // BRANCH #2 : forza il lato {w, u};
 
    /*
    // a | ap_simple | ap_recursive | ap_iterative
    bb_backup_save(&backup_2, status, N, N, Y, Y, Y, Y, N);
    bb_constraints_ap_iterative(w, v, 0, SMALL, 0.0, status, &backup_2, NULL);
    bb_solver(input, output, status, stats);
    bb_backup_restore(&backup_2, status, N, N, Y, Y, Y, Y, N, Y);
    bb_backup_delete(&backup_2);
    */

    /*
    // ac | acp_simple | acp_recursive | apc_iterative
    bb_backup_save(&backup_2, status, N, N, Y, Y, Y, Y, N);
    if (bb_constraints_acp_iterative(w, v, 0, SMALL, 0.0, status, &backup_2, NULL) == SUCCESS) {
      bb_solver(input, output, status, stats);
    }
    bb_backup_restore(&backup_2, status, N, N, Y, Y, Y, Y, N, Y);
    bb_backup_delete(&backup_2);
    */

    // ace | acep_simple | acep_recursive | acep_iterative
    bb_backup_save(&backup_2, status, N, N, Y, Y, Y, Y, N);
    if (bb_constraints_acp_iterative(w, v, 0, SMALL, 0.0, status, &backup_2, NULL) == SUCCESS) {
      bb_solver(input, output, status, stats);
    }
    bb_backup_restore(&backup_2, status, N, N, Y, Y, Y, Y, N, Y);
    bb_backup_delete(&backup_2);

  }

  if ((*status).current_call == 1) {
    onetree_copy(&(*status).H_best, &(*output).H_opt);
    (*output).z_opt = (*status).z_best;
  }

  return;
}

