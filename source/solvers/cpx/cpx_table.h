#ifndef CPX_TABLE_H_
#define CPX_TABLE_H_

#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "../../data/graph.h"

/*
 * cpx_table
 *
 * hash table for computing position of a constraint starting from vertices
 * and viceversa. It's a 'size x 3' matrix
 *
 * - size    : # of entries (rows)
 * - n       : number of nodes of the graph associated to the table.
 *               Useful for computing the position of the elements
 *               in the table
 * - entries : index of node/position, according to the following structure:
 *             |  v1  |  v2  |  position  |
 *             |  v1  |  v3  |  position  |
 *             |  v1  |  v4  |  position  |
 *             ...
 *
 */
typedef struct _cpx_table {
  int size;
  int n;
  int** entries;
} cpx_table;

/*
 * cpx_table_init
 *
 * create a table starting from a graph
 */
void cpx_table_init(cpx_table* CPX_TAB, int size);

/*
 * cpx_table_delete
 *
 * delete a table
 */
void cpx_table_delete(cpx_table* CPX_TAB);

/*
 * cpx_table_populate
 *
 * hash (v1, v2)->(pos)
 *
 * cpx_table *   : hash table
 * int *         : pointer to index of vertex 1 (not modified)
 * int *         : pointer to index of vertex 2 (not modified)
 * int *         : pointer to index of position (to be modified)
 */
/* 
 * le posizioni dei lati (i, j) sono assegnate come segue (sia n=5)
 * lato: (1,2) (1,3) (1,4) (1,5) (2,3) (2,4) (2,5) (3,4) (3,5) ...
 * pos :   1     2     3     4     5     6     7     8     9   ...
 */
void cpx_table_populate(cpx_table* CPX_TAB, graph* G);

/*
 * vertices_from_pos
 *
 * hash (pos)->(v1, v2)
 *
 * cpx_table *   : hash table
 * int *         : pointer to index of vertex 1 (to be modified)
 * int *         : pointer to index of vertex 2 (to be modified)
 * int           : index of position (not modified)
 */
void vertices_from_pos(cpx_table* CPX_TAB, int* x, int* y, int pos);

/*
 * pos_from_vertices
 *
 * hash (pos)<-(v1, v2)
 *
 * cpx_table *   : hash table
 * int           : index of vertex 1 (not modified)
 * int           : index of vertex 2 (not modified)
 * int *         : pointer to index of position (to be modified)
 */
void pos_from_vertices(cpx_table* CPX_TAB, int x, int y, int* pos);


#endif
