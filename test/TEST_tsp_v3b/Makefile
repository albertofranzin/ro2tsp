NOMEFILE = test.c

PAR = -g -O0 -lm
# -g  : debug symbols
# -O3 : badass optimization
# -lm : linking to math lib math.h

#OPT = -ftree-vectorizer-verbose=2 -finline-functions -std=c99

CHECK = -Winline
# -Winline : warn if any problem in including inline functions occurs

all: test clean

test: graph.o egraph.o tree.o onetree.o list.o elist.o vector.o lvector.o compute_mst.o compute_ot.o compute_lagrange.o compute_nearest_neighbour.o compute_upper_bound.o tsp_input.o tsp_output.o tsp_stats.o tsp_status.o tsp_backup.o tsp_select_node.o tsp_select_edges.o tsp_constraints.o tsp_solve.o
	gcc graph.o egraph.o tree.o onetree.o list.o elist.o vector.o lvector.o compute_mst.o compute_ot.o compute_lagrange.o compute_nearest_neighbour.o compute_upper_bound.o tsp_input.o tsp_output.o tsp_stats.o tsp_status.o tsp_backup.o tsp_select_node.o tsp_select_edges.o tsp_constraints.o tsp_solve.o $(NOMEFILE) -o test $(PAR)

graph.o: graph.h graph.c
	gcc -g -c graph.c

egraph.o: egraph.h egraph.c
	gcc -g -c egraph.c

tree.o: tree.h tree.c
	gcc -g -c tree.c

onetree.o: onetree.h onetree.c
	gcc -g -c onetree.c

list.o: list.h list.c
	gcc -g -c list.c

elist.o: elist.h elist.c
	gcc -g -c elist.c

vector.o: vector.h vector.c
	gcc -g -c vector.c

lvector.o: lvector.h lvector.c
	gcc -g -c lvector.c

compute_mst.o: compute_mst.h compute_mst.c
	gcc -g -c compute_mst.c

compute_ot.o: compute_ot.h compute_ot.c
	gcc -g -c compute_ot.c

compute_lagrange.o: compute_lagrange.h compute_lagrange.c
	gcc -g -c compute_lagrange.c

compute_nearest_neighbour.o: compute_nearest_neighbour.h compute_nearest_neighbour.c
	gcc -g -c compute_nearest_neighbour.c

compute_upper_bound.o: compute_upper_bound.h compute_upper_bound.c
	gcc -g -c compute_upper_bound.c

tsp_input.o: tsp_input.h tsp_input.c
	gcc -g -c tsp_input.c

tsp_output.o: tsp_output.h tsp_output.c
	gcc -g -c tsp_output.c

tsp_stats.o: tsp_stats.h tsp_stats.c
	gcc -g -c tsp_stats.c

tsp_stauts.o: tsp_status.h tsp_status.c
	gcc -g -c tsp_status.c

tsp_backup.o: tsp_backup.h tsp_backup.c
	gcc -g -c tsp_backup.c

tsp_select_node.o: tsp_select_node.h tsp_select_node.c
	gcc -g -c tsp_select_node.c

tsp_select_edges.o: tsp_select_edges.h tsp_select_edges.c
	gcc -g -c tsp_select_edges.c

tsp_constraints.o: tsp_constraints.h tsp_constraints.c
	gcc -g -c tsp_constraints.c

tsp_solve.o: tsp_solve.h tsp_solve.c
	gcc -g -c tsp_solve.c

clean:
	rm ./*o
	rm ./*~