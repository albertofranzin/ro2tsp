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

inline void onetree_set_root(onetree* OT, int r) {
  tree_set_root(&(*OT).tree, r);
}

inline int onetree_get_root(onetree* OT) {
  return tree_get_root(&(*OT).tree);
}

inline int onetree_get_pred(onetree* OT, int v) {
  return tree_get_pred(&(*OT).tree, v);
}

inline int onetree_get_first_node(onetree* OT) {
  return (*OT).first_edge.node;
}

inline int onetree_get_second_node(onetree* OT) {
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
      else //if ((*OT).second_edge.node == v)
        return (*OT).second_edge.cost;
    } else {  // if (v == 1) {
      if ((*OT).first_edge.node == u)
        return (*OT).first_edge.cost;
      else// if ((*OT).second_edge.node == u)
        return (*OT).second_edge.cost;
    }
  } else
    return tree_get_edge_cost(&(*OT).tree, u, v);
}

inline int onetree_get_node_deg(onetree* OT, int v) {
  return tree_get_node_deg(&(*OT).tree, v);
}

inline int onetree_adjacent_nodes(onetree* OT, int u, int v) {
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
 * onetree_reroot
 * OT : 1-tree
 * root : new root node
 *
 * We suppose OT contains a cycle (or subcycle), and root belongs to it.
 * Root gives the direction.
 *
 * performs O(n)-time re-rooting and re-orienting of a 1-tree
 */
void onetree_reroot(onetree *OT, int root) {
  //int old_root = onetree_get_root(OT);
  int current, prev, steps, tmpp, tmps;
  onetree_set_root(OT, root);

  steps = 0;
  prev = root;

  // two cases

  if (root == 1) {
    // first one: root is node 1, which is treated separately
    // from the rest of the nodes
    // We know we can start from node 1, and move to its successor, which we assume
    // to be the node contained in first_edge. Then we move along that direction,
    // rotating every edge that goes 'against' our chosen path, that is, the successor
    // of the node corresponds to the node we are coming from. When we arrive to
    // the node contained in OT->second_edge, which connects back to 1, we are done.

    if (OT->tree.V[OT->first_edge.node-1].succ == 1) {
      printf("aaa\n");
      tree_redirect_node(&OT->tree, OT->first_edge.node);
    }

    current = OT->first_edge.node;
    while (current != OT->second_edge.node) {
      printf("-- %d %d %d --\n", prev, current, tree_get_succ(&OT->tree, current));
      tmpp = tree_get_pred(&OT->tree, current);
      tmps = tree_get_succ(&OT->tree, current);
      if (tmps == prev) {
        printf("mmm\n");
        tree_redirect_node(&OT->tree, current);
      }
      prev = current;
      current = tree_get_succ(&OT->tree, current);
    }

    if (OT->tree.V[OT->second_edge.node-1].pred == 1) {
      printf("ccc\n");
      tree_redirect_node(&OT->tree, OT->second_edge.node);
    }

  } else {
    // second one: root is a node which both edges are contained in OT->tree
    // Starting from our new root, move along the path until a node connected to 1
    // is found, turning edges when needed. Then bypass 1, and follow until root
    // is joined again.
    current = tree_get_succ(&OT->tree, root);

    while (current != OT->first_edge.node  &&
           current != OT->second_edge.node &&
           current != root) {
      tmpp = tree_get_pred(&OT->tree, current);
      tmps = tree_get_succ(&OT->tree, current);
      if (tmps == prev) {
        tree_redirect_node(&OT->tree, current);
      }
      prev = current;
      current = tree_get_succ(&OT->tree, current);
    }

    if (current == OT->first_edge.node) {
      if (OT->tree.V[OT->first_edge.node-1].pred == 1) {
        tree_redirect_node(&OT->tree, OT->first_edge.node);
      }
    } else {
      if (OT->tree.V[OT->second_edge.node-1].pred == 1) {
        tree_redirect_node(&OT->tree, OT->second_edge.node);
      }
    }

    while (current != root) {
      tmpp = tree_get_pred(&OT->tree, current);
      tmps = tree_get_succ(&OT->tree, current);
      if (tmps == prev) {
        tree_redirect_node(&OT->tree, current);
      }
      prev = current;
      current = tree_get_succ(&OT->tree, current);
    }

  }
}
