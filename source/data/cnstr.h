#ifndef CNSTR_H_
#define CNSTR_H_

// lato
struct cnstr {
  int x; // x e y indici dei due nodi estremi del lato
  int y;
  int constr;
};

typedef struct cnstr cnstr;

#endif
