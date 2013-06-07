#include "cpx_solve_miliotis.h"

int cpx_solve_miliotis(CPXENVptr	env, 
		       CPXLPptr	lp, 
		       cpx_env		*ce, 
		       cpx_stats	*cs, 
		       double*		x, 
		       int		x_size, 
		       int*		solstat) {


  int n = ce->G.n;
  //int numcols = ce->T.numel;
  int numcols = n * (n - 1) / 2;

  int status;

  //status = CPXsetintparam (env, CPX_PARAM_MIPSEARCH, CPX_MIPSEARCH_DYNAMIC);
  status = CPXsetintparam (env, CPX_PARAM_MIPSEARCH, CPX_MIPSEARCH_TRADITIONAL);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c:\n"
                    "function: cpx_set_problem_parameters:\n"
                    "CPXsetinparam (CPX_PARAM_MIPSEARCH) : %d\n", status);
    return status;
  }

  //status = CPXsetintparam (env, CPX_PARAM_PRELINEAR, CPX_ON);
  status = CPXsetintparam (env, CPX_PARAM_PRELINEAR, CPX_OFF);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c:\n"
                    "function: cpx_set_problem_parameters:\n"
                    "CPXsetinparam (CPX_PARAM_PRELINEAR) : %d\n", status);
    return status;
  }

  //status = CPXsetintparam (env, CPX_PARAM_REDUCE, CPX_PREREDUCE_NOPRIMALORDUAL);
  //status = CPXsetintparam (env, CPX_PARAM_REDUCE, CPX_PREREDUCE_PRIMALANDDUAL);
  status = CPXsetintparam (env, CPX_PARAM_REDUCE, CPX_PREREDUCE_PRIMALONLY);
  //status = CPXsetintparam (env, CPX_PARAM_REDUCE, CPX_PREREDUCE_DUALONLY);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c:\n"
                    "function: cpx_set_problem_parameters:\n"
                    "CPXsetinparam (CPX_PARAM_REDUCE) : %d\n", status);
    return status;
  }

  status = CPXsetintparam (env, CPX_PARAM_MIPCBREDLP, CPX_ON);
  //status = CPXsetintparam (env, CPX_PARAM_MIPCBREDLP, CPX_OFF);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c:\n"
                    "function: cpx_set_problem_parameters:\n"
                    "CPXsetinparam (CPX_PARAM_MIPCBREDLP) : %d\n", status);
    return status;
  }

  /*
  status = CPXsetintparam (env, CPX_PARAM_PREDUAL, -1);
  //status = CPXsetintparam (env, CPX_PARAM_PREDUAL, 0);
  if (status) {
    fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c:\n"
                    "function: cpx_set_problem_parameters:\n"
                    "CPXsetinparam (CPX_PARAM_PREDUAL) : %d\n", status);
    return status;
  }
  */





  if (x_size != numcols) return 1;

  status = CPXsetlazyconstraintcallbackfunc(env, cpx_callback_miliotis, ce);
  //status = CPXsetincumbentcallbackfunc(env, cpx_callback_miliotis, ce);
  if (status) {
	fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
			"function: cpx_solve_miliotis:\n"
			"CPXsetlazyconstraintcallbackfunc : %d\n", status);
	return 1;
  }

  status = CPXpresolve(env, lp, CPX_ALG_NONE);
  if (status) {
	fprintf(stderr, "Fatal error in solvers/cpx/cpx_setup_problem.c:\n"
             		"function: cpx_set_problem_parameters:\n"
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
	fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_iterative.c:\n"
		        "function: cpx_solve_iteraitve:\n"
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


int CPXPUBLIC cpx_callback_miliotis(CPXENVptr	env,
				    void	*cbdata,
				    int 	wherefrom,
				    void	*cbhandle,
				    int		*useraction_p) {


  printf("# hello callback!\n");
  //getchar();

  int status = 0;
  *useraction_p = CPX_CALLBACK_DEFAULT;

  cpx_env* ce = (cpx_env*)cbhandle;
  int n = ce->G.n;
  //int numcols = ce->T.numel;
  int numcols = n * (n - 1) / 2;
  int vrtx_comp[n];
  double x[numcols];

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

  double prex[prenumcols];
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

  if (numcomp > 1) {
    *useraction_p = CPX_CALLBACK_SET;
  }

  return 0;

  /*
  printf("# hello callback!\n");
  //getchar();

  int status = 0;
  *useraction_p = CPX_CALLBACK_DEFAULT;

  cpx_env* ce = (cpx_env*)cbhandle;
  int n = ce->G.n;
  //int numcols = ce->T.numel;
  int numcols = n * (n - 1) / 2;
  int vrtx_comp[n];
  double x[numcols];
  cpx_constraint sec;
  cpx_constraint_init(&sec, numcols);

  int numcomp, my_comp;


  status = CPXgetcallbacknodex(env, cbdata, wherefrom, x, 0, numcols-1);
  if (status) {
	fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
			"function: cpx_callback_miliotis:\n"
			"CPXgetcallbacknodex : %d\n", status);
	return 1;
  }

  status = cpx_mark_components(ce, x, numcols, vrtx_comp, n, &numcomp);
  if (status) {
	fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
			"function: cpx_callback_miliotis:\n"
			"cpx_mark_components : %d\n", status);
	return 1;
  }


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


		//status = CPXcutcallbackadd(env, cbdata, wherefrom, sec.nzcnt, sec.rhs, sec.sense, sec.rmatind, sec.rmatval, CPX_USECUT_FORCE);
		status = CPXcutcallbackadd(env, cbdata, wherefrom, sec.nzcnt, sec.rhs, sec.sense, sec.rmatind, sec.rmatval, CPX_USECUT_PURGE);
		if (status) {
	       		fprintf(stderr, "Fatal error in solvers/cpx/cpx_solve_miliotis.c:\n"
					"function: cpx_callback_miliotis:\n"
					"CPXcutcallbackadd : %d\n", status);
			return 1;
		}

    
	}
  }


  cpx_constraint_delete(&sec);
  if (numcomp > 1) {
    *useraction_p = CPX_CALLBACK_SET;
  }

  return 0;
  */

}
