NOMEFILE = test.c

CHECK = -Winline -Wall
# -Winline : warn if any problem in including inline functions occurs

CPLEX_COMPILER_OPT = -I/opt/ibm/ILOG/CPLEX_Studio125/cplex/include -DIL_STD

CPLEX_LINKER_OPT = -L/opt/ibm/ILOG/CPLEX_Studio125/cplex/lib/x86-64_sles10_4.1/static_pic -lcplex -lm -lpthread

PAR = -lm -lpthread
# -lm : linking to math lib math.h
#  OPT = -ftree-vectorizer-verbose=2 -finline-functions -std=c99

CFLAGS = -L. -lro2ds -lconcorde $(CHECK) $(CPLEX_COMPILER_OPT) $(CPLEX_LINKER_OPT) $(PAR)
# debug options will be appended when asked by using 'make debug'
# release optimization will be added then command is 'make release'

all: test


test: buildLib $(NOMEFILE)
	gcc $(NOMEFILE) -o $@ $(CFLAGS)
# cleanComp

release: CFLAGS += -O3
release: clean test
# -O3 : badass optimization

debug: CFLAGS += -g -DDEBUG -O3
debug:  test
# clean
# -g      : debug symbols
# -DDEBUG : declare DEBUG macro

# link libraries
buildLib: arraylist.o coords.o cycle.o environment.o graph.o list.o parameters.o set.o statistics.o tree.o \
	plot.o setup_parameters.o setup_problem.o utils.o \
	compare_sort.o counting_sort.o radix_sort.o \
	reduce.o \
	heur_nn.o heur_rc.o heur_2opt.o heur_3opt.o heur_nn2opt.o heur_rc2opt.o compute_ub.o \
	pr_mst.o pr_onetree.o pr_lagrange_hk.o pr_lagrange_vj.o \
	kr_mst.o kr_onetree.o kr_lagrange_vj.o \
	propagate.o setncheck.o edgesel.o vertexsel.o pr_bb.o kr_bb.o
	ar r libro2ds.a *.o
	rm *.o

# data structures
arraylist.o: data/arraylist.h data/arraylist.c
	gcc $(CFLAGS) $(PAR) -c data/arraylist.c

coords.o: data/coords.h data/coords.c
	gcc $(CFLAGS) $(PAR) -c data/coords.c

cycle.o: data/cycle.h data/cycle.c
	gcc $(CFLAGS) $(PAR) -c data/cycle.c
	
environment.o: data/environment.h data/environment.c
	gcc $(CFLAGS) $(PAR) -c data/environment.c

graph.o: data/graph.h data/graph.c
	gcc $(CFLAGS) $(PAR) -c data/graph.c

list.o: data/list.h data/list.c
	gcc $(CFLAGS) $(PAR) -c data/list.c

parameters.o: data/parameters.h data/parameters.c
	gcc $(CFLAGS) $(PAR) -c data/parameters.c

set.o: data/set.h data/set.c
	gcc $(CFLAGS) $(PAR) -c data/set.c

statistics.o: data/statistics.h data/statistics.c
	gcc $(CFLAGS) $(PAR) -c data/statistics.c

tree.o: data/tree.h data/tree.c
	gcc $(CFLAGS) $(PAR) -c data/tree.c

# algorithms
heur_nn.o: algos/heur_nn.h algos/heur_nn.c
	gcc $(CFLAGS) $(PAR) -c algos/heur_nn.c

heur_rc.o: algos/heur_rc.h algos/heur_rc.c
	gcc $(CFLAGS) $(PAR) -c algos/heur_rc.c
		
heur_2opt.o: algos/heur_2opt.h algos/heur_2opt.c
	gcc $(CFLAGS) $(PAR) -c algos/heur_2opt.c

heur_3opt.o: algos/heur_3opt.h algos/heur_3opt.c
	gcc $(CFLAGS) $(PAR) -c algos/heur_3opt.c
	
heur_nn2opt.o: algos/heur_nn2opt.h algos/heur_nn2opt.c
	gcc $(CFLAGS) $(PAR) -c algos/heur_nn2opt.c
	
heur_rc2opt.o: algos/heur_rc2opt.h algos/heur_rc2opt.c
	gcc $(CFLAGS) $(PAR) -c algos/heur_rc2opt.c
	
compute_ub.o: algos/compute_ub.h algos/compute_ub.c
	gcc $(CFLAGS) $(PAR) -c algos/compute_ub.c
	
compare_sort.o: algos/compare_sort.h algos/compare_sort.c
	gcc $(CFLAGS) $(PAR) -c algos/compare_sort.c

counting_sort.o: algos/counting_sort.h algos/counting_sort.c
	gcc $(CFLAGS) $(PAR) -c algos/counting_sort.c

radix_sort.o: algos/radix_sort.h algos/radix_sort.c
	gcc $(CFLAGS) $(PAR) -c algos/radix_sort.c

reduce.o: algos/reduce.h algos/reduce.c
	gcc $(CFLAGS) $(PAR) -c algos/reduce.c

pr_mst.o: algos/pr_mst.h algos/pr_mst.c
	gcc $(CFLAGS) $(PAR) -c algos/pr_mst.c

pr_onetree.o: algos/pr_onetree.h algos/pr_onetree.c
	gcc $(CFLAGS) $(PAR) -c algos/pr_onetree.c

pr_lagrange_hk.o: algos/pr_lagrange_hk.h algos/pr_lagrange_hk.c
	gcc $(CFLAGS) $(PAR) -c algos/pr_lagrange_hk.c
	
pr_lagrange_vj.o: algos/pr_lagrange_vj.h algos/pr_lagrange_vj.c
	gcc $(CFLAGS) $(PAR) -c algos/pr_lagrange_vj.c
	
kr_mst.o: algos/kr_mst.h algos/kr_mst.c
	gcc $(CFLAGS) $(PAR) -c algos/kr_mst.c

kr_onetree.o: algos/kr_onetree.h algos/kr_onetree.c
	gcc $(CFLAGS) $(PAR) -c algos/kr_onetree.c
	
kr_lagrange_vj.o: algos/kr_lagrange_vj.h algos/kr_lagrange_vj.c
	gcc $(CFLAGS) $(PAR) -c algos/kr_lagrange_vj.c
	
		
# solvers: bb
setncheck.o: solvers/bb/setncheck.h solvers/bb/setncheck.c
	gcc $(CFLAGS) $(PAR) -c solvers/bb/setncheck.c
	
propagate.o: solvers/bb/propagate.h solvers/bb/propagate.c
	gcc $(CFLAGS) $(PAR) -c solvers/bb/propagate.c

edgesel.o: solvers/bb/edgesel.h solvers/bb/edgesel.c
	gcc $(CFLAGS) $(PAR) -c solvers/bb/edgesel.c

vertexsel.o: solvers/bb/vertexsel.h solvers/bb/vertexsel.c
	gcc $(CFLAGS) $(PAR) -c solvers/bb/vertexsel.c

pr_bb.o: solvers/bb/pr_bb.h solvers/bb/pr_bb.c
	gcc $(CFLAGS) $(PAR) -c solvers/bb/pr_bb.c

kr_bb.o: solvers/bb/kr_bb.h solvers/bb/kr_bb.c
	gcc $(CFLAGS) $(PAR) -c solvers/bb/kr_bb.c
	
# solvers: cpx

# base (common utilities)
plot.o: base/plot.h base/plot.c
	gcc $(CFLAGS) $(PAR) -c base/plot.c

utils.o: base/utils.h base/utils.c
	gcc $(CFLAGS) $(PAR) -c base/utils.c
	
setup_parameters.o: base/setup_parameters.h base/setup_parameters.c
	gcc $(CFLAGS) $(PAR) -c base/setup_parameters.c
	
setup_problem.o: base/setup_problem.h base/setup_problem.c
	gcc $(CFLAGS) $(PAR) -c base/setup_problem.c
	
# cleanup
cleanComp:
	clear
	-rm test
	-rm test.a
	-rm *.gch

clean:
	clear
	-rm test
	-rm test.a
	-rm *.o
	-rm *.gch