#include "../data/cycle.h"

int cycle_init(cycle *c) {

	c->vrtx_num		= -1;
    c->vrtx_idx		= NULL;
    return 0;

}


int cycle_delete(cycle *c) {

    c->vrtx_num		= -1;
    if (c->vrtx_idx != NULL) {
        free(c->vrtx_idx);
        c->vrtx_idx = NULL;
    }
    return 0;

}


int cycle_setup(cycle *c, int n) {

#ifdef DEBUG
	if (n < 1) {
        fprintf(stderr, "Error in /data/cycle.c:\n"
                        "function: cycle_setup:\n"
                        "cycle_setup : errcode %d\n", 1);
        return -1;
    }
#endif

    if (c->vrtx_idx != NULL) {
        free(c->vrtx_idx);
    }

    int i;
    c->vrtx_num		= n;
    c->vrtx_idx		= (int*)malloc(n * sizeof(int));
    for (i = 0; i < n; i++ ) {
        c->vrtx_idx[i]	= -1;
    }
    return 0;

}


int cycle_copy(cycle *from, cycle *to) {

    if (to->vrtx_idx != NULL) {
        free(to->vrtx_idx);
    }

    int i;
    to->vrtx_num	= from->vrtx_num;
    to->vrtx_idx 	= (int*)malloc(from->vrtx_num * sizeof(int));
    for (i = 0; i < from->vrtx_num; i++) {
        to->vrtx_idx[i] = from->vrtx_idx[i];
    }
    return 0;

}
