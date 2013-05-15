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

CPLEX_COMPILER_OPT = -I/opt/ibm/ILOG/CPLEX_Studio125/cplex/include -DIL_STD

CPLEX_LINKER_OPT = -L/opt/ibm/ILOG/CPLEX_Studio125/cplex/lib/x86-64_sles10_4.1/static_pic -lcplex -lm -lpthread

all: clean test

test: cleanComp buildLib $(NOMEFILE)
	gcc $(NOMEFILE) -o $@ -L. -lro2ds $(CFLAGS) $(CHECK) $(CPLEX_COMPILER_OPT) $(CPLEX_LINKER_OPT) $(PAR)

release: CFLAGS += -O3
release: clean test
# -O3 : badass optimization

debug: CFLAGS += -g -DDEBUG
debug: clean test
# -g      : debug symbols
# -DDEBUG : declare DEBUG macro


# link libraries
buildLib: list.o elist.o vector.o lvector.o tree.o onetree.o graph.o egraph.o cycle.o \
	compute_mst.o compute_ot.o compute_lagrange.o compute_upper_bound.o \
	bb_input.o bb_output.o bb_stats.o bb_status.o bb_backup.o bb_constraints.o bb_select_node.o bb_select_edges.o bb_solver.o \
	cpx_table.o cpx_create_problem.o cpx_setup_problem.o cpx_add_sec.o cpx_mark_subtours.o cpx_solver.o \
	base.o utils.o cpx_callbacks.o
	ar r libro2ds.a *.o
	rm *.o

# data structures
list.o: data/list.h data/list.c
	gcc $(CFLAGS) $(PAR) -c data/list.c

elist.o: data/edge.h data/elist.h data/elist.c
	gcc $(CFLAGS) $(PAR) -c data/elist.c

vector.o: data/vector.h data/vector.c
	gcc $(CFLAGS) $(PAR) -c data/vector.c

lvector.o: data/lvector.h data/lvector.c
	gcc $(CFLAGS) $(PAR) -c data/lvector.c

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

# algorithms
compute_mst.o: algos/compute_mst.h algos/compute_mst.c
	gcc $(CFLAGS) $(PAR) -c algos/compute_mst.c

compute_ot.o: algos/compute_ot.h algos/compute_ot.c
	gcc $(CFLAGS) $(PAR) -c algos/compute_ot.c

compute_lagrange.o: algos/compute_lagrange.h algos/compute_lagrange.c
	gcc $(CFLAGS) $(PAR) -c algos/compute_lagrange.c

compute_upper_bound.o: algos/compute_upper_bound.h algos/compute_upper_bound.c
	gcc $(CFLAGS) $(PAR) -c algos/compute_upper_bound.c

# solvers: bb

bb_input.o: solvers/bb/bb_input.h solvers/bb/bb_input.c
	gcc $(CFLAGS) $(PAR) -c solvers/bb/bb_input.c

bb_output.o: solvers/bb/bb_output.h solvers/bb/bb_output.c
	gcc $(CFLAGS) $(PAR) -c solvers/bb/bb_output.c

bb_stats.o: solvers/bb/bb_stats.h solvers/bb/bb_stats.c
	gcc $(CFLAGS) $(PAR) -c solvers/bb/bb_stats.c

bb_status.o: solvers/bb/bb_status.h solvers/bb/bb_status.c
	gcc $(CFLAGS) $(PAR) -c solvers/bb/bb_status.c

bb_backup.o: solvers/bb/bb_backup.h solvers/bb/bb_backup.c
	gcc $(CFLAGS) $(PAR) -c solvers/bb/bb_backup.c

bb_select_node.o: solvers/bb/bb_select_node.h solvers/bb/bb_select_node.c
	gcc $(CFLAGS) $(PAR) -c solvers/bb/bb_select_node.c

bb_select_edges.o: solvers/bb/bb_select_edges.h solvers/bb/bb_select_edges.c
	gcc $(CFLAGS) $(PAR) -c solvers/bb/bb_select_edges.c

bb_constraints.o: solvers/bb/bb_constraints.h solvers/bb/bb_constraints.c
	gcc $(CFLAGS) $(PAR) -c solvers/bb/bb_constraints.c

bb_solver.o: solvers/bb/bb_solver.h solvers/bb/bb_solver.c
	gcc $(CFLAGS) $(PAR) -c solvers/bb/bb_solver.c

# solvers: cpx

cpx_table.o: solvers/cpx/cpx_table.h solvers/cpx/cpx_table.c
	gcc $(CFLAGS) $(PAR) -c solvers/cpx/cpx_table.c

cpx_create_problem.o: solvers/cpx/cpx_create_problem.h solvers/cpx/cpx_create_problem.c
	gcc $(CFLAGS) $(CPLEX_COMPILER_OPT) $(CPLEX_LINKER_OPT) $(PAR) -c solvers/cpx/cpx_create_problem.c

cpx_setup_problem.o: solvers/cpx/cpx_setup_problem.h solvers/cpx/cpx_setup_problem.c
	gcc $(CFLAGS) $(CPLEX_COMPILER_OPT) $(CPLEX_LINKER_OPT) $(PAR) -c solvers/cpx/cpx_setup_problem.c

cpx_add_sec.o: solvers/cpx/cpx_add_sec.h solvers/cpx/cpx_add_sec.c
	gcc $(CFLAGS) $(CPLEX_COMPILER_OPT) $(CPLEX_LINKER_OPT) $(PAR) -c solvers/cpx/cpx_add_sec.c

cpx_mark_subtours.o: solvers/cpx/cpx_mark_subtours.h solvers/cpx/cpx_mark_subtours.c
	gcc $(CFLAGS) $(CPLEX_COMPILER_OPT) $(CPLEX_LINKER_OPT) $(PAR) -c solvers/cpx/cpx_mark_subtours.c

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
