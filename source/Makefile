NOMEFILE = test.c

PAR = -lm
# -lm : linking to math lib math.h
#
#  #OPT = -ftree-vectorizer-verbose=2 -finline-functions -std=c99

CFLAGS = 
# debug options will be appended when asked by using 'make debug'
# release optimization will be added then command is 'make release'

CHECK = -Winline -Wall
# -Winline : warn if any problem in including inline functions occurs

#CPLEX_COMPILER_OPT = -I/home/ludo/Programmi/CPLEX/cplex/include -DIL_STD
CPLEX_COMPILER_OPT = -I/opt/ibm/ILOG/CPLEX_Studio125/cplex/include -DIL_STD

#CPLEX_LINKER_OPT = -L/home/ludo/Programmi/CPLEX/cplex/lib/x86-64_sles10_4.1/static_pic -lcplex -lm -lpthread
CPLEX_LINKER_OPT = -L/opt/ibm/ILOG/CPLEX_Studio125/cplex/lib/x86-64_sles10_4.1/static_pic -lcplex -lm -lpthread

all: clean test

test: buildLib $(NOMEFILE)
	gcc $(NOMEFILE) -o $@ -L. -lro2ds $(CFLAGS) $(CHECK) $(CPLEX_COMPILER_OPT) $(CPLEX_LINKER_OPT) $(PAR)
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
buildLib: list.o elist.o tree.o graph.o onetree.o egraph.o cycle.o \
	compute_min_st.o compute_min_ot.o compute_lagrange.o compute_upper_bound.o \
	compute_deltas.o bb_bkp.o bb_select_node.o \
	bb_select_edges.o bb_propagate.o bb_forbid_fat_edges.o bb_solver.o \
	base.o utils.o preprocessing.o cpx_callbacks.o \
	cpx_table.o cpx_create_problem.o cpx_setup_problem.o cpx_add_my_sec.o \
	cpx_add_secs.o cpx_add_kruskal_secs.o cpx_solver.o base.o utils.o \
	tsp_env.o tsp_stats.o
	ar r libro2ds.a *.o
	#rm *.o

#buildLib: list.o elist.o tree.o graph.o onetree.o egraph.o cycle.o \
#	compute_min_st.o compute_min_ot.o compute_lagrange.o compute_upper_bound.o \
#	compute_deltas.o bb_bkp.o bb_select_node.o bb_preprocessing.o \
#	bb_select_edges.o bb_propagate.o bb_forbid_fat_edges.o bb_solver.o \
#	cpx_table.o cpx_create_problem.o cpx_setup_problem.o cpx_add_my_sec.o \
#	cpx_add_secs.o cpx_add_kruskal_secs.o cpx_solver.o base.o utils.o \
#	tsp_env.o tsp_stats.o cpx_callbacks.o
#	ar r libro2ds.a *.o
#	#rm *.o

# data structures
list.o: data/list.h data/list.c
	gcc $(CFLAGS) $(PAR) -c data/list.c

elist.o: data/edge.h data/elist.h data/elist.c
	gcc $(CFLAGS) $(PAR) -c data/elist.c

tree.o: data/tree.h data/tree.c
	gcc $(CFLAGS) $(PAR) -c data/tree.c

onetree.o: data/onetree.h data/onetree.c
	gcc $(CFLAGS) $(PAR) -c data/onetree.c

graph.o: data/graph.h data/graph.c
	gcc $(CFLAGS) $(PAR) -c data/graph.c

egraph.o: data/egraph.h data/egraph.c
	gcc $(CFLAGS) $(PAR) -c data/egraph.c

cycle.o: data/cycle.h data/cycle.c
	gcc $(CFLAGS) $(PAR) -c data/cycle.c

tsp_env.o: data/tsp_env.h data/tsp_env.c
	gcc $(CFLAGS) $(PAR) -c data/tsp_env.c

tsp_stats.o: data/tsp_stats.h data/tsp_stats.c
	gcc $(CFLAGS) $(PAR) -c data/tsp_stats.c

# algorithms
compute_min_st.o: algos/compute_min_st.h algos/compute_min_st.c
	gcc $(CFLAGS) $(PAR) -c algos/compute_min_st.c

compute_min_ot.o: algos/compute_min_ot.h algos/compute_min_ot.c
	gcc $(CFLAGS) $(PAR) -c algos/compute_min_ot.c

compute_lagrange.o: algos/compute_lagrange.h algos/compute_lagrange.c
	gcc $(CFLAGS) $(PAR) -c algos/compute_lagrange.c

compute_upper_bound.o: algos/compute_upper_bound.h algos/compute_upper_bound.c
	gcc $(CFLAGS) $(PAR) -c algos/compute_upper_bound.c

compute_deltas.o: algos/compute_deltas.h algos/compute_deltas.c
	gcc $(CFLAGS) $(PAR) -c algos/compute_deltas.c

preprocessing.o: algos/preprocessing.h algos/preprocessing.c
	gcc $(CFLAGS) $(PAR) -c algos/preprocessing.c

# solvers: bb

bb_bkp.o: solvers/bb/bb_bkp.h solvers/bb/bb_bkp.c
	gcc $(CFLAGS) $(PAR) -c solvers/bb/bb_bkp.c

bb_select_node.o: solvers/bb/bb_select_node.h solvers/bb/bb_select_node.c
	gcc $(CFLAGS) $(PAR) -c solvers/bb/bb_select_node.c

bb_select_edges.o: solvers/bb/bb_select_edges.h solvers/bb/bb_select_edges.c
	gcc $(CFLAGS) $(PAR) -c solvers/bb/bb_select_edges.c

bb_propagate.o: solvers/bb/bb_propagate.h solvers/bb/bb_propagate.c
	gcc $(CFLAGS) $(PAR) -c solvers/bb/bb_propagate.c

bb_forbid_fat_edges.o: solvers/bb/bb_forbid_fat_edges.h solvers/bb/bb_forbid_fat_edges.c
	gcc $(CFLAGS) $(PAR) -c solvers/bb/bb_forbid_fat_edges.c

bb_solver.o: solvers/bb/bb_solver.h solvers/bb/bb_solver.c
	gcc $(CFLAGS) $(PAR) -c solvers/bb/bb_solver.c

# solvers: cpx

cpx_table.o: solvers/cpx/cpx_table.h solvers/cpx/cpx_table.c
	gcc $(CFLAGS) $(PAR) -c solvers/cpx/cpx_table.c

cpx_create_problem.o: solvers/cpx/cpx_create_problem.h solvers/cpx/cpx_create_problem.c
	gcc $(CFLAGS) $(CPLEX_COMPILER_OPT) $(CPLEX_LINKER_OPT) $(PAR) -c solvers/cpx/cpx_create_problem.c

cpx_setup_problem.o: solvers/cpx/cpx_setup_problem.h solvers/cpx/cpx_setup_problem.c
	gcc $(CFLAGS) $(CPLEX_COMPILER_OPT) $(CPLEX_LINKER_OPT) $(PAR) -c solvers/cpx/cpx_setup_problem.c

cpx_add_my_sec.o: solvers/cpx/cpx_add_my_sec.h solvers/cpx/cpx_add_my_sec.c
	gcc $(CFLAGS) $(CPLEX_COMPILER_OPT) $(CPLEX_LINKER_OPT) $(PAR) -c solvers/cpx/cpx_add_my_sec.c

cpx_add_secs.o: solvers/cpx/cpx_add_secs.h solvers/cpx/cpx_add_secs.c
	gcc $(CFLAGS) $(CPLEX_COMPILER_OPT) $(CPLEX_LINKER_OPT) $(PAR) -c solvers/cpx/cpx_add_secs.c

cpx_add_kruskal_secs.o: solvers/cpx/cpx_add_kruskal_secs.h solvers/cpx/cpx_add_kruskal_secs.c
	gcc $(CFLAGS) $(CPLEX_COMPILER_OPT) $(CPLEX_LINKER_OPT) $(PAR) -c solvers/cpx/cpx_add_kruskal_secs.c

cpx_solver.o: solvers/cpx/cpx_solver.h solvers/cpx/cpx_solver.c
	gcc $(CFLAGS) $(CPLEX_COMPILER_OPT) $(CPLEX_LINKER_OPT) $(PAR) -c solvers/cpx/cpx_solver.c

cpx_callbacks.o: solvers/cpx/cpx_callbacks.h solvers/cpx/cpx_callbacks.c
	gcc $(CFLAGS) $(CPLEX_COMPILER_OPT) $(CPLEX_LINKER_OPT) $(PAR) -c solvers/cpx/cpx_callbacks.c

# base (common utilities)
base.o: base/base.h base/base.c
	gcc $(CFLAGS) $(PAR) -c base/base.c

utils.o: base/utils.h base/utils.c
	gcc $(CFLAGS) $(PAR) -c base/utils.c

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
