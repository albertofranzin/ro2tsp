NOMEFILE = test.c

PAR = -g -O3 -lm
# -g  : debug symbols
# -O3 : badass optimization
# -lm : linking to math lib math.h

#OPT = -ftree-vectorizer-verbose=2 -finline-functions -std=c99

CHECK = -Winline
# -Winline : warn if any problem in including inline functions occurs

all: test clean

test: graph.o egraph.o tree.o onetree.o compute_mst.o compute_ot.o
	gcc graph.o egraph.o tree.o onetree.o compute_mst.o compute_ot.o $(NOMEFILE) -o test $(PAR)

graph.o: graph.h graph.c
	gcc -g -c graph.c

egraph.o: egraph.h egraph.c
	gcc -g -c egraph.c

tree.o: tree.h tree.c
	gcc -g -c tree.c

onetree.o: onetree.h onetree.c
	gcc -g -c onetree.c

compute_mst.o: compute_mst.h compute_mst.c
	gcc -g -c compute_mst.c

compute_ot.o: compute_ot.h compute_ot.c
	gcc -g -c compute_ot.c

clean:
	rm ./*o