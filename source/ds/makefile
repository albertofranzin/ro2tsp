NOMEFILE = test.c
PAR = -lm -g -O3 -g

test: clean buildLib $(NOMEFILE)
	gcc $(PAR) -o $@ $(NOMEFILE) -L. -ltest

buildLib: itemset.o set.o rbtree.o node.o edge.o matrixGraph.o utils.o
	ar r libtest.a *.o
#	rm *.o

itemset.o: itemset.h itemset.c
	gcc -c itemset.c

set.o: set.h set.c
	gcc -c set.c

rbtree.o: rbtree.h rbtree.c
	gcc -c rbtree.c

node.o: node.h node.c
	gcc -c node.c

edge.o: edge.h edge.c
	gcc -c edge.c

utils.o: utils.h utils.c
	gcc -c utils.c

matrixGraph.o: matrixGraph.h matrixGraph.c
	gcc -c matrixGraph.c

clean:
	clear
	-rm test
	-rm makefile~
	-rm test.a
	-rm *.o
	-rm *.gch
