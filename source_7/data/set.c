#include "../data/set.h"


int set_init(set *s) {

    s->num	= -1;
    s->list = NULL;
    s->last	= NULL;
    s->size	= NULL;
    s->next	= NULL;

    return 0;

}


int set_delete(set *s) {

    s->num	= -1;
    if (s->list != NULL) {
        free(s->list);
        s->list = NULL;
    }
    if (s->last != NULL) {
        free(s->last);
        s->last = NULL;
    }
    if (s->size != NULL) {
        free(s->size);
        s->size = NULL;
    }
    if (s->next != NULL) {
        free(s->next);
        s->next = NULL;
    }
    return 0;

}


int set_empty(set *s) {

    int i;
    int n = s->num;

    for (i = 0; i < n; i++ ) {
    	s->list[i]	=  i;
    	s->last[i]	=  i;
    	s->size[i]	=  1;
    	s->next[i]	= -1;
    }

    return 0;

}


int set_setup(set *s, int n) {

#ifdef DEBUG
    if (n < 1) {
        fprintf(stderr, "Error in /data/set.c:\n"
                        "function: set_setup:\n"
                        "set_setup : errcode %d\n", 1);
        return -1;
    }
#endif

    if (s->list != NULL) {
        free(s->list);
    }
    if (s->last != NULL) {
        free(s->last);
    }
    if (s->size != NULL) {
        free(s->size);
    }
    if (s->next != NULL) {
        free(s->next);
    }

    int i;
    s->num	= n;
    s->list	= (int*)malloc(n * sizeof(int));
    s->last	= (int*)malloc(n * sizeof(int));
    s->size	= (int*)malloc(n * sizeof(int));
    s->next	= (int*)malloc(n * sizeof(int));

    for (i = 0; i < n; i++ ) {
    	s->list[i]	=  i;
    	s->last[i]	=  i;
    	s->size[i]	=  1;
    	s->next[i]	= -1;
    }
    return 0;

}



int set_copy(set *from, set *to) {

    if (to->list != NULL) {
        free(to->list);
    }
    if (to->last != NULL) {
        free(to->last);
    }
    if (to->size != NULL) {
        free(to->size);
    }
    if (to->next != NULL) {
        free(to->next);
    }

    int i;
    int n = from->num;
    to->num		= n;
    to->list 	= (int*)malloc(n * sizeof(int));
    to->last 	= (int*)malloc(n * sizeof(int));
    to->size 	= (int*)malloc(n * sizeof(int));
    to->next 	= (int*)malloc(n * sizeof(int));
    for (i = 0; i < n; i++) {
        to->list[i]	= from->list[i];
        to->last[i]	= from->last[i];
        to->size[i]	= from->size[i];
        to->next[i]	= from->next[i];
    }
    return 0;

}


int set_find(set *s, int z, int *x) {

#ifdef DEBUG
    if (z < 0 || z >= s->num) {
        fprintf(stderr, "Error in /data/set.c:\n"
                        "function: set_find:\n"
                        "set_find : errcode %d\n", 1);
        return -1;
    }
#endif

    *x = s->list[z];
	return 0;

}


int set_union(set *s, int x, int y) {

	int i;

	/* always append the smaller list to the longer one */
	if (s->size[x] < s->size[y]) {

		s->next[s->last[y]]	= x;
		s->last[y] = s->last[x];
		s->size[y] = s->size[y] + s->size[x];

		s->list[x] = y;
		i = x;
		while (s->next[i] != -1) {
			i = s->next[i];
			s->list[i] = y;
		}

	}
	else {

		s->next[s->last[x]]	= y;
		s->last[x] = s->last[y];
		s->size[x] = s->size[x] + s->size[y];

		s->list[y] = x;
		i = y;
		while (s->next[i] != -1) {
			i = s->next[i];
			s->list[i] = x;
		}

	}
	return 0;

}





