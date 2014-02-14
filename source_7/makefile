all: test clean

clean:
	rm test.o arraylist.o coords.o environment.o graph.o list.o parameters.o set.o statistics.o tree.o plot.o setup_parameters.o setup_problem.o utils.o compare_sort.o counting_sort.o radix_sort.o reduce.o pr_mst.o kr_mst.o pr_onetree.o kr_onetree.o kr_lagrange_vj.o pr_lagrange_hk.o pr_lagrange_vj.o edgesel.o vertexsel.o pr_bb.o kr_bb.o

test: test.o arraylist.o coords.o environment.o graph.o list.o parameters.o set.o statistics.o tree.o plot.o setup_parameters.o setup_problem.o utils.o compare_sort.o counting_sort.o radix_sort.o reduce.o pr_mst.o kr_mst.o pr_onetree.o kr_onetree.o kr_lagrange_vj.o pr_lagrange_hk.o pr_lagrange_vj.o edgesel.o vertexsel.o pr_bb.o kr_bb.o
	g++ -DDEBUG=1 -O0 -g -o test test.o arraylist.o coords.o environment.o graph.o list.o parameters.o set.o statistics.o tree.o plot.o setup_parameters.o setup_problem.o utils.o compare_sort.o counting_sort.o radix_sort.o reduce.o pr_mst.o kr_mst.o pr_onetree.o kr_onetree.o kr_lagrange_vj.o pr_lagrange_hk.o pr_lagrange_vj.o edgesel.o vertexsel.o pr_bb.o kr_bb.o

test.o:
	g++ -DDEBUG=1 -O0 -c -g test.c


#algos
compare_sort.o: ./algos/compare_sort.h ./algos/compare_sort.c
	g++ -DDEBUG=1 -O0 -c -g ./algos/compare_sort.c

counting_sort.o: ./algos/counting_sort.h ./algos/counting_sort.c
	g++ -DDEBUG=1 -O0 -c -g ./algos/counting_sort.c

radix_sort.o: ./algos/radix_sort.h ./algos/radix_sort.c
	g++ -DDEBUG=1 -O0 -c -g ./algos/radix_sort.c

reduce.o: ./algos/reduce.h ./algos/reduce.c
	g++ -DDEBUG=1 -O0 -c -g ./algos/reduce.c
	
pr_mst.o: ./algos/pr_mst.h ./algos/pr_mst.c
	g++ -DDEBUG=1 -O0 -c -g ./algos/pr_mst.c

kr_mst.o: ./algos/kr_mst.h ./algos/kr_mst.c
	g++ -DDEBUG=1 -O0 -c -g ./algos/kr_mst.c

pr_onetree.o: ./algos/pr_onetree.h ./algos/pr_onetree.c
	g++ -DDEBUG=1 -O0 -c -g ./algos/pr_onetree.c

kr_onetree.o: ./algos/kr_onetree.h ./algos/kr_onetree.c
	g++ -DDEBUG=1 -O0 -c -g ./algos/kr_onetree.c

kr_lagrange_vj.o: ./algos/kr_lagrange_vj.h ./algos/kr_lagrange_vj.c
	g++ -DDEBUG=1 -O0 -c -g ./algos/kr_lagrange_vj.c

pr_lagrange_hk.o: ./algos/pr_lagrange_hk.h ./algos/pr_lagrange_hk.c
	g++ -DDEBUG=1 -O0 -c -g ./algos/pr_lagrange_hk.c

pr_lagrange_vj.o: ./algos/pr_lagrange_vj.h ./algos/pr_lagrange_vj.c
	g++ -DDEBUG=1 -O0 -c -g ./algos/pr_lagrange_vj.c
												
#data
arraylist.o: ./data/arraylist.h ./data/arraylist.c
	g++ -DDEBUG=1 -O0 -c -g ./data/arraylist.c
	
coords.o: ./data/coords.h ./data/coords.c
	g++ -DDEBUG=1 -O0 -c -g ./data/coords.c
	
environment.o: ./data/environment.h ./data/environment.c
	g++ -DDEBUG=1 -O0 -c -g ./data/environment.c
	
graph.o: ./data/graph.h ./data/graph.c
	g++ -DDEBUG=1 -O0 -c -g ./data/graph.c

list.o: ./data/list.h ./data/list.c
	g++ -DDEBUG=1 -O0 -c -g ./data/list.c
		
parameters.o: ./data/parameters.h ./data/parameters.c
	g++ -DDEBUG=1 -O0 -c -g ./data/parameters.c
	
set.o: ./data/set.h ./data/set.c
	g++ -DDEBUG=1 -O0 -c -g ./data/set.c
		
statistics.o: ./data/statistics.h ./data/statistics.c
	g++ -DDEBUG=1 -O0 -c -g ./data/statistics.c
	
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
	
#solvers
edgesel.o: ./solvers/edgesel.h ./solvers/edgesel.c
	g++ -DDEBUG=1 -O0 -c -g ./solvers/edgesel.c
	
vertexsel.o: ./solvers/vertexsel.h ./solvers/vertexsel.c
	g++ -DDEBUG=1 -O0 -c -g ./solvers/vertexsel.c
	
pr_bb.o: ./solvers/pr_bb.h ./solvers/pr_bb.c
	g++ -DDEBUG=1 -O0 -c -g ./solvers/pr_bb.c
	
kr_bb.o: ./solvers/kr_bb.h ./solvers/kr_bb.c
	g++ -DDEBUG=1 -O0 -c -g ./solvers/kr_bb.c