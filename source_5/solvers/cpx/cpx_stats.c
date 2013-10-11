#include "cpx_stats.h"


void cpx_stats_init(cpx_stats* cs) {
  cs->rc2opt_ub	     = 0.0;
  cs->nn2opt_ub	     = 0.0;
  cs->init_ub 	     = 0.0;
  cs->init_lb 	     = 0.0;
  cs->z_opt 	       = 0.0;
  cs->heur_time      = 0.0;
  cs->cpx_time       = 0.0;
  cs->cpx_iterations = 0;
}


void cpx_stats_delete(cpx_stats* cs) {
  cs->rc2opt_ub	     = 0.0;
  cs->nn2opt_ub	     = 0.0;
  cs->init_ub 	     = 0.0;
  cs->init_lb 	     = 0.0;
  cs->z_opt 	       = 0.0;
  cs->heur_time      = 0.0;
  cs->cpx_time       = 0.0;
  cs->cpx_iterations = 0;
}


void cpx_stats_print(cpx_stats* cs) {
  printf("----------------------------------------------------------------\n");
  printf("# cost of optimal sol.     = %f\n", cs->z_opt);
  printf("----------------------------------------------------------------\n");

  printf("# upper bound by RC + 2OPT = %f\n", cs->rc2opt_ub);
  printf("# upper bound by NN + 2OPT = %f\n", cs->nn2opt_ub);

  printf("# initial upper bound      = %f\n", cs->init_ub);
  printf("# initial lower bound      = %f\n", cs->init_lb);
  printf("# quality of init. ub      = %f\n", cs->init_ub / cs->z_opt);
  printf("# quality of init. lb      = %f\n", cs->init_lb / cs->z_opt);

  printf("# cplex iterations         = %d\n", cs->cpx_iterations);
  printf("time for preprocessing     = %f secs\n",
                              cs->heur_time + cs->lb_time);
  printf("time for cplex operations  = %f secs\n", cs->cpx_time);

  printf("----------------------------------------------------------------\n");
}
