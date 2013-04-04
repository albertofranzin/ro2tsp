#ifndef EDGE_H_
#define EDGE_H_

/* lato di un grafo;
 */
struct edge {
  int flag; // flag = 1 se il lato è presente, flag = 0 altrimenti;
  double cost; // costo del lato;
};

typedef struct edge edge;

#endif
