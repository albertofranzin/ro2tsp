#ifndef LAGRANGE_H_
#define LAGRANGE_H_

#include "graph.h"

double lagrange(graph* G, graph* H, int K, double ub, double alpha);
double plot_lagrange(graph* G, graph* H, int K, double ub, double alpha);

#endif
