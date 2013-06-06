#ifndef CPX_STATS_H_
#define CPX_STATS_H_


#include <stdio.h>


// ---------------------------------------------------------- //
//                                                            //
//                       data structures                      //
//                                                            //
// ---------------------------------------------------------- //


typedef struct _cpx_stats {

  double rc2opt_ub;

  double nn2opt_ub;


  double init_ub;

  double init_lb;

  double z_opt;


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
