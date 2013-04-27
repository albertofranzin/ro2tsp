NOMEFILE = test.c

PAR = -g -O0 -lm
# -g  : debug symbols
# -O3 : badass optimization
# -lm : linking to math lib math.h

#OPT = -ftree-vectorizer-verbose=2 -finline-functions -std=c99

CHECK = -Winline
# -Winline : warn if any problem in including inline functions occurs

all: test clean

test: graph.o egraph.o tree.o onetree.o compute_nearest_neighbour.o compute_upper_bound.o
	gcc graph.o egraph.o tree.o onetree.o compute_nearest_neighbour.o compute_upper_bound.o $(NOMEFILE) -o test $(PAR)

graph.o: graph.h graph.c
	gcc -g -c graph.c

egraph.o: egraph.h egraph.c
	gcc -g -c egraph.c

tree.o: tree.h tree.c
	gcc -g -c tree.c

onetree.o: onetree.h onetree.c
	gcc -g -c onetree.c

compute_nearest_neighbour.o: compute_nearest_neighbour.h compute_nearest_neighbour.c
	gcc -g -c compute_nearest_neighbour.c

compute_upper_bound.o: compute_upper_bound.h compute_upper_bound.c
	gcc -g -c compute_upper_bound.c

clean:
	rm ./*o
	rm ./*~