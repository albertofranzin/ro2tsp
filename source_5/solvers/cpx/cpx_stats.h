#ifndef CPX_STATS_H_
#define CPX_STATS_H_


#include <stdio.h>


// ---------------------------------------------------------- //
//                                                            //
//                       data structures                      //
//                                                            //
// ---------------------------------------------------------- //

/**
 * cpx_stats
 * contains initial bounds and final value of the solution
 *
 * rc2opt_ub : bound obtained by random cycle + 2-opt heuristic
 * nn2opt_ub : bound obtained by nearest neighbour + 2-opt heuristic
 * init_ub   : chosen initial upper bound on the cost of the solution
 * init_lb   : chosen initial lower bound on the cost of the solution
 * z_opt     : optimal cost of the solution
 */
typedef struct _cpx_stats {
  double rc2opt_ub;
  double nn2opt_ub;
  double init_ub;
  double init_lb;
  double z_opt;
  double heur_time;
  double lb_time;
  double cpx_time;
  int    cpx_iterations;
} cpx_stats;


// ---------------------------------------------------------- //
//                                                            //
//                          functions                         //
//                                                            //
// ---------------------------------------------------------- //


/**
 * initialize statistics
 * @param cs	cpx statistics
 */
void cpx_stats_init(cpx_stats* cs);


/**
 * delete statistics
 * @param cs	cpx statistics
 */
void cpx_stats_delete(cpx_stats* cs);


/**
 * print statistics
 * @param cs	cpx statistics
 */
void cpx_stats_print(cpx_stats* cs);


#endif
