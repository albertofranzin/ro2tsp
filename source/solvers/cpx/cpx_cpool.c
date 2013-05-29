#include "cpx_cpool.h"



void cpx_cpool_init(cpx_cpool *cl) {
  cl->size = 0;
  cl->head.prev = NULL;
  cl->head.next = &(cl->tail);
  cl->tail.prev = &(cl->head);
  cl->tail.next = NULL;
}



void cpx_cpool_delete(cpx_cpool *cl) {
  while (cl->size > 0) {
    cpx_cpool_pop_first(cl);
  }
}



void cpx_cpool_copy(cpx_cpool *FROM, cpx_cpool *TO) {

  cpx_cpool_node *curr_node;
  cpx_constraint c;
  int i;
  int n = FROM->size;

  cpx_cpool_delete(TO);
  curr_node = FROM->head.next;

  for (i = 0; i < n; i++) {
    c = curr_node->constraint;
    cpx_cpool_push_last(TO, c.cid, c.num, c.rmatind, c.rmatval, c.rmatbeg,
                        c.rhs, c.sense);
    curr_node = curr_node->next;
  }

}


void cpx_cpool_push_last(cpx_cpool *cl,
                         int        cid,
                         int        num,
                         int       *rmatind,
                         double    *rmatval,
                         int        rmatbeg,
                         double     rhs,
                         char       sense)
{

  cpx_cpool_node *tail = &(cl->tail);
  cpx_cpool_node *last_node = cl->tail.prev;
  cpx_cpool_node *new_node = malloc(sizeof(cpx_cpool_node));

  tail->prev = new_node;
  new_node->next = tail;
  last_node->next = new_node;
  new_node->prev = last_node;
  new_node->constraint.cid = cid;
  new_node->constraint.num = num;
  new_node->constraint.rmatind = rmatind;
  new_node->constraint.rmatval = rmatval;
  new_node->constraint.rmatbeg = rmatbeg;
  new_node->constraint.rhs = rhs;
  new_node->constraint.sense = sense;
  cl->size++;
}



cpx_constraint *cpx_cpool_pop_first(cpx_cpool* cl) {

#ifdef DEBUG
  if (cl->size <= 0) {
    printf("error: cpx_cpool_pop_first\n");
    exit(1);
  }
#endif

  cpx_constraint *c;
  cpx_cpool_node *first_node = cl->head.next;

  c = &(first_node->constraint);
  (first_node->next)->prev = &(cl->head);
  cl->head.next = first_node->next;
  cl->size--;
  free(first_node);
  return c;
}


cpx_constraint *cpx_cpool_pop_last(cpx_cpool *cl) {

printf("0\n");
printf("cl->size = %d\n", cl->size);
printf("0.5\n");

#ifdef DEBUG
  if (cl->size <= 0) {
    printf("error: cpx_cpool_pop_last\n");
    exit(1);
  }
#endif

  printf("1\n");

  cpx_constraint *c;

  printf("2\n");
  cpx_cpool_node *last_node = &(cl->tail);

  printf("3\n");

  c = &(last_node->constraint);
  printf("4\n");
  (last_node->prev)->next = NULL;  // &(cl->head);
  printf("5\n");
  cl->tail = *(last_node->prev);
  printf("6\n");
  cl->size--;
  printf("7\n");
  free(last_node);
  printf("8\n");
  return c;
}

/**
 * insert a constraint
 * @param cl constraint pool
 * @param c  constraint
 */
void cpx_cpool_insert(cpx_cpool      *cl,
                      cpx_constraint *c) {

  cpx_cpool_node *tail      = &(cl->tail);
  cpx_cpool_node *last_node = cl->tail.prev;
  cpx_cpool_node *new_node  = malloc(sizeof(cpx_cpool_node));

  tail->prev      = new_node;
  new_node->next  = tail;
  last_node->next = new_node;
  new_node->prev  = last_node;
  new_node->constraint = *c;
  cl->size++;

}

void cpx_cpool_remove(cpx_cpool      *cl,
                      cpx_cpool_node *node)
{
#ifdef DEBUG
  if (node->prev == NULL || node->next == NULL) {
    printf("error: cpx_cpool_remove\n");
    exit(1);
  }
#endif

  cpx_cpool_node *prev = node->prev;
  cpx_cpool_node *next = node->next;

  prev->next = next;
  next->prev = prev;
  cl->size--;
  free(node);
}


// ---------------------------------------------------------- //
//                                                            //
//                single constraint operations                //
//                                                            //
// ---------------------------------------------------------- //

/**
 * create local branching constraint (see Fischetti-Lodi paper)
 * @param  x       coefficients of objective function
 * @param  numcols number of columns in the model
 * @param  cid     ID associated to the constraint
 * @param  sense   sense of the inequality (<=, =, >=)
 * @param  rhs     right-hand-side coefficient of the inequality
 * @return         newly created constraint
 */
cpx_constraint *cpx_create_lb_constraint(double  *x,
                                         int      numcols,
                                         int      cid,
                                         char     sense,
                                         double   rhs)
{
  int i;
  int rmatind[numcols];
  double coeffs[numcols];

  for (i = 0; i < numcols; ++i) {
    if (x[i] > 0.9) {         // == 1
      coeffs[i] = 0.0;
    } else if (x[i] < 0.1) {  // == 0
      coeffs[i] = 1.0;
    }
    rmatind[i] = i;
  }

  cpx_constraint *c = malloc(sizeof(cpx_constraint));

  c->cid     = cid;
  c->num     = numcols;
  c->rmatind = rmatind;
  c->rmatval = coeffs;
  c->rmatbeg = 0;
  c->rhs     = rhs;
  c->sense   = sense;

  return c;
}
