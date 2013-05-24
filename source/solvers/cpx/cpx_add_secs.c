#include "cpx_add_secs.h"

int cpx_add_secs(CPXENVptr env,
                 CPXLPptr  lp,
                 cpx_table *hash_table,
		 graph     *G)
{

  int n = (*G).n;
  int i, j, numcols, numedgs, status;

  numcols = CPXgetnumcols(env, lp);

  double sol[numcols];

  status = CPXgetx(env, lp, sol, 0, numcols-1);

  int edg_inds[n];

  numedgs = 0;
  for (i = 0; i < numcols; i++) {

    if (sol[i] > 0.9)  edg_inds[numedgs++] = i+1;

  }

  // if numedgs != n : error

  int ind, x, y;

  int vrtx_neighbour1[n]; for (i = 0; i < n; i++) vrtx_neighbour1[i] = 0;
  int vrtx_neighbour2[n]; for (i = 0; i < n; i++) vrtx_neighbour2[i] = 0;

  for (i = 0; i < numedgs; i++) {

    ind = edg_inds[i];

    vertices_from_indx(hash_table, &x, &y, ind);

    if (vrtx_neighbour1[x-1] == 0) vrtx_neighbour1[x-1] = y;
    else                           vrtx_neighbour2[x-1] = y;

    if (vrtx_neighbour1[y-1] == 0) vrtx_neighbour1[y-1] = x;
    else                           vrtx_neighbour2[y-1] = x;

  }

  int vrtx_start, vrtx_curr, my_mark;

  int vrtx_mrks[n]; for (i = 0; i < n; i++) vrtx_mrks[i] = 0;
  vrtx_start = 1; my_mark = 1;

  int termination = FALSE;

  while (!termination) {

    vrtx_curr = vrtx_neighbour1[vrtx_start-1];
    vrtx_mrks[vrtx_curr-1] = my_mark;

    vrtx_curr = (vrtx_neighbour1[vrtx_curr-1] != vrtx_start) ? vrtx_neighbour1[vrtx_curr-1] : vrtx_neighbour2[vrtx_curr-1];
    vrtx_mrks[vrtx_curr-1] = my_mark;

    while (vrtx_curr != vrtx_start) {

      vrtx_curr = (vrtx_mrks[vrtx_neighbour1[vrtx_curr-1]-1] == 0) ? vrtx_neighbour1[vrtx_curr-1] : vrtx_neighbour2[vrtx_curr-1];
      vrtx_mrks[vrtx_curr-1] = my_mark;

    }

    vrtx_start = 0;
    for (i = 0; i < n; i++) {
      if (vrtx_mrks[i] == 0) {
	vrtx_start = i+1;
	break;
      }
    }

    if (vrtx_start == 0) termination = TRUE;
    else                 my_mark++;

  }

  int numsubtrs = my_mark;


  if (numsubtrs > 1) {

    for (my_mark = 1; my_mark <= numsubtrs; my_mark++) {

      cpx_add_my_sec(env, lp, hash_table, vrtx_mrks, n, my_mark);

    }

    printf("# SECs added = %d\n", numsubtrs);

  }

  return numsubtrs;
}

