#include "tsp_solve.h"

void tsp_solve(tsp_input* input, tsp_output* output, tsp_status* status, tsp_stats* stats) {

  int i, j;

  int n = (*input).G.n;
  // memorizza i puntatori più utilizzati
  graph* G_curr = &(*status).G_curr;
  onetree* OT_curr = &(*status).OT_curr;


  if ((*status).current_call == 0) {

    graph_copy(&(*input).G, G_curr);
    onetree_copy(&(*input).H, &(*status).H_best);
    (*status).z_best = onetree_get_cost(&(*input).H); // eventualmente status.z_best=input.ub, ma ciò imporrebbe di fornire in input un ub che è esattamente il costo del tour input.H

  }
  (*stats).number_of_calls++;
  (*status).current_call = (*stats).number_of_calls;
  (*status).current_level++;
  (*stats).number_of_levels = ((*status).current_level > (*stats).number_of_levels) ? (*status).current_level : (*stats).number_of_levels;

  // calcola un 1-albero
  double ub = compute_upper_bound(G_curr, OT_curr);
  compute_lagrange(G_curr, OT_curr, ub+EPSILON);
  // calcola il costo dell'1-albero con i costi originari dei lati del grafo di input
  (*status).z_curr = 0.0;
  for (i = 1; i <= n; i++) {
    (*status).z_curr += graph_get_edge_cost(&(*input).G, onetree_get_pred(OT_curr, i), i);
  }

  //printf("z curr = %.60f\nz best = %.60f\nub = %.60f\n", (*status).z_curr, (*status).z_best, ub);
  if ((*status).z_curr >= (*status).z_best+EPSILON) {
    return;
  }

  if (onetree_is_cycle(OT_curr)) {
    if ((*status).current_call != 1) {
      onetree_copy(OT_curr, &(*status).H_best);
      (*status).z_best = (*status).z_curr;
    }
    else if ((*status).current_call == 1) {
      onetree_copy(OT_curr, &(*output).H_opt);
      (*output).z_opt = (*status).z_best = (*status).z_curr;
    }
    printf("# updated incumbent = %f : current_call = %d : current level = %d\n", (*status).z_best, (*status).current_call, (*status).current_level);
    return;
  }


  int w, v, u;
  // seleziona un nodo per il branching
  tsp_select_node(status, &w);
  // seleziona due lati per il branching
  tsp_select_edges(status, &w, &v, &u);


  tsp_backup backup;
  tsp_backup_init(&backup);
  tsp_backup_save(&backup, status, N, N, N, N, Y, Y, N);

  // se esistono due lati mai forzati e mai vietati, allora procedi con BRANCHING A 3 VIE
  if (v > 0 && u > 0) {

    // vieta il lato {w, v};
    tsp_constraints_a(w, v, 0, BIG, 0.0, status, &backup);
    tsp_solve(input, output, status, stats);
    tsp_backup_restore(&backup, status, N, N, N, N, Y, Y, N, Y);


    // forza il lato {w, v} e vieta il lato {w, u};
    tsp_constraints_a(w, v, u, SMALL, BIG, status, &backup);
    tsp_solve(input, output, status, stats);
    tsp_backup_restore(&backup, status, N, N, N, N, Y, Y, N, Y);

    // forza i lati {w, v} e {w, u};
    tsp_constraints_a(w, v, u, SMALL, SMALL, status, &backup);
    tsp_solve(input, output, status, stats);
    tsp_backup_restore(&backup, status, N, N, N, N, Y, Y, N, Y);

  }

  // se esiste un solo lato mai forzato e mai vietato, allora procedi con BRANCHING A 2 VIE
  else if (v > 0) {

    // vieta il lato {w, v}
    tsp_constraints_a(w, v, 0, BIG, 0.0, status, &backup);
    tsp_solve(input, output, status, stats);
    tsp_backup_restore(&backup, status, N, N, N, N, Y, Y, N, Y);

    // forza il lato {w, u};
    tsp_constraints_a(w, v, 0, SMALL, 0.0, status, &backup);
    tsp_solve(input, output, status, stats);
    tsp_backup_restore(&backup, status, N, N, N, N, Y, Y, N, Y);

  }

  if ((*status).current_call == 1) {
    onetree_copy(&(*status).H_best, &(*output).H_opt);
    (*output).z_opt = (*status).z_best;
  }

  tsp_backup_delete(&backup);
  return;
}

