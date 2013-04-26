NOMEFILE = test.c

PAR = -g -O0 -lm
# -g  : debug symbols
# -O3 : badass optimization
# -lm : linking to math lib math.h

#OPT = -ftree-vectorizer-verbose=2 -finline-functions -std=c99

CHECK = -Winline
# -Winline : warn if any problem in including inline functions occurs

all: test clean

test: graph.o egraph.o tree.o list.o compute_mst.o
	gcc graph.o egraph.o tree.o list.o compute_mst.o $(NOMEFILE) -o test $(PAR)

graph.o: graph.h graph.c
	gcc -g -c graph.c

egraph.o: egraph.h egraph.c
	gcc -g -c egraph.c

tree.o: tree.h tree.c
	gcc -g -c tree.c

list.o: list.h list.c
	gcc -g -c list.c

compute_mst.o: compute_mst.h compute_mst.c
	gcc -g -c compute_mst.c


clean:
	rm ./*o
	rm ./*~