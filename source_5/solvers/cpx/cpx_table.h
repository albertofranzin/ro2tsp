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

	
typedef struct _cpx_table_entry {

  int x;
  int y;

} cpx_table_entry;


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
 * delete table
 * @param T 	table
 */
void cpx_table_delete(cpx_table* T);


/**
 * get the vertices of an edge with specified index
 * @param T	table
 * @param x	edge vertex
 * @param y	edge vertex
 * @param idx	edge index
 */
void vrtx_from_idx(cpx_table* T, int* x, int* y, int idx);


/**
 * get the index of an edge with specified vertices
 * @param T table
 * @param x	edge vertex
 * @param y	edge vertex
 * @param idx	edge index
 */
void idx_from_vrtx(cpx_table* T, int x, int y, int* idx);


#endif
