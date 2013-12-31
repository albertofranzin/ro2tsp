#ifndef GRAPH_H_
#define GRAPH_H_

#include <stdlib.h>
#include <stdio.h>
#include "../base/global.h"


typedef struct _graph {

	int		vrtx_num;		/* number of vertices */
	int		*vrtx_deg_all;	/* list containing the number of all */
							/* edges incident to each vertex */
	int		*vrtx_deg_frc;	/* list containing the number of forced */
							/* edges incident to each vertex */
	int		*vrtx_deg_frb;	/* list containing the number of forbidden */
							/* edges incident to each vertex */
	double	*edge_cost;		/* list of costs, one for each edge */
	int		*edge_cstr;		/* list of constraints, one for each edge */
	int		*edge_flag;		/* list of flags, one for each edge; */
							/* if the edge is present, then its flag */
							/* should be setted equal to 1 */
} graph;

/**
* @brief    	Initialize a graph structure.
* @param[in] g	graph
* @return   	= 0 if no error
* 				< 0 if error
* @pre			-
* @post			-
* @note			-
*/
int graph_init(graph *g);


/**
* @brief    	Delete a graph.
* @param[in] g  graph
* @return   	= 0 if no error
* 				< 0 if error
* @pre      	g is supposed to be initialized
* @post			-
* @note			-
*/
int graph_delete(graph *g);


/**
* @brief    	Setup a graph, allocate memory
*           	space for it.
* @param[in] g  graph
* @param[in] n  number of vertices
* @return   	= 0 if no error
* 				< 0 if error
* @pre      	g is supposed to be initialized
* @pre      	n >= 1
* @post			-
* @note			-
*/
int graph_setup(graph *g, int n);


/**
* @brief       		Copy a graph.
* @param[in]  from	graph to be copied
* @param[out] to 	copied graph
* @return   		= 0 if no error
* 					< 0 if error
* @pre        		from is supposed to be initialized
* @pre        		to is supposed to be initialized
* @pre				-
* @post				-
* @note				-
*/
int graph_copy(graph *from, graph *to);


/**
* @brief		Check if there is an edge between two vertices.
* @param[in] g  graph
* @param[in] u  first vertex
* @param[in] v  second vertex
* @return   	= 0 if no error and if there is no edge between u and v
* 				= 1 if no error and if there is an edge between u and v
* 				< 0 if an error occurred
* @pre       	g is supposed to be initialized
* @pre      	u is a valid vertex index
* @pre      	v is a valid vertex index
* @pre      	u != v i.e. no self-loops
* @post			-
* @note			-
*/
int graph_is_edge(graph* g, int u, int v);


/**
* @brief		Add an edge to the graph.
* @param[in] g  graph
* @param[in] u  first vertex
* @param[in] v  second vertex
* @param[in] w	weight or cost
* @param[in] c	constraint
* @return   	= 0 if no error
* 				< 0 if error
* @pre       	g is supposed to be initialized
* @pre      	u is a valid vertex index
* @pre      	v is a valid vertex index
* @pre      	u != v i.e. no self-loops
* @pre			there is no edge between u and v
* @post			-
* @note			-
*/
int graph_insert_edge(graph *g, int u, int v, double w, int c);


/**
* @brief    	Remove an edge.
* @param[in] g  graph
* @param[in] u  first vertex
* @param[in] v  second vertex
* @return   	= 0 if no error
* 				< 0 if error
* @pre       	g is supposed to be initialized
* @pre      	u is a valid vertex index
* @pre      	v is a valid vertex index
* @pre      	u != v i.e. no self-loops
* @pre			there is an edge between u and v
* @post			-
* @note			-
*/
int graph_remove_edge(graph *g, int u, int v);


/**
* @brief		Set the cost of an edge.
* @param[in] g  graph
* @param[in] u  first vertex
* @param[in] v  second vertex
* @param[in] w  weight or cost
* @return   	= 0 if no error
* 				< 0 if error
* @pre          g is supposed to be initialized
* @pre      	u is a valid vertex index
* @pre      	v is a valid vertex index
* @pre      	u != v i.e. no self-loops
* @pre			there is an edge between u and v
* @post			-
* @note			-
*/
int graph_set_edge_cost(graph *g, int u, int v, double w);


/**
* @brief			Get the cost of an edge.
* @param[in] 	g	graph
* @param[in] 	u	first vertex
* @param[in] 	v	second vertex
* @param[out] 	w	weight or cost
* @return   		= 0 if no error
* 					< 0 if error
* @pre          	g is supposed to be initialized
* @pre      		u is a valid vertex index
* @pre      		v is a valid vertex index
* @pre      		u != v i.e. no self-loops
* @pre				there is an edge between u and v
* @post				-
* @note				-
*/
int graph_get_edge_cost(graph *g, int u, int v, double *w);


/**
* @brief		Set the constraint of an edge.
* @param[in] g  graph
* @param[in] u  first vertex
* @param[in] v  second vertex
* @param[in] c  constraint
* @return   	= 0 if no error
* 				< 0 if error
* @pre          g is supposed to be initialized
* @pre      	u is a valid vertex index
* @pre      	v is a valid vertex index
* @pre      	u != v i.e. no self-loops
* @pre			there is an edge between u and v
* @post			-
* @note			-
*/
int graph_set_edge_cstr(graph *g, int u, int v, int c);


/**
* @brief			Get the constraint of an edge.
* @param[in] 	g	graph
* @param[in] 	u	first vertex
* @param[in] 	v	second vertex
* @param[out] 	c	constraint
* @return  	 		= 0 if no error
* 					< 0 if error
* @pre          	g is supposed to be initialized
* @pre      		u is a valid vertex index
* @pre      		v is a valid vertex index
* @pre      		u != v i.e. no self-loops
* @pre			there is an edge between u and v
* @post				-
* @note				-
*/
int graph_get_edge_cstr(graph *g, int u, int v, int *c);


#endif /* GRAPH_H_ */
