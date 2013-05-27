#include "cpx_callbacks.h"




int CPXPUBLIC cpx_subtour_callback(CPXCENVptr  env,
                                   void       *cbdata,
                                   int         wherefrom,
                                   void       *cbhandle,
                                   int        *useraction_p)
{

  int status = 0;

  cutinfo *ci = (cutinfo *) cbhandle;

  cpx_table hash_table = ci->hash_table;
  CPXLPptr  lp         = ci->lp;

  parameters *pars     = ci->pars;

  int      numcols  = ci->numcols;
  int      numcuts  = ci->num;
  double  *solx     = ci->x;
  int     *beg      = ci->beg;
  int     *ind      = ci->ind;
  double  *val      = ci->val;
  double  *rhs      = ci->rhs;
  int      n        = ci->number_of_nodes;
  double   objval;
  int      i, k;

  /**/
  *useraction_p = CPX_CALLBACK_DEFAULT;

  status = CPXgetcallbacknodex(env, cbdata, wherefrom, solx, 0, numcols-1);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_callbacks :: ");
    fprintf(stderr, "CPXgetcallbacknodex : %d\n", status);
    fprintf(stderr, "Failed to get node solution.\n");
    exit(1);
  }

  int num_of_subtours = 0;

  //numcols = CPXgetnumcols(env, lp);
  //status  = CPXgetobjval(env, lp, &objval);
  status = CPXgetcallbacknodeobjval(env, cbdata, wherefrom, &objval);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_callbacks :: ");
    fprintf(stderr, "CPXgetcallbacknodeobjval : %d\n", status);
    printf("Failed to obtain objval.\n");
    exit(1);
  }


  int edge_marks[n],
      edge_indices[n];

  memset(edge_marks, 0, sizeof(edge_marks));
  memset(edge_indices, 0, sizeof(edge_indices));

  k = 0;
  for (i = 0; i < numcols; i++) {
    if (solx[i] > 0.9) {
      // oppure solx[i] == 1.0 dopo aver arrotondato le x[i]
      // all'intero più vicino
      edge_indices[k] = i+1;
      k++;

#ifdef DEBUG
      if (pars->verbosity >= USEFUL) {
        printf("%d ", k);
      }
#endif

    }
  }

#ifdef DEBUG
  if (pars->verbosity >= USEFUL) {
    printf("\n");
  }
#endif

  //numcols = CPXgetnumcols(env, lp);
  //printf("numcols = %d\n", numcols);

  int vrtx_neighbour1[n];
  int vrtx_neighbour2[n];
  memset(vrtx_neighbour1, 0, sizeof(vrtx_neighbour1));
  memset(vrtx_neighbour2, 0, sizeof(vrtx_neighbour2));

  for (i = 0; i < n; ++i) {
    int x, y;
    vertices_from_indx(&hash_table, &x, &y, edge_indices[i]);

#ifdef DEBUG
  if (pars->verbosity >= USEFUL) {
    printf("%d: %d (%d %d)\n", i+1, edge_indices[i], x, y);
  }
#endif

    if (vrtx_neighbour1[x-1] == 0) vrtx_neighbour1[x-1] = y;
    else                           vrtx_neighbour2[x-1] = y;

    if (vrtx_neighbour1[y-1] == 0) vrtx_neighbour1[y-1] = x;
    else                           vrtx_neighbour2[y-1] = x;

  }

#ifdef DEBUG
  if (pars->verbosity >= USEFUL) {
    printf("\n");
    printf("looking for subcycles\n");
  }
#endif

  //num_of_subtours = cpx_add_secs(env, lp, &hash_table, &G, pars);


  int vrtx_start, vrtx_curr, my_mark;

  int vrtx_mrks[n];
  memset(vrtx_mrks, 0, sizeof(vrtx_mrks));
  vrtx_start = 1; my_mark = 1;

  int termination = FALSE;

  while (!termination) {

    vrtx_curr = vrtx_neighbour1[vrtx_start-1];
    vrtx_mrks[vrtx_curr-1] = my_mark;

    vrtx_curr = (vrtx_neighbour1[vrtx_curr-1] != vrtx_start) ?
                  vrtx_neighbour1[vrtx_curr-1] :
                  vrtx_neighbour2[vrtx_curr-1];
    vrtx_mrks[vrtx_curr-1] = my_mark;

    while (vrtx_curr != vrtx_start) {
      vrtx_curr = (vrtx_mrks[vrtx_neighbour1[vrtx_curr-1]-1] == 0) ?
                  vrtx_neighbour1[vrtx_curr-1] :
                  vrtx_neighbour2[vrtx_curr-1];
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


  num_of_subtours = my_mark;
  int size_of_subtours[num_of_subtours+1];
  memset(size_of_subtours, 0, sizeof(size_of_subtours));

  for (i = 0; i < n; ++i) {
    size_of_subtours[vrtx_mrks[i]]++;
  }

#ifdef DEBUG
  if (pars->verbosity >= ESSENTIAL) {
    printf("# of subtours found : %d\n", num_of_subtours);
  }
  if (pars->verbosity >= USEFUL) {
    printf("marks: ");
    for (i = 0; i < n; ++i) {
      printf("%d ", vrtx_mrks[i]);
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

      #ifdef DEBUG
        if (pars->verbosity >= USEFUL) {
          printf("\nTour has:\n");
        }
      #endif

        int subtour_length = size_of_subtours[my_mark];  // 0;
        /**/for (i = 0; i < n; i++) {
          if (vrtx_mrks[i] == my_mark) {
            //subtour_length++;

      #ifdef DEBUG
            if (pars->verbosity >= USEFUL) {
              printf("%d ", i);
            }
      #endif

          }
        }

      #ifdef DEBUG
        if (pars->verbosity >= USEFUL) {
          printf("\n");
        }
      #endif
        /**/

        int rmatind[subtour_length];
        k = 0;
        for (i = 0; i < n; i++) {
          // aggiungo al sec solo i lati con mark uguale a my_mark
          // (cioè i lati appartenenti al subtour associato a my_mark)
          if (vrtx_mrks[i] == my_mark) {
            rmatind[k] = edge_indices[i]-1;
            // indici dei lati partono da 0 dentro cplex
            k++;
          }
        }


        double rmatval[subtour_length];
        for (i = 0; i < subtour_length; i++) {
          rmatval[i] = 1.0;
        }

        double rhs[1];
        rhs[0] = subtour_length-1;

        char sense[1];
        sense[0] = 'L';

      #ifdef DEBUG
        if (pars->verbosity >= USEFUL) {
          printf("Adding constraint...\n");
          printf("subtour_length: %d\n", subtour_length);
          printf("rhs[0]: %f\n", rhs[0]);
        }
      #endif

        status = CPXcutcallbackadd(env, cbdata, wherefrom, subtour_length,
                                   rhs[0], sense[0], rmatind, rmatval, 1);
        // Last 1 is purgeable value. See CPXcutcallbackadd documentation.
        if (status) {
          fprintf(stderr, "Fatal error in solvers/cpx/cpx_add_sec.c :: ");
          fprintf(stderr, " CPXcutcallbackadd : %d\n", status);
          fprintf(stderr, "Error while inserting a new constraint.\n");
          exit(1);
        } else {

      #ifdef DEBUG
          if (pars->verbosity >= USEFUL) {
            printf("done \n");
          }
      #endif

        }


    }

#ifdef DEBUG
    if (pars->verbosity >= USEFUL) {
      printf("# number of new SECs added = %d\n", num_of_subtours);
    }

#endif

    // Tell CPLEX that cuts have been created
    *useraction_p = CPX_CALLBACK_SET;

  } else {
    //printf("done!\n");
    // getchar();
  }

  return (status);

} // end cpx_cut_callback


/**
 * [cpx_flow_callback description]
 * @param  env          [description]
 * @param  cbdata       [description]
 * @param  wherefrom    [description]
 * @param  cbhandle     [description]
 * @param  useraction_p [description]
 * @return              [description]
 */
int CPXPUBLIC cpx_flow_callback(CPXCENVptr  env,
                                void       *cbdata,
                                int         wherefrom,
                                void       *cbhandle,
                                int        *useraction_p)
{
  int status = 0;

  return status;
}
