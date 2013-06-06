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


typedef struct _cpx_env {

  graph		G_INPUT;

  egraph	EG_INPUT;

  graph		G_OUTPUT;


  graph 	G;

  onetree	OT;

  cpx_table	T;


  cycle		TOUR_HEUR;

  graph		TOUR_OPT;

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
