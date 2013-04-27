#ifndef TSP_CONSTRAINTS_H_
#define TSP_CONSTRAINTS_H_

#include "constants.h"
#include "graph.h"
#include "vector.h"
#include "tsp_status.h"
#include "tsp_backup.h"

void tsp_constraints_a(int w, int v, int u, double cost_wv, double cost_wu, tsp_status* status, tsp_backup* backup, tsp_backup* backup_fc);

#endif
