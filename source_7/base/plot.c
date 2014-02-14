#include "../base/plot.h"

int plot_graph(graph* g, coords* crds, char *title) {

#ifdef DEBUG
    if (g->vrtx_num != crds->num) {
        fprintf(stderr, "Error in /base/plot.c:\n"
                        "function: plot_graph:\n"
                        "plot_graph : errcode %d\n", 1);
        return -1;
    }
#endif

	int i, j;
	int hasEdge = 0;
	double min_x = crds->x[0];
	double max_x = crds->x[0];
	double min_y = crds->y[0];
	double max_y = crds->y[0];

	for (i = 0; i < (crds->num * (crds->num - 1)) / 2; i++) {
		if (g->edge_flag[i] == 1) {
			hasEdge = 1;
			break;
		}
	}

	for (i = 0; i < crds->num; i++) {
		if (crds->x[i] < min_x) min_x = crds->x[i];
		if (crds->x[i] > max_x) max_x = crds->x[i];
		if (crds->y[i] < min_y) min_y = crds->y[i];
		if (crds->y[i] > max_y) max_y = crds->y[i];
	}

	FILE* pipe = popen("gnuplot -persist", "w");

	fprintf(pipe, "set multiplot\n");
	/* fprintf(pipe, "set size square\n"); */
	if (title != NULL) fprintf(pipe, "set title \"%s\"\n", title);
	fprintf(pipe, "set xrange [%.3f:%.3f]\n",
			min_x - (max_x - min_x)*0.01, max_x + (max_x - min_x)*0.01);
	fprintf(pipe, "set yrange [%.3f:%.3f]\n",
			min_y - (max_y - min_y)*0.01, max_y + (max_y - min_y)*0.01);
	fprintf(pipe, "set xlabel 'X'\n");
	fprintf(pipe, "set ylabel 'Y'\n");
	/* fprintf(pipe, "unset xtics\n"); */
	/* fprintf(pipe, "unset ytics\n"); */


	fprintf(pipe, "set style line 1 linetype 1 linecolor rgb \"blue\" linewidth 1\n");
	fprintf(pipe, "set style line 2 linetype 1 linecolor rgb \"red\" pointtype 7\n");

	if (hasEdge) {
		fprintf(pipe, "plot '-' using 1:2 with lines linestyle 1 notitle"
				", '' using 1:2 with points linestyle 2 notitle"
				", '' using 1:2:3 with labels offset 0.5,0.5 notitle\n");
	}
	else {
		fprintf(pipe, "plot '-' using 1:2 with points linestyle 2 notitle"
				", '' using 1:2:3 with labels offset 0.5,0.5 notitle\n");
	}


	/* plot edges */
	if (hasEdge) {
		for (i = 0; i < crds->num; i++) {
			for (j = i+1; j < crds->num; j++) {
				if (g->edge_flag[get_idx(i, j)]) {
					fprintf(pipe, "%f %f\n", crds->x[i], crds->y[i]);
					fprintf(pipe, "%f %f\n", crds->x[j], crds->y[j]);
					fprintf(pipe, "\n");
				}
			}
		}
		fprintf(pipe, "e\n");
	}

	/* plot vertices */
	for (i = 0; i < crds->num; i++) {
		fprintf(pipe, "%f %f\n", crds->x[i], crds->y[i]);
		fprintf(pipe, "\n");
	}
	fprintf(pipe, "e\n");

	for (i = 0; i < crds->num; i++) {
		fprintf(pipe, "%f %f %d", crds->x[i], crds->y[i], i);
		fprintf(pipe, "\n");
	}
	fprintf(pipe, "e\n");

	fflush(pipe);

	return 0;
}


int plot_tree(tree* t, coords* crds, char* title) {

#ifdef DEBUG
    if (t->vrtx_num != crds->num) {
        fprintf(stderr, "Error in /base/plot.c:\n"
                        "function: plot_tree:\n"
                        "plot_tree : errcode %d\n", 1);
        return -1;
    }
#endif

    int i;
	graph gtemp;
	graph_init(&gtemp);
	graph_setup(&gtemp, crds->num);

	for (i = 0; i < t->edge_num; i++) {
		graph_insert_edge(&gtemp, t->edges[i], 0.0, FREE);
	}

	plot_graph(&gtemp, crds, title);
	graph_delete(&gtemp);
	return 0;

}




