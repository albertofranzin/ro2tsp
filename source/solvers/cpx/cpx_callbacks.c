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
  double  *x        = ci->x;
  int     *beg      = ci->beg;
  int     *ind      = ci->ind;
  double  *val      = ci->val;
  double  *rhs      = ci->rhs;
  int      n        = ci->number_of_nodes;
  double   objval;
  int      i, k;

  /**/
  *useraction_p = CPX_CALLBACK_DEFAULT;

  status = CPXgetcallbacknodex(env, cbdata, wherefrom, x, 0, numcols-1);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_callbacks :: \n");
    fprintf(stderr, "CPXgetcallbacknodex : %d\n", status);
    fprintf(stderr, "Failed to get node solution.\n");
    exit(1);
  }

  int num_of_subtours = 0;

  //numcols = CPXgetnumcols(env, lp);
  //status  = CPXgetobjval(env, lp, &objval);
  status = CPXgetcallbacknodeobjval(env, cbdata, wherefrom, &objval);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_callbacks :: \n");
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
    if (x[i] > 0.9) {
      // oppure x[i] == 1.0 dopo aver arrotondato le x[i] all'intero più vicino
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

  for (i = 0; i < n; ++i) {
    int x, y;
    vertices_from_pos(&hash_table, &x, &y, edge_indices[i]);

#ifdef DEBUG
  if (pars->verbosity >= USEFUL) {
    printf("%d: %d (%d %d)\n", i+1, edge_indices[i], x, y);
  }
#endif

  }

#ifdef DEBUG
  if (pars->verbosity >= USEFUL) {
    printf("\n");
    printf("looking for subcycles\n");
  }
#endif

  // RICERCA SOTTOCICLI
  num_of_subtours = cpx_mark_subtours(&hash_table, edge_indices,
                                      edge_marks, n, pars);

#ifdef DEBUG
  if (pars->verbosity >= ESSENTIAL) {
    printf("# of subtours found : %d\nmarks:\n", num_of_subtours);
  }
  if (pars->verbosity >= USEFUL) {
    for (i = 0; i < n; ++i) {
      printf("%d ", edge_marks[i]);
    }
    printf("\n--\n");
  }
#endif

  // if the current solution has subcycles, add relative constraints
  if (num_of_subtours > 1) {
    //  AGGIUNTA NUOVI VINCOLI SEC
    int my_mark;
    for (my_mark = 1; my_mark <= num_of_subtours; my_mark++) {

      status = cpx_add_sec(env, lp, cbdata, wherefrom,
                           edge_indices, edge_marks, n, my_mark, pars);
      assert(status == 0);

    }

#ifdef DEBUG
    if (pars->verbosity >= USEFUL) {
      printf("# number of new SECs added = %d\n", num_of_subtours);/**/
    }
#endif

    // Tell CPLEX that cuts have been created
    *useraction_p = CPX_CALLBACK_SET;

  } else {
    //printf("done!\n");
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