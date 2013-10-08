#ifndef CPX_TABLE_H_
#define CPX_TABLE_H_


#include <stdio.h>
#include <stdlib.h>
#include "../../data/graph.h"


// ---------------------------------------------------------- //
//                                                            //
//                       data structures                      //
//                                                            //
// ---------------------------------------------------------- //

/**
 * cpx_table_entry
 * contains coordinates of an entry (vertex) of the graph
 *
 * x,y coordinates
 */
typedef struct _cpx_table_entry {
  int x;
  int y;
} cpx_table_entry;

/**
 * cpx_table
 * contains the list of entries (vertices)
 *
 * n       : number of entries
 * entries : list of entries
 */
typedef struct _cpx_table {
  int n;
  cpx_table_entry* entries;
} cpx_table;


// ---------------------------------------------------------- //
//                                                            //
//                          functions                         //
//                                                            //
// ---------------------------------------------------------- //


/**
 * initialize table
 * @param T	table
 * @param G	graph
 */
void cpx_table_init(cpx_table* T, graph* G);


/**
 * delete    table
 * @param T  table
 */
void cpx_table_delete(cpx_table* T);


/**
 * get the vertices of an edge with given index
 * @param T	    table
 * @param x	    edge vertex
 * @param y	    edge vertex
 * @param idx	edge index
 */
void vrtx_from_idx(cpx_table* T, int* x, int* y, int idx);


/**
 * get the index of an edge with given vertices
 * @param T     table
 * @param x	    edge vertex
 * @param y	    edge vertex
 * @param idx	edge index
 */
void idx_from_vrtx(cpx_table* T, int x, int y, int* idx);


#endif
