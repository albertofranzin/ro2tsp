#ifndef NODE_H_
#define NODE_H_

/* nodo di un grafo;
 */
struct node {
  double x; // coordinata asse x;
  double y; // coordinata asse y;
  int deg; // grado del nodo (numero di lati incidenti nel nodo);
};

typedef struct node node;

#endif
