#include "../data/coords.h"


int coords_init(coords *c) {

	c->num		= -1;
    c->x		= NULL;
    c->y		= NULL;
    return 0;

}


int coords_delete(coords *c) {

    c->num		= -1;
    if (c->x != NULL) {
        free(c->x);
        c->x = NULL;
    }
    if (c->y != NULL) {
        free(c->y);
        c->y = NULL;
    }
    return 0;

}


int coords_setup(coords *c, int n) {

#ifdef DEBUG
    if (n < 1) {
        fprintf(stderr, "Error in /data/coords.c:\n"
                        "function: coords_setup:\n"
                        "coords_setup : errcode %d\n", 1);
        return -1;
    }
#endif

    if (c->x != NULL) {
        free(c->x);
    }
    if (c->y != NULL) {
        free(c->y);
    }

    int i;
    c->num	= n;
    c->x	= (double*)malloc(n * sizeof(double));
    c->y	= (double*)malloc(n * sizeof(double));
    for (i = 0; i < n; i++ ) {
        c->x[i]	= -1.0;
        c->y[i]	= -1.0;
    }
    return 0;

}


int coords_copy(coords *from, coords *to) {

    if (to->x != NULL) {
        free(to->x);
    }
    if (to->y != NULL) {
        free(to->y);
    }

    int i;
    to->num	= from->num;
    to->x 	= (double*)malloc(from->num * sizeof(double));
    to->y 	= (double*)malloc(from->num * sizeof(double));
    for (i = 0; i < from->num; i++) {
        to->x[i] = from->x[i];
        to->y[i] = from->y[i];
    }
    return 0;

}


