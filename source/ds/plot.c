#include "plot.h"

void plotGraph(matrixGraph *graph, edge **el, short onlyTree) {
	int i, j;

	FILE *pipe = popen("gnuplot -persist","w");
	fprintf(pipe, "set xrange [-0.010:1.010]\n");
	fprintf(pipe, "set yrange [-0.010:1.250]\n");
	fprintf(pipe, "set title '1-tree for the given graph'\n");
	fprintf(pipe, "set xlabel 'X'\n");
	fprintf(pipe, "set ylabel 'Y'\n");
	fprintf(pipe, "unset ytics\n");
	fprintf(pipe, "unset xtics\n");
	fprintf(pipe, "set style line 1 lt 1 lc rgb \"red\" lw 1\n");
	fprintf(pipe, "set style line 2 lt 1 lc rgb \"grey\" lw 1\n");

	//fprintf(pipe, "set terminal png\n");
	//fprintf(pipe, "set output 'graph.png'\n");

	if (onlyTree == 0) {
		// print all the graph
		// the 1-tree will be plotted twice, as all the nodes, but...
		node **nl = graph->nodeList;
		fprintf(pipe, "plot '-' us 1:2 with lines ls 2, '' us 1:2 with lines ls 1\n");
		for (i = 0; i < graph->no_of_nodes ; ++i) {
			for (j = 0; j < i; ++j) {
				fprintf(pipe, "%f %f\n", nl[i]->x, nl[i]->y);
				fprintf(pipe, "%f %f\n", nl[j]->x, nl[j]->y);
				fprintf(pipe, "\n");
			}
		}
		fprintf(pipe, "e\n");
	} else {
		// launch gnuplot the following cycle
		fprintf(pipe, "plot '-' us 1:2 with lines ls 1\n");
	}

	// print the 1-tree, possibly overlapping previously-plottes edges
	// (nodes will be re-drawn)
	for (i = 0; i < graph->no_of_nodes ; ++i) {
		fprintf(pipe, "%f %f\n", el[i]->u->x, el[i]->u->y);
		fprintf(pipe, "%f %f\n", el[i]->v->x, el[i]->v->y);
		fprintf(pipe, "\n");
	}
	fprintf(pipe, "e\n");
	fflush(pipe);

	/*fprintf(pipe, "set samples 40\n");
	fprintf(pipe, "set isosamples 40\n");
	fprintf(pipe, "set hidden3d\n");
	fprintf(pipe, "set xrange [-8.000:8.000]\n");
	fprintf(pipe, "set yrange [-8.000:8.000]\n");
	fprintf(pipe, "set zrange [-2.000:2.000]\n");
	fprintf(pipe, "set terminal png\n");
	//fprintf(pipe, "set output 'graph.png'\n");
	fprintf(pipe, "set title 'We are plotting from C'\n");
	fprintf(pipe, "set xlabel 'Label X'\n");
	fprintf(pipe, "set ylabel 'Label Y'\n");
	/* fprintf(pipe, "plot 'datafile.dat' using 1:2\n");*/
	//fprintf(pipe, "splot cos(x)+cos(y)\n");
	//close(pipe);
}