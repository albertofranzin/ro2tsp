#ifndef CPX_COMPONENTS_H_
#define CPX_COMPONENTS_H_

#include <assert.h>
#include <ilcplex/cplex.h>

#include "../../base/constants.h"
#include "../../base/utils.h"
#include "../../data/graph.h"

#include "cpx_env.h"
#include "cpx_table.h"


/**
 * cplex-based tsp solver
 * @param ce			cpx environment
 * @param x			solution returned by CPLEX
 * @param x_size		length of x
 * @param vrtx_comp		vector of vertex component-labels
 * @param vrtx_comp_size	length of vrtx_comp
 * @param numcomp		number of components
 * @return			status of the function
 */
int cpx_mark_components(cpx_env *ce,
                        double  *x,
			int     x_size,
                        int     *vrtx_comp,
			int     vrtx_comp_size,
                        int     *numcomp);


#endif

