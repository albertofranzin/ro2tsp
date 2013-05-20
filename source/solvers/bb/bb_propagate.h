#ifndef BB_PROPAGATE_H_
#define BB_PROPAGATE_H_

#include "../../base/constants.h"
#include "../../data/graph.h"
#include "../../data/clist.h"
#include "../../data/tsp_env.h"
//#include "bb_env.h"
#include "bb_bkp.h"

int bb_apply_and_check(int       w,
                       int       v,
                       int       u,
                       double    constr_wv,
                       double    constr_wu,
                       tsp_env  *env,
                       bb_bkp   *bkp,
                       bb_bkp   *update,
                       int       mode);

int bb_propagate_and_check(int      w,
                           int      v,
                           int      u,
                           int      constr_wv,
                           int      constr_wu,
                           tsp_env *env,
                           bb_bkp  *bkp,
                           bb_bkp  *update,
                           int      mode);

#endif
