#include "cpx_add_secs.h"

int cpx_add_secs(CPXENVptr   env,
                 CPXLPptr    lp,
                 double     *sol,
                 int         numcols,
                 cpx_table  *hash_table,
                 parameters *pars)
{

  int n = pars->number_of_nodes;

  int i, numedgs, status;

  int edg_inds[n];

  numedgs = 0;
  for (i = 0; i < numcols; i++) {
    if (sol[i] > 0.9)  edg_inds[numedgs++] = i;
  }

  /*int rmatind[numcols];
  double coeffs[numcols];
  for (i = 0; i < numcols; ++i) {
    if (sol[i] > 0.9) {
      coeffs[i] = 0.0;
    } else if (sol[i] < 0.1) {
      coeffs[i] = 1.0;
    }
    rmatind[i] = i;
  }

  int rmatbeg[1];
  rmatbeg[0] = 0;

  double rhs[1];
  rhs[0] = 10;

  char sense[1];
  sense[0] = 'L';*/

  /*status = CPXaddrows(env, lp, 0, 1, numcols,
                      rhs, sense, rmatbeg, rmatind, coeffs,
                      NULL, NULL);
  if (status) {
    fprintf(stderr, "Fatal error in cpx_add_secs.c :: CPXaddrows :: ");
    fprintf(stderr, "failed to add constraint : error %d\n", status);
    exit(1);
  } else {
    printf("done\n");
    getchar();
  }*/

  // if numedgs != n : error

  int ind, x, y;

  int vrtx_neighbour1[n];
  int vrtx_neighbour2[n];
  for (i = 0; i < n; i++) vrtx_neighbour1[i] = vrtx_neighbour2[i] = -1;
  //memset(vrtx_neighbour1, 0, sizeof(vrtx_neighbour1));
  //memset(vrtx_neighbour2, 0, sizeof(vrtx_neighbour2));

  for (i = 0; i < numedgs; i++) {

    ind = edg_inds[i];

    vertices_from_indx(hash_table, &x, &y, ind);

    if (vrtx_neighbour1[x] == -1) vrtx_neighbour1[x] = y;
    else                          vrtx_neighbour2[x] = y;

    if (vrtx_neighbour1[y] == -1) vrtx_neighbour1[y] = x;
    else                          vrtx_neighbour2[y] = x;

  }

  int vrtx_start, vrtx_curr, my_mark;

  int vrtx_mrks[n];
  for (i = 0; i < n; i++) vrtx_mrks[i] = -1;
  vrtx_start = 0; my_mark = 0;

  int termination = FALSE;

  while (!termination) {

    vrtx_curr = vrtx_neighbour1[vrtx_start];
    vrtx_mrks[vrtx_curr] = my_mark;

    vrtx_curr = (vrtx_neighbour1[vrtx_curr] != vrtx_start) ?
                  vrtx_neighbour1[vrtx_curr] :
                  vrtx_neighbour2[vrtx_curr];

    vrtx_mrks[vrtx_curr] = my_mark;



    while (vrtx_curr != vrtx_start) {

      vrtx_curr = (vrtx_mrks[vrtx_neighbour1[vrtx_curr]] == -1) ?
                  vrtx_neighbour1[vrtx_curr] :
                  vrtx_neighbour2[vrtx_curr];

      vrtx_mrks[vrtx_curr] = my_mark;

    }



    vrtx_start = -1;
    for (i = 0; i < n; i++) {
      if (vrtx_mrks[i] == -1) {
          vrtx_start = i;
          break;
      }
    }

    if (vrtx_start == -1) termination = TRUE;
    else                           my_mark++;

  }


  int numsubtrs = my_mark + 1;

  if (numsubtrs > 1) {

    for (my_mark = 0; my_mark < numsubtrs; my_mark++) {
      status = cpx_add_my_sec(env, lp, hash_table, vrtx_mrks, n, my_mark, pars);
      assert(status == SUCCESS);
    }

    printf("# SECs added = %d\n", numsubtrs);

  }

  return numsubtrs;
}


/*
int cpx_add_cb_secs(CPXCENVptr  env,
                    CPXLPptr    lp,
                    cpx_table  *hash_table,
                    parameters *pars) {




#ifdef DEBUG
  if (pars->verbosity >= ESSENTIAL) {
    printf("# of subtours found : %d\n", num_of_subtours);
  }
  if (pars->verbosity >= USEFUL) {
    printf("marks: ");
    for (i = 0; i < n; ++i) {
      printf("%d ", edge_marks[i]);
    }
    printf("\n--\n");
  }
#endif

  // if the current solution has subcycles, add relative constraints
  if (num_of_subtours > 1) {
    // tried to add the full cycles too, but:
    // 1. it's useless: since its cost is greater then the optimum,
    //    further iterations will never find it again
    // 2. it slooows down cplex.
    //  AGGIUNTA NUOVI VINCOLI SEC
    int my_mark;
    for (my_mark = 1; my_mark <= num_of_subtours; my_mark++) {

      status = cpx_add_sec(env, lp, cbdata, wherefrom,
                           edge_indices, edge_marks, n, my_mark, pars);
      assert(status == 0);

    }

#ifdef DEBUG
    if (pars->verbosity >= USEFUL) {
      printf("# number of new SECs added = %d\n", num_of_subtours);
    }

#endif

    // Tell CPLEX that cuts have been created
    *useraction_p = CPX_CALLBACK_SET;

  }
}
*/
