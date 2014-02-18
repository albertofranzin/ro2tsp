#include "cpx_solve_miliotis.h"

int cpx_solve_miliotis(CPXENVptr  env, 
                       CPXLPptr   lp, 
                       cpx_env   *ce, 
                       cpx_stats *cs, 
                       double    *x, 
                       int        x_size, 
                       int       *solstat) {


  int n = ce->G.n;
  //int numcols = ce->T.numel;
  int numcols = n * (n - 1) / 2;

  int status;

  //status = CPXsetintparam (env, CPX_PARAM_MIPSEARCH, CPX_MIPSEARCH_DYNAMIC);
  status = CPXsetintparam (env, CPX_PARAM_MIPSEARCH, CPX_MIPSEARCH_TRADITIONAL);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
                    "function: cpx_solve_miliotis:\n"
                    "CPXsetinparam (CPX_PARAM_MIPSEARCH) : %d\n", status);
    return status;
  }

  //status = CPXsetintparam (env, CPX_PARAM_PRELINEAR, CPX_ON);
  status = CPXsetintparam (env, CPX_PARAM_PRELINEAR, CPX_OFF);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
                    "function: cpx_solve_miliotis:\n"
                    "CPXsetinparam (CPX_PARAM_PRELINEAR) : %d\n", status);
    return status;
  }

  //status = CPXsetintparam (env, CPX_PARAM_REDUCE, CPX_PREREDUCE_NOPRIMALORDUAL);
  status = CPXsetintparam (env, CPX_PARAM_REDUCE, CPX_PREREDUCE_PRIMALANDDUAL);
  //status = CPXsetintparam (env, CPX_PARAM_REDUCE, CPX_PREREDUCE_PRIMALONLY);
  //status = CPXsetintparam (env, CPX_PARAM_REDUCE, CPX_PREREDUCE_DUALONLY);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
                    "function: cpx_solve_miliotis:\n"
                    "CPXsetinparam (CPX_PARAM_REDUCE) : %d\n", status);
    return status;
  }

  status = CPXsetintparam (env, CPX_PARAM_MIPCBREDLP, CPX_ON);
  //status = CPXsetintparam (env, CPX_PARAM_MIPCBREDLP, CPX_OFF);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
                    "function: cpx_solve_miliotis:\n"
                    "CPXsetinparam (CPX_PARAM_MIPCBREDLP) : %d\n", status);
    return status;
  }

  /**/
  status = CPXsetintparam (env, CPX_PARAM_PREDUAL, -1);
  //status = CPXsetintparam (env, CPX_PARAM_PREDUAL, 0);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c:\n"
                    "function: cpx_set_problem_parameters:\n"
                    "CPXsetinparam (CPX_PARAM_PREDUAL) : %d\n", status);
    return status;
  }
  /**/

  /** /status = CPXsetdblparam(env, CPX_PARAM_CUTUP, ce->init_ub);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
                    "function: cpx_solve_miliotis:\n"
                    "CPXsetdblparam (CPX_PARAM_CUTUP) : %d\n", status);
    return status;
  }/*

  status = CPXsetdblparam(env, CPX_PARAM_CUTLO, ce->init_lb);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
                    "function: cpx_solve_miliotis:\n"
                    "CPXsetdblparam (CPX_PARAM_CUTLO) : %d\n", status);
    return status;
  }*/


  if (x_size != numcols) return 1;

  status = CPXsetlazyconstraintcallbackfunc(env, cpx_callback_miliotis, ce);
  //status = CPXsetincumbentcallbackfunc(env, cpx_callback_miliotis, ce);
  if (status) {
  fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
      "function: cpx_solve_miliotis:\n"
      "CPXsetlazyconstraintcallbackfunc : %d\n", status);
  return 1;
  }

  //status = CPXsetusercutcallbackfunc(env, cpx_callback_maxflow, ce);
  if (status) {
  fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
                  "function: cpx_solve_miliotis:\n"
                  "CPXsetusercutcallbackfunc : %d\n", status);
  return 1;
  }

  status = CPXpresolve(env, lp, CPX_ALG_NONE);
  if (status) {
  fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
                    "function: cpx_solve_miliotis:\n"
              "CPXpresolve : %d\n", status);
  return status;
  }


  status = CPXmipopt(env, lp);
  if (status) {
  fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
      "function: cpx_solve_miliotis:\n"
      "CPXmipopt : %d\n", status);
  return 1;
  }

  
  status = CPXfreepresolve(env, lp);
  if (status) {
  fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
                    "function: cpx_solve_miliotis:\n"
            "CPXfreepresolve : %d\n", status);
  return 1;
  }



  status = CPXgetstat(env, lp);
  *solstat = status;
  if (status == 103) return 0;


  if (CPXgetnumcols(env, lp) != numcols) return 1;


  status = CPXgetx(env, lp, x, 0, numcols-1);
  if (status) {
  fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
            "function: cpx_solve_miliotis:\n"
            "CPXgetx : %d\n", status);
  return 1;
  }



  return 0;
}


int CPXPUBLIC cpx_callback_miliotis(CPXENVptr env,
                                    void     *cbdata,
                                    int       wherefrom,
                                    void     *cbhandle,
                                    int      *useraction_p) {


  printf("# hello callback!\n");
  //getchar();

  int status = 0;
  *useraction_p = CPX_CALLBACK_DEFAULT;

  cpx_env* ce = (cpx_env*)cbhandle;
  int n = ce->G.n;
  //int numcols = ce->T.numel;
  int numcols = n * (n - 1) / 2;
  int vrtx_comp[n];
  double *x = malloc(numcols * sizeof(double));

  int numcomp, my_comp;
  int i;

  int prenumcols;
  CPXLPptr prelp;

  status = CPXgetcallbacklp(env, cbdata, wherefrom, &prelp);
  if (status) {
  fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
      "function: cpx_callback_miliotis:\n"
      "CPXgetcallbacklp : %d\n", status);
  return 1;
  }

  prenumcols = CPXgetnumcols(env, prelp);

  double *prex = malloc(prenumcols*sizeof(double));
  status = CPXgetcallbacknodex(env, cbdata, wherefrom, prex, 0, prenumcols-1);
  if (status) {
  fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
      "function: cpx_callback_miliotis:\n"
      "CPXgetcallbacknodex : %d\n", status);
  return 1;
  }

  status = CPXuncrushx(env, ce->mylp, x, prex);
  if (status) {
  fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
      "function: cpx_callback_miliotis:\n"
      "CPXgetuncrushx : %d\n", status);
  return 1;
  }

  status = cpx_mark_components(ce, x, numcols, vrtx_comp, n, &numcomp);
  if (status) {
  fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
      "function: cpx_callback_miliotis:\n"
      "cpx_mark_components : %d\n", status);
  return 1;
  }

  cpx_constraint sec;
  cpx_constraint_init(&sec, numcols);
  cpx_constraint presec;
  cpx_constraint_init(&presec, prenumcols);

  int prenzcnt;
  double preoffset;

  if (numcomp > 1 && wherefrom == CPX_CALLBACK_MIP_CUT_FEAS) {

    //int count = (numcomp < 3) ? numcomp : 3;

    printf("# SECs added: %d\n", numcomp);
    //printf("# SECs added: %d\n", count);
    //printf("%d\n", rand());

    for (my_comp = 0; my_comp < numcomp; my_comp++) {
    //for (my_comp = 0; my_comp < count; my_comp++) {


      status = cpx_constraint_generate_sec(ce, vrtx_comp, n, my_comp, &sec);
      if (status) {
        fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
            "function: cpx_callback_miliotis:\n"
            "cpx_constraint_generate_sec : %d\n", status);
        return 1;
      }


      CPXcrushform(env, ce->mylp, sec.nzcnt, sec.rmatind, sec.rmatval, &prenzcnt, &preoffset, presec.rmatind, presec.rmatval);

      presec.nzcnt = prenzcnt;
      presec.rhs = sec.rhs -  preoffset;
      //presec.rowname = sec.rowname;
      presec.rmatbeg = sec.rmatbeg;
      presec.sense = sec.sense;

      status = CPXcutcallbackadd(env, cbdata, wherefrom, presec.nzcnt, presec.rhs, presec.sense, presec.rmatind, presec.rmatval, CPX_USECUT_PURGE);
      if (status) {
              fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
            "function: cpx_callback_miliotis:\n"
            "CPXcutcallbackadd : %d\n", status);
        return 1;
      }

      
    }
  }



  cpx_constraint_delete(&sec);
  cpx_constraint_delete(&presec);
  free(x);
  free(prex);

  if (numcomp > 1) {
    *useraction_p = CPX_CALLBACK_SET;
  }

  return 0;

}


int CPXPUBLIC cpx_callback_maxflow(CPXENVptr  env,
                                    void     *cbdata,
                                    int       wherefrom,
                                    void     *cbhandle,
                                    int      *useraction_p) {

  int status = 0, i, j, k, idx;
  *useraction_p = CPX_CALLBACK_DEFAULT;

  if (rand() % 100 > 5) {
    *useraction_p = CPX_CALLBACK_SET;
    return 0;
  }

  cpx_env* ce = (cpx_env*)cbhandle;
  int n = ce->G.n;
  int numcols = n * (n - 1) / 2;

  double *x = calloc(numcols, sizeof(double));

  int prenumcols;
  CPXLPptr prelp;

  status = CPXgetcallbacklp(env, cbdata, wherefrom, &prelp);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
                    "function: cpx_callback_maxflow:\n"
                    "CPXgetcallbacklp : %d\n", status);
    return 1;
  }

  prenumcols = CPXgetnumcols(env, prelp);

  double *prex = malloc(prenumcols*sizeof(double));
  status = CPXgetcallbacknodex(env, cbdata, wherefrom, prex, 0, prenumcols-1);
  if (status) {
  fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
                  "function: cpx_callback_maxflow:\n"
                  "CPXgetcallbacknodex : %d\n", status);
  return 1;
  }

  status = CPXuncrushx(env, ce->mylp, x, prex);
  if (status) {
  fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
                  "function: cpx_callback_maxflow:\n"
                  "CPXgetuncrushx : %d\n", status);
  return 1;
  }

  int *cut = NULL;
  int cutcount = -1;
  double minval;
  int  ncomp;
  int *compscount,
      *comps;

  status = cpx_maxflow_constraints(ce, x, &comps, &compscount, &ncomp, &cut, &cutcount, &minval);

  printf("%d\n", cutcount);
  //getchar();

  if (ncomp == 1 && cutcount > 0) {
    printf("in: if cutcount > 0\n");
    for (i = 0; i < cutcount; i++) {
      printf("%d ", cut[i]);
    }
    printf("\n");


    int nflows = cutcount * (n - cutcount);
    flow *flist = malloc(nflows * sizeof(flow));
    /*for (i = 0 ; i < nflows ; i++) {
      flist[i] = malloc(sizeof(flist));
    }*/


    int    *matind = malloc(cutcount*(n-cutcount)*sizeof(int));
    double *matval = malloc(cutcount*(n-cutcount)*sizeof(double));
    int    *mark   = calloc(n, sizeof(int));
    
    for (i = 0 ; i < cutcount ; i++) mark[cut[i]] = 1;

    k = 0;
    for (i = 0 ; i < cutcount ; i++) {
      for (j = 0 ; j < n ; j++) {
        if (mark[j] == 0) {
          idx_from_vrtx(&ce->T, cut[i], j, &idx);
          flist[k].index    = k;
          flist[k].edge     = idx;
          flist[k].capacity = x[idx];
          // printf("%d | %d %d | %d %f\n", k, cut[i], j, idx, x[idx]);
          k++;
        }
      }
    }

    /*printf ("%d =?= %d\n", k, nflows);
    getchar();*/

    qsort(flist, nflows, sizeof(flow), comp_flows);

    for (i = 0 ; i < nflows ; i++) {
      // printf("%d %f\n", flist[i].edge, fabs(flist[i].capacity));
      matind[i] = flist[i].edge;
      matval[i] = 1.0;//fabs(flist[i].capacity);
    }

    double *pval = calloc(numcols, sizeof(double));
    int    *pind = calloc(numcols, sizeof(int));
    double  offset_p;
    int     plen_p;

    status = CPXcrushform(env, ce->mylp, nflows, matind, matval, &plen_p, &offset_p, pind, pval);
    if (status) {
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
                      "function: cpx_callback_maxflow:\n"
                      "CPXcrushform : %d\n", status);
      return 1;
    }

    char   sen = 'G';
    double rhs = 2.0;
    status = CPXcutcallbackadd(env, cbdata, wherefrom, nflows, rhs, sen, pind, pval, CPX_USECUT_PURGE);
    if (status) {
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
                      "function: cpx_callback_maxflow:\n"
                      "CPXcutcallbackadd : %d\n", status);
      return 1;
    }

    free(mark);
    free(matind);
    free(matval);
    free(flist);
    free(pval);
    free(pind);
    if (*cut != NULL) free(cut);
  } else if (ncomp > 1) {
    // add SEC constraints to break subcycles
    // first element of first connected component returned is always 0
    // or, at least, I hope so
    int current_node = compscount[0], curr_cc;

    int    *matind = malloc(compscount[0]*(n - compscount[0])*sizeof(int));
    double *matval = malloc(compscount[0]*(n - compscount[0])*sizeof(double));
    double *pval = calloc(numcols, sizeof(double));
    int    *pind = calloc(numcols, sizeof(int));
    double  offset_p;
    int     plen_p;

    k = 0;
    for (i = 0 ; i < compscount[0] ; i++) {
      for (j = compscount[0] ; j < n ; j++) {
        idx_from_vrtx(&ce->T, comps[i], comps[j], &idx);
          //printf("%d %d %d\n", comps[i], comps[j], idx);
          matind[k] = idx;
          matval[k] = 1.0;
          k++;
      }
    }

    qsort(matind, k, sizeof(int), comp_int);

    //getchar();
    status = CPXcrushform(env, ce->mylp, k, matind, matval, &plen_p, &offset_p, pind, pval);
    if (status) {
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
                      "function: cpx_callback_maxflow:\n"
                      "CPXcrushform : %d\n", status);
      return 1;
    }

    char   sen = 'G';
    double rhs = 2.0;
    status = CPXcutcallbackadd(env, cbdata, wherefrom, k, rhs, sen, pind, pval, CPX_USECUT_PURGE);
    if (status) {
      fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
                      "function: cpx_callback_maxflow:\n"
                      "CPXcutcallbackadd : %d\n", status);
      return 1;
    }

    free(matind);
    free(matval);
    free(pval);
    free(pind);

    /*for (current_node = 0 ; current_node < n ; current_node++)
      printf(" %d", comps[current_node]);
    printf("\n");*/
    //getchar();
  }


  free(x);
  free(prex);

  *useraction_p = CPX_CALLBACK_SET;
  return 0;
}


int comp_flows(const void *f1, const void *f2) {
  flow *ff1 = (flow *)f1;
  flow *ff2 = (flow *)f2;
  //printf("%d %d %f | %d %d %f\n", ff1.index, ff1.edge, ff1.capacity, ff2.index, ff2.edge, ff2.capacity);
  return (ff1->edge - ff2->edge);
}
