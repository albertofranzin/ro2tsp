#ifndef NODE_H_
#define NODE_H_


typedef struct _node {

	struct _node*	pred;
	struct _node*	next;
	int				data;

} node;


#endif /* NODE_H_ */
