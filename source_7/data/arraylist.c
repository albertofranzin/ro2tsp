#include "../data/arraylist.h"


int arraylist_init(arraylist *l) {

	l->capacity	= -1;
	l->size		= -1;
	l->pred 	= NULL;
	l->next 	= NULL;
    return 0;

}

int arraylist_delete(arraylist *l) {

    l->capacity	= -1;
    l->size 	= -1;

    if (l->pred != NULL) {
        free(l->pred);
        l->pred = NULL;
    }
    if (l->next != NULL) {
        free(l->next);
        l->next = NULL;
    }

    return 0;
}

int arraylist_empty(arraylist *l) {

    int i;
    int c = l->capacity;
    l->size		= 0;

    for (i = 0; i < c+1; i++ ) {
    	l->pred[i] = -1;
    	l->next[i] = -1;
    }

    l->pred[c] = c;
    l->next[c] = c;
    return 0;
}

int arraylist_setup(arraylist *l, int c) {

#ifdef DEBUG
    if (c < 1) {
        fprintf(stderr, "Error in /data/arraylist.c:\n"
                        "function: arraylist_setup:\n"
                        "arraylist_setup : errcode %d\n", 1);
        return -1;
    }
#endif

    if (l->pred != NULL) {
        free(l->pred);
    }
    if (l->next != NULL) {
        free(l->next);
    }

    int i;
    l->capacity	= c;
    l->size		= 0;

    l->pred	= (int*)malloc((c+1) * sizeof(int));
    l->next	= (int*)malloc((c+1) * sizeof(int));

    for (i = 0; i < c+1; i++ ) {
    	l->pred[i] = -1;
    	l->next[i] = -1;
    }

    l->pred[c] = c;
    l->next[c] = c;
    return 0;

}

int arraylist_copy(arraylist *from, arraylist *to) {

    if (to->pred != NULL) {
        free(to->pred);
    }
    if (to->next != NULL) {
        free(to->next);
    }

    int i;
    int c = from->capacity;
    to->capacity	= c;
    to->size		= from->size;
    to->pred 		= (int*)malloc((c+1) * sizeof(int));
    to->next 		= (int*)malloc((c+1) * sizeof(int));
    for (i = 0; i < c+1; i++) {
        to->pred[i]	= from->pred[i];
        to->next[i]	= from->next[i];
    }
    return 0;

}

int arraylist_insert_next(arraylist *l, int i, int e) {

#ifdef DEBUG
    if (e < 0 || e >= l->capacity) {
        fprintf(stderr, "Error in /data/arraylist.c:\n"
                        "function: arraylist_insert_next:\n"
                        "arraylist_insert_next : errcode %d\n", 1);
        return -1;
    }
    if (i < 0 || i > l->capacity) {
        fprintf(stderr, "Error in /data/arraylist.c:\n"
                        "function: arraylist_insert_next:\n"
                        "arraylist_insert_next : errcode %d\n", 2);
        return -2;
    }
    if (l->size >= l->capacity) {
        fprintf(stderr, "Error in /data/arraylist.c:\n"
                        "function: arraylist_insert_next:\n"
                        "arraylist_insert_next : errcode %d\n", 3);
        return -3;
    }
    if (l->pred[i] == -1 || l->next[i] == -1) {
        fprintf(stderr, "Error in /data/arraylist.c:\n"
                        "function: arraylist_insert_next:\n"
                        "arraylist_insert_next : errcode %d\n", 4);
        return -4;
    }
    if (l->pred[e] != -1 || l->next[e] != -1) {
        fprintf(stderr, "Error in /data/arraylist.c:\n"
                        "function: arraylist_insert_next:\n"
                        "arraylist_insert_next : errcode %d\n", 5);
        return -5;
    }
#endif

    int tmp 		= l->next[i];
    l->next[i]		= e;
    l->pred[e] 		= i;
    l->next[e] 		= tmp;
    l->pred[tmp] 	= e;
	l->size++;
	return 0;

}

int arraylist_insert_pred(arraylist *l, int i, int e) {

#ifdef DEBUG
    if (e < 0 || e >= l->capacity) {
        fprintf(stderr, "Error in /data/arraylist.c:\n"
                        "function: arraylist_insert_pred:\n"
                        "arraylist_insert_pred : errcode %d\n", 1);
        return -1;
    }
    if (i < 0 || i > l->capacity) {
        fprintf(stderr, "Error in /data/arraylist.c:\n"
                        "function: arraylist_insert_pred:\n"
                        "arraylist_insert_pred : errcode %d\n", 2);
        return -2;
    }
    if (l->size >= l->capacity) {
        fprintf(stderr, "Error in /data/arraylist.c:\n"
                        "function: arraylist_insert_pred:\n"
                        "arraylist_insert_pred : errcode %d\n", 3);
        return -3;
    }
    if (l->pred[i] == -1 || l->next[i] == -1) {
        fprintf(stderr, "Error in /data/arraylist.c:\n"
                        "function: arraylist_insert_pred:\n"
                        "arraylist_insert_pred : errcode %d\n", 4);
        return -4;
    }
    if (l->pred[e] != -1 || l->next[e] != -1) {
        fprintf(stderr, "Error in /data/arraylist.c:\n"
                        "function: arraylist_insert_pred:\n"
                        "arraylist_insert_pred : errcode %d\n", 5);
        return -5;
    }
#endif

    int tmp 		= l->pred[i];
    l->next[tmp] 	= e;
    l->pred[e] 		= tmp;
    l->next[e] 		= i;
    l->pred[i]		= e;
	l->size++;
	return 0;

}

int arraylist_remove(arraylist *l, int e) {

#ifdef DEBUG
    if (e < 0 || e >= l->capacity) {
        fprintf(stderr, "Error in /data/arraylist.c:\n"
                        "function: arraylist_remove:\n"
                        "arraylist_remove : errcode %d\n", 1);
        return -1;
    }
    if (l->pred[e] == -1 || l->next[e] == -1) {
        fprintf(stderr, "Error in /data/arraylist.c:\n"
                        "function: arraylist_remove:\n"
                        "arraylist_remove : errcode %d\n", 2);
        return -2;
    }
#endif

    int p		= l->pred[e];
    int n		= l->next[e];
    l->pred[n]	= p;
    l->next[p]	= n;
    l->pred[e]	= -1;
    l->next[e]	= -1;
	l->size--;
	return 0;

}

int arraylist_isin(arraylist *l, int e) {

#ifdef DEBUG
    if (e < 0 || e >= l->capacity) {
        fprintf(stderr, "Error in /data/arraylist.c:\n"
                        "function: arraylist_get_pred:\n"
                        "arraylist_get_pred : errcode %d\n", 1);
        return -1;
    }
#endif

    return (l->pred[e] != -1 && l->next[e] != -1);

}
