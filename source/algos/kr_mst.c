#include "../algos/kr_mst.h"

int kr_mst(int *edges, int num_edges, set *vertexset, tree *t, int* status) {

	*status		= 0;
	int ntoadd 	=  t->vrtx_num - t->edge_num;
	int i, s1, s2;

	for (i = 0; i < num_edges; i++) {

		set_find(vertexset, get_v1(edges[i]), &s1);
		set_find(vertexset, get_v2(edges[i]), &s2);
		if (s1 != s2) {
			set_union(vertexset, s1, s2);
			tree_insert_edge(t, edges[i]);
			ntoadd--;
		}

		if (ntoadd == 0) {
			break;
		}

	}

	if (ntoadd > 0) {
		*status = 1;
	}

	return 0;
}


int kr_mst2(arraylist *edgelist, set *vertexset, tree *t, int* status) {

	*status			= 0;
	int ntoadd 		=  t->vrtx_num - t->edge_num;
	int num_edges	= edgelist->size;
	int i, s1, s2, my_edge;

	my_edge = edgelist->next[edgelist->capacity];
	for (i = 0; i < num_edges; i++) {

		if (get_v1(my_edge) != 0 && get_v2(my_edge) != 0) {

			set_find(vertexset, get_v1(my_edge), &s1);
			set_find(vertexset, get_v2(my_edge), &s2);
			if (s1 != s2) {
				set_union(vertexset, s1, s2);
				tree_insert_edge(t, my_edge);
				ntoadd--;
			}

			if (ntoadd == 0) {
				break;
			}

		}
		my_edge = edgelist->next[my_edge];
	}

	if (ntoadd > 0) {
		*status = 1;
	}

	return 0;
}

