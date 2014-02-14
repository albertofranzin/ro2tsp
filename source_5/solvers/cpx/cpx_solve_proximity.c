#include "cpx_solve_proximity.h"

int cpx_solve_proximity(CPXENVptr  env, 
                        CPXLPptr   lp, 
                        cpx_env   *ce, 
                        cpx_stats *cs, 
                        double    *x, 
                        int        x_size, 
                        int       *solstat) {
 

  int n = ce->G.n;
  int numcols = n * (n - 1) / 2;
  double *x_feas = malloc(numcols * sizeof(double));
  cpx_constraint cutoff;
  cpx_constraint_init(&cutoff, numcols);
  double theta = 1.0;

  int i, j, idx, cutoff_idx, status;

  if (x_size != numcols) return 1;


  for (idx = 0; idx < numcols; idx++) {
    x_feas[idx] = 0.0;
  }

  for (i = 0; i < n; i++) {
    idx_from_vrtx(&ce->T, ce->TOUR_HEUR.nodes[i], ce->TOUR_HEUR.nodes[(i+1) % n], &idx);
    x_feas[idx] = 1.0;
  }

  status = cpx_recenter_obj(env, lp, x_feas, numcols);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_proximity.c:\n"
                    "function: cpx_solve_proximity:\n"
                    "cpx_recenter_obj: %d\n", status);
    return 1;
  }


  status = cpx_constraint_generate_proximity_cutoff(ce, x_feas, numcols, theta, &cutoff);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_proximity.c:\n"
                    "function: cpx_solve_proximity:\n"
                    "cpx_constraint_generate_proximity_cutoff: %d\n", status);
    return 1;
  }


  status = cpx_constraint_add(env, lp, &cutoff);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_proximity.c:\n"
                    "function: cpx_solve_proximity:\n"
                    "cpx_constraint_add: %d\n", status);
    return 1;
  }

  cutoff_idx = CPXgetnumrows(env, lp) - 1;
  //printf("CUTOFF IDX = %d\n", cutoff_idx);

  int cnt;
  int    indices[numcols];
  char   lu[numcols];
  double bd[numcols];



  int iter = 0;
  double z_feas;
  int begin, end;
  while (iter < 1) {
    printf("iter = %d\n", iter);
    iter++;
    /*
    printf("numcols = %d\n", CPXgetnumcols(env, lp));
    printf("numrows = %d\n", CPXgetnumrows(env, lp));
    */
    cpx_solve_iterative(env, lp, ce, cs, x_feas, numcols, &status);
    //printf("NUMCOLS = %d\n", CPXgetnumcols(env, lp));
    /*
    printf("numcols = %d\n", CPXgetnumcols(env, lp));
    printf("numrows = %d\n", CPXgetnumrows(env, lp));
    printf("============================\n");
    */

    if (status == 103) {printf("CIAO!\n"); break; }
    printf("SOLSTAT = %d\n", status);
    getchar();

    for (idx = 0; idx < numcols; idx++) {
      x[idx] = x_feas[idx];
    }
    *solstat = status;

    z_feas = 0.0;
    for (idx = 0; idx < numcols; idx++) {
      if (x_feas[idx] > 0.9) {
        vrtx_from_idx(&ce->T, &i, &j, idx);
        z_feas += graph_get_edge_cost(&ce->G, i, j);
      }
    }
    printf("feas. sol = %f\n", z_feas);

    status = cpx_recenter_obj(env, lp, x_feas, numcols);
    if (status) {
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_proximity.c:\n"
                        "function: cpx_solve_proximity:\n"
                        "cpx_recenter_obj: %d\n", status);
      return 1;
    }

    status = cpx_update_cutoff(env, lp, ce, x_feas, numcols, theta, cutoff_idx);
    if (status) {
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_proximity.c:\n"
                        "function: cpx_solve_proximity:\n"
                        "cpx_update_cutoff: %d\n", status);
      return 1;
    }

    begin = cutoff_idx + 1;
    end = CPXgetnumrows(env, lp) - 1;
    if (begin < end) {
      status = CPXdelrows(env, lp, begin, end);
      if (status) {
        fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_proximity.c:\n"
                                "function: cpx_solve_proximity:\n"
                                "CPXdelrows: %d\n", status);
        return 1;
      }
    }

    
    cnt = 0;   
    for (idx =  0; idx < numcols; idx++) {
      vrtx_from_idx(&ce->T, &i, &j, idx);
      if (ce->init_lb + graph_get_edge_delta(&ce->G, i, j) > z_feas+1) {
          indices[cnt] = idx;
          lu[cnt]      = 'B';
          bd[cnt]      = 0.0;
          cnt++;
      }
    }
     

    status = CPXchgbds(env, lp, cnt, indices, lu, bd);
    if (status) {
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_preprocessing.c:\n"
                        "function: cpx_preprocessing:\n"
                        "CPXchgbds: %d\n", status);
      return status;
    }

    printf("# discarded fat edges = %d\n", cnt);
    

  } // end while (iter)

  return 0;
}


int cpx_recenter_obj(CPXENVptr env, CPXLPptr lp, double* x_feas, int x_feas_size) {

  if (x_feas_size != CPXgetnumcols(env, lp)) return 1;

  int i;
  int status;
  int rmatind[x_feas_size];
  double rmatval[x_feas_size];

  for (i = 0; i < x_feas_size; i++) {
    rmatind[i] = i;
    if (x_feas[i] > 0.9) rmatval[i] = -1.0;
    else                 rmatval[i] =  1.0;
  }

  status = CPXchgobj(env, lp, x_feas_size, rmatind, rmatval);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_proximity.c:\n"
                    "function: cpx_recenter_obj:\n"
                    "CPXchgobj: %d\n", status);
    return 1;
  }

  return 0;

}

int cpx_update_cutoff(CPXENVptr env, CPXLPptr lp, cpx_env* ce, double* x_feas, int x_feas_size, double theta, int cutoff_idx) {

  if (x_feas_size != CPXgetnumcols(env, lp)) return 1;


  int i, j, idx, status;
  double z_feas;

  z_feas = 0.0;
  for (idx = 0; idx < x_feas_size; idx++) {
    if (x_feas[idx] > 0.9) {
      vrtx_from_idx(&ce->T, &i, &j, idx);
      z_feas += graph_get_edge_cost(&ce->G, i, j);
    }
  }

  int rmatind[1];
  rmatind[0] = cutoff_idx;

  double rmatval[1];
  rmatval[0] = z_feas - theta;

  status = CPXchgrhs(env, lp, 1, rmatind, rmatval);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_proximity.c:\n"
                    "function: cpx_update_cutoff:\n"
                    "CPXchgrhs: %d\n", status);
    return 1;
  }

  return  0;
}
