#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "onetree.h"

void onetree_init(onetree* OT, int n) {
  (*OT).n = n;
  (*OT).first_edge.node = 0;
  (*OT).first_edge.cost = 0.0;
  (*OT).second_edge.node = 0;
  (*OT).second_edge.cost = 0.0;
  tree_init(&(*OT).tree, n);
}

void onetree_delete(onetree* OT) {
  (*OT).n = 0;
  (*OT).first_edge.node = 0;
  (*OT).first_edge.cost = 0.0;
  (*OT).second_edge.node = 0;
  (*OT).second_edge.cost = 0.0;
  tree_delete(&(*OT).tree);
}

void onetree_copy(onetree* FROM, onetree* TO) {

  int n = (*FROM).n;
  onetree_delete(TO);
  onetree_init(TO, n);
  (*TO).first_edge.node = (*FROM).first_edge.node;
  (*TO).first_edge.cost = (*FROM).first_edge.cost;
  (*TO).second_edge.node = (*FROM).second_edge.node;
  (*TO).second_edge.cost = (*FROM).second_edge.cost;
  tree_copy(&(*FROM).tree, &(*TO).tree);
}

void onetree_set_root(onetree* OT, int r) {
  tree_set_root(&(*OT).tree, r);
}

int onetree_get_root(onetree* OT) {
  return tree_get_root(&(*OT).tree);
}

int onetree_get_pred(onetree* OT, int v) {
  return tree_get_pred(&(*OT).tree, v);
}

int onetree_get_first_node(onetree* OT) {
  return (*OT).first_edge.node;
}

int onetree_get_second_node(onetree* OT) {
  return (*OT).second_edge.node;
}

void onetree_insert_edge(onetree* OT, int u, int v, double cost) {
  if (onetree_adjacent_nodes(OT, u, v))
    return;

  if (u == 1 || v == 1) {
    if (u == 1) {
      if ((*OT).first_edge.node == 0) {
        (*OT).first_edge.node = v;
        (*OT).first_edge.cost = cost;
      }
      else if ((*OT).second_edge.node == 0) {
        (*OT).second_edge.node = v;
        (*OT).second_edge.cost = cost;
      }
      (*OT).tree.V[u-1].deg++;
      (*OT).tree.V[v-1].deg++;
    }
    else if (v == 1) {
      if ((*OT).first_edge.node == 0) {
        (*OT).first_edge.node = u;
        (*OT).first_edge.cost = cost;
      }
      else if ((*OT).second_edge.node == 0) {
        (*OT).second_edge.node = u;
        (*OT).second_edge.cost = cost;
      }
      (*OT).tree.V[u-1].deg++;
      (*OT).tree.V[v-1].deg++;
    }

  }
  else
    tree_insert_edge(&(*OT).tree, u, v, cost);
}

void onetree_remove_edge(onetree* OT, int u, int v) {

  if (!onetree_adjacent_nodes(OT, u, v))
    return;

  if (u == 1 || v == 1) {
    if (u == 1) {
      if ((*OT).first_edge.node == v) {
        (*OT).first_edge.node = 0;
        (*OT).first_edge.cost = 0.0;
      }
      else if ((*OT).second_edge.node == v) {
        (*OT).second_edge.node = 0;
        (*OT).second_edge.node = 0.0;
      }
      (*OT).tree.V[u-1].deg--;
      (*OT).tree.V[v-1].deg--;
    }
    else if (v == 1) {
      if ((*OT).first_edge.node == u) {
        (*OT).first_edge.node = 0;
        (*OT).first_edge.cost = 0.0;
      }
      else if ((*OT).second_edge.node == u) {
        (*OT).second_edge.node = 0;
        (*OT).second_edge.node = 0.0;
      }
      (*OT).tree.V[u-1].deg--;
      (*OT).tree.V[v-1].deg--;
    }
  }
  else
    tree_remove_edge(&(*OT).tree, u, v);
}

void onetree_set_edge_cost(onetree* OT, int u, int v, double cost) {

  if (!onetree_adjacent_nodes(OT, u, v))
    return;

  if (u == 1 || v == 1) {
    if (u == 1) {
      if ((*OT).first_edge.node == v)
	(*OT).first_edge.cost = cost;
      else if ((*OT).second_edge.node == v)
	(*OT).second_edge.cost = cost;
    }
    else if (v == 1) {
      if ((*OT).first_edge.node == u)
	(*OT).first_edge.cost = cost;
      else if ((*OT).second_edge.node == u)
	(*OT).second_edge.cost = cost;
    }
  }
  else
    tree_set_edge_cost(&(*OT).tree, u, v, cost);
}


double onetree_get_edge_cost(onetree* OT, int u, int v) {

  if (!onetree_adjacent_nodes(OT, u, v))
    return 0.0;

  if (u == 1 || v == 1) {
    if (u == 1) {
      if ((*OT).first_edge.node == v)
        return (*OT).first_edge.cost;
      else if ((*OT).second_edge.node == v)
        return (*OT).second_edge.cost;
    } else if (v == 1) {
      if ((*OT).first_edge.node == u)
        return (*OT).first_edge.cost;
      else if ((*OT).second_edge.node == u)
        return (*OT).second_edge.cost;
    }
  }
  else
    return tree_get_edge_cost(&(*OT).tree, u, v);
}

int onetree_get_node_deg(onetree* OT, int v) {
  return tree_get_node_deg(&(*OT).tree, v);
}

int onetree_adjacent_nodes(onetree* OT, int u, int v) {
  if (u == 1 && v != 1)
    return ((*OT).first_edge.node == v || (*OT).second_edge.node == v);
  else if (u != 1 && v == 1)
    return ((*OT).first_edge.node == u || (*OT).second_edge.node == u);
  else
    return tree_adjacent_nodes(&(*OT).tree, u, v);
}

double onetree_get_cost(onetree* OT) {
  double c;

  c = tree_get_cost(&(*OT).tree);
  if ((*OT).first_edge.node > 0)
    c += (*OT).first_edge.cost;
  if ((*OT).second_edge.node > 0)
    c += (*OT).second_edge.cost;
 
  return c;
}

/*

  THIS IS SOMETHING FOR THE LINEAR-TIME RE-ROOTING OF A 1-TREE
  I began writing this instead of the other thing I had to do, crap.

  int i, j, deg;
  int u = 0, v = 0;

  // look for two nodes with degree = 1
  for (i = 1; i < T->n; ++i) {
    if (tree_get_node_deg(T, i) == 1) {
      u = i;
      break;
    }
  }
  for (; i <= T->n; ++i) {
    if (tree_get_node_deg(T, i) == 1) {
      v = i;
      break;
    }
  }

  // maybe there is only one node with degree 1 : in this case, look for a node
  // with degree 3. An edge insisting on it will be removed, causing another node
  // to have degree 1. Be careful to not insert a denied edge.
  // This node will be taken into account for our job.
  if (v == 0) {
    for (i = 1; i < T->n; ++i) {
      deg = tree_get_node_deg(T, i);
      if (deg > 2) {
        for (j = 1; j <= deg; ++j) {
          if (j != i && tree_adjacent_nodes(T, i, j) && tree_get_node_deg(T, i) == 2
              && tree_get_edge_cost(T, u, j) < BIG) {
            v = j;
            tree_remove_edge(T, i, j);
            break;
          }
        }
      }
    }
  }

  // join the 2 nodes u, v
  if (v == 0) {
    / * haha * /
  } else {
    tree_insert_edge(T, u, v, graph_get_edge_cost(G, u, v));
  }

  // do the rest

*/
