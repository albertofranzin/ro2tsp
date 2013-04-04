#ifndef LAGRANGE_H_
#define LAGRANGE_H_

#include "graph.h"

double lagrange(graph* G, graph* H, int K, double ub, double alpha, int alpha_decading);
double plot_lagrange(graph* G, graph* H, int K, double ub, double alpha, int alpha_decading);

#endif
