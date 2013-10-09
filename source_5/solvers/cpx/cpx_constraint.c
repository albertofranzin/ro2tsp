#include "cpx_constraint.h"


void cpx_constraint_init(cpx_constraint *c, 
                         int             max_nzcnt) {

#ifdef DEBUG
  if (max_nzcnt < 1) {
    printf("error: cpx_constraint_init\n");
    exit(1);
  }
#endif

  c->max_nzcnt  = max_nzcnt;
  c->rmatind    = (int*)calloc(max_nzcnt, sizeof(int));
  c->rmatval    = (double*)calloc(max_nzcnt, sizeof(double));
  c->rowname    = (char*)calloc(100, sizeof(char));
  c->nzcnt      = 0;
  c->rmatbeg    = 0;
  c->rhs        = 0.0;
  c->sense      = 0;

}

void cpx_constraint_delete(cpx_constraint *c) {

  free(c->rmatind);
  free(c->rmatval);
  free(c->rowname);

  c->rmatind   = NULL;
  c->rmatval   = NULL;
  c->rowname   = NULL;
  c->nzcnt     = 0;
  c->rmatbeg   = 0;
  c->rhs       = 0.0;
  c->sense     = 0;
  c->max_nzcnt = 0;

}


int cpx_constraint_add(CPXENVptr       env, 
                       CPXLPptr        lp, 
                       cpx_constraint *c) {

  int status;

  double vrhs[1];
  char   vsense[1];
  int    vrmatbeg[1];
  char  *vrowname[1];

  vrhs[0]     = c->rhs;
  vsense[0]   = c->sense;
  vrmatbeg[0] = c->rmatbeg;
  vrowname[0] = c->rowname;

  status = CPXaddrows(env, lp, 0, 1, c->nzcnt, vrhs, vsense,
                      vrmatbeg, c->rmatind, c->rmatval, NULL, vrowname);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_constraint.c:\n"
                    "function: cpx_constraint_add:\n"
                    "CPXaddrows : %d\n", status);
    return status;
  }

  return status;
}


int cpx_constraint_generate_sec(cpx_env        *ce,
                                int            *vrtx_comp,
                                int             vrtx_comp_size,
                                int             my_comp,
                                cpx_constraint *c) {

  int n       = ce->G.n;
  int numcols = n * (n - 1) / 2;
  int i, j, idx, numvrtx, numedg;

  if (vrtx_comp_size != n) return 1;
  if (c->max_nzcnt < n * (n - 1) / 2) return 1;
  /*
    could be replaced with
    assert(vrtx_comp_size == n);
    assert(c->max_nzcnt >= n * (n - 1) / 2);
   */


  sprintf(c->rowname, "sec_cstr");
  #ifdef DEBUG
  if (ce->pars->verbosity >= ANNOYING) {
    printf("Adding SEC: %s\n", c->rowname);
  }
  #endif

  numedg = 0;
  for (idx = 0; idx < numcols; idx++) {
    if (vrtx_comp[ ce->T.entries[idx].x ] == my_comp &&
        vrtx_comp[ ce->T.entries[idx].y ] == my_comp   ) {
          (c->rmatind)[numedg] = idx;
          (c->rmatval)[numedg] = 1.0;
          numedg++;
          #ifdef DEBUG
          if (ce->pars->verbosity >= ANNOYING) {
            printf("%d ", idx);
          }
          #endif
    }
  }
  #ifdef DEBUG
  if (ce->pars->verbosity >= ANNOYING) {
    printf("\n");
  }
  #endif

  /*
  for (i = 0; i < n; i++) {
  for (j = i+1; j < n; j++) {

    if (vrtx_comp[i] == my_comp && vrtx_comp[j] == my_comp) {

      idx_from_vrtx(&ce->T, i, j, &idx);
      (c->rmatind)[numedg] = idx;
      (c->rmatval)[numedg] = 1.0;
      numedg++;
    
    }
  }
  }
  */

  numvrtx = 0;
  for (i = 0; i < n; i++) {
    if (vrtx_comp[i] == my_comp) {
      numvrtx++;
    }
  }
  

  c->nzcnt   = numedg;
  c->rmatbeg = 0;
  c->rhs     = numvrtx-1;
  c->sense   = 'L';       
         
  return 0;
}



int cpx_constraint_generate_proximity_cutoff(cpx_env        *ce,
                                             double         *x_feas,
                                             int             x_feas_size,
                                             double          theta,
                                             cpx_constraint *c) {

  int    n       = ce->G.n;
  int    numcols = n * (n - 1) / 2;
  int    i, j, idx;
  double z_feas;

  if (x_feas_size != numcols || c->max_nzcnt < numcols) return 1;
  /**
   * could be replaced with
   * assert(x_feas_size == numcols && c->max_nzcnt >= numcols);
   */

  sprintf(c->rowname, "cutoff_cstr");

  z_feas = 0.0;
  for (idx = 0; idx < x_feas_size; idx++) {
    if (x_feas[idx] > 0.9) {
      vrtx_from_idx(&ce->T, &i, &j, idx);
      z_feas += graph_get_edge_cost(&ce->G, i, j);
    }
  } 

  for (idx = 0; idx < numcols; idx++) {
    vrtx_from_idx(&ce->T, &i, &j, idx);
    (c->rmatind)[idx] = idx;
    (c->rmatval)[idx] = graph_get_edge_cost(&ce->G, i, j);

  }

  c->nzcnt   = numcols;
  c->rmatbeg = 0;
  c->rhs     = z_feas - theta;
  c->sense   = 'L';       

  return 0;

}
