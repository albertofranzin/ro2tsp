#ifndef CPX_SOLVE_HARDFIXING_H_
#define CPX_SOLVE_HARDFIXING_H_


#include <stdlib.h>
#include <stdio.h>
#include <ilcplex/cplex.h>
#include "../../base/global.h"
#include "../../base/utils.h"

#include "../../data/environment.h"
#include "../../data/statistics.h"
#include "../../data/parameters.h"

#include "../../solvers/cpx/cpx_cstr.h"
#include "../../solvers/cpx/cpx_create_problem.h"
#include "../../solvers/cpx/cpx_setup_problem.h"
#include "../../solvers/cpx/cpx_set_parameters.h"
#include "../../solvers/cpx/cpx_preprocessing.h"

#include "../../solvers/cpx/cpx_solve_iterative.h"
#include "../../solvers/cpx/cpx_solve_miliotis.h"

int cpx_solve_hardfixing(CPXENVptr    cplexenv,
                         CPXLPptr     lp,
                         environment *env,
                         parameters  *pars,
                         statistics	 *stats,
                         int         *zeros,
                         int         *ones,
                         double      *ret_x,
                         int          x_size,
                         int         *ret_solstat);



#endif /* CPX_SOLVE_HARDFIXING_H_ */
