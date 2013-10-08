#ifndef CPX_ENV_H_
#define CPX_ENV_H_


#include <stdlib.h>
#include <ilcplex/cplex.h>

#include "../../data/graph.h"
#include "../../data/egraph.h"
#include "../../data/onetree.h"
#include "../../data/cycle.h"
#include "cpx_table.h"


// ---------------------------------------------------------- //
//                                                            //
//                       data structures                      //
//                                                            //
// ---------------------------------------------------------- //

/**
 * cpx_env
 * environment of the problem solved using CPLEX:
 * contains the graph (in all of its formats) and the tour(s, same) found
 * as solution, plus the cost of the solution, and the upper/lower bounds
 *
 * G_INPUT   : input (complete) graph
 * EG_INPUT  : input egraph, containing the coordinates
 * G_OUTPUT  : output graph
 * G         : incumbent graph
 * OT        ; incumbent 1-tree
 * T         : table of coordinates in cpx_table format
 * TOUR_HEUR : tour as computed by the heuristic
 * TOUR_OPT  : graph containing the optimal tour
 * z-opt     : cost of the optimal solution
 * init_ub   : initial upper bound on the solution
 * init_lb   : initial lower bound on the solution
 * mylp      : pointer to CPLEX LP
 */
typedef struct _cpx_env {
  graph		  G_INPUT;
  egraph	  EG_INPUT;
  graph		  G_OUTPUT;
  graph 	  G;
  onetree	  OT;
  cpx_table	T;
  cycle		  TOUR_HEUR;
  graph		  TOUR_OPT;

  double	z_opt;
  double	init_ub;
  double	init_lb;
  CPXLPptr mylp;
} cpx_env;


// ---------------------------------------------------------- //
//                                                            //
//                          functions                         //
//                                                            //
// ---------------------------------------------------------- //


/**
 * initialize environment
 * @param ce	cpx environment
 */
void cpx_env_init(cpx_env* ce);


/**
 * delete environment
 * @param ce	cpx environment
 */
void cpx_env_delete(cpx_env* ce);


#endif
