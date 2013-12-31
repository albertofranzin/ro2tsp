all: test clean

clean:
	rm test.o coords.o cycle.o environment.o graph.o parameters.o tree.o plot.o setup_parameters.o setup_problem.o utils.o prim_mst.o prim_1tree.o lagrange.o

test: test.o coords.o cycle.o environment.o graph.o parameters.o tree.o plot.o setup_parameters.o setup_problem.o utils.o prim_mst.o prim_1tree.o lagrange.o
	g++ -DDEBUG=1 -O0 -g -o test test.o coords.o cycle.o environment.o graph.o parameters.o tree.o plot.o setup_parameters.o setup_problem.o utils.o prim_mst.o prim_1tree.o lagrange.o

test.o:
	g++ -DDEBUG=1 -O0 -c -g test.c


#algos
prim_mst.o: ./algos/prim_mst.h ./algos/prim_mst.c
	g++ -DDEBUG=1 -O0 -c -g ./algos/prim_mst.c

prim_1tree.o: ./algos/prim_1tree.h ./algos/prim_1tree.c
	g++ -DDEBUG=1 -O0 -c -g ./algos/prim_1tree.c
	
lagrange.o: ./algos/prim_1tree.h ./algos/lagrange.c
	g++ -DDEBUG=1 -O0 -c -g ./algos/lagrange.c
	
#data
coords.o: ./data/coords.h ./data/coords.c
	g++ -DDEBUG=1 -O0 -c -g ./data/coords.c
	
cycle.o: ./data/cycle.h ./data/cycle.c
	g++ -DDEBUG=1 -O0 -c -g ./data/cycle.c
	
environment.o: ./data/environment.h ./data/environment.c
	g++ -DDEBUG=1 -O0 -c -g ./data/environment.c
	
graph.o: ./data/graph.h ./data/graph.c
	g++ -DDEBUG=1 -O0 -c -g ./data/graph.c
	
parameters.o: ./data/parameters.h ./data/parameters.c
	g++ -DDEBUG=1 -O0 -c -g ./data/parameters.c

tree.o: ./data/tree.h ./data/tree.c
	g++ -DDEBUG=1 -O0 -lm -c -g ./data/tree.c
	
#base
plot.o: ./base/plot.h ./base/plot.c
	g++ -DDEBUG=1 -O0 -c -g ./base/plot.c

setup_parameters.o: ./base/setup_parameters.h ./base/setup_parameters.c
	g++ -DDEBUG=1 -O0 -c -g ./base/setup_parameters.c
	
setup_problem.o: ./base/setup_problem.h ./base/setup_problem.c
	g++ -DDEBUG=1 -O0 -c -g ./base/setup_problem.c
	
utils.o: ./base/utils.h ./base/utils.c
	g++ -DDEBUG=1 -O0 -c -g ./base/utils.c