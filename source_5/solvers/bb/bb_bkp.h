#ifndef BB_BKP_H_
#define BB_BKP_H_



#include "../../base/constants.h"
#include "../../data/graph.h"
#include "../../data/onetree.h"
#include "../../data/edge.h"
#include "../../data/elist.h"
#include "../../data/tsp_env.h"



typedef struct _bb_bkp {

  int mode; // BASIC, SAVE_OT


  elist modified_constraints;

  int curr_call;

  int curr_level;


  onetree OT_CURR;

  double z_curr;

} bb_bkp;



void bb_bkp_init(bb_bkp* bkp, int mode);

void bb_bkp_delete(bb_bkp* bkp);

void bb_bkp_save_constraint(bb_bkp* bkp, int x, int y, int constr);

void bb_bkp_save(bb_bkp* bkp, tsp_env* env);

void bb_bkp_restore(bb_bkp* bkp, tsp_env* env);



#endif
