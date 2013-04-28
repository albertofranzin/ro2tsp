#ifndef EDGE_H_
#define EDGE_H_

// lato
struct edge {
  int x; // x e y indici dei due nodi estremi del lato
  int y;
  double cost; // costo del lato
};

typedef struct edge edge;

#endif
