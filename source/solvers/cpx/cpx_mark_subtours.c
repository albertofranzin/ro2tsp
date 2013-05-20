#include "cpx_mark_subtours.h"

/**
 * mark every subtour with a different label
 * @param  hash_table   hash table to retrieve edge position in the arrays
 *                        given their nodes, and viceversa
 * @param  edge_indices array of edge indices
 * @param  edge_marks   array of edge marks
 * @param  n            number of elements in the arrays
 * @return              number of subtours found
 */
int cpx_mark_subtours(cpx_table *hash_table,
                      int       *edge_indices,
                      int       *edge_marks,
                      int        n)
{
  // iteration controls, vertex coordinates
  int i, x, y, pos,
      start_vertex,
      next_vertex,
      mark,
      termination,
      num_of_subtours = 0;

  // markers
  int vertex_marks[n];
  int first_neighbour[n];
  int second_neighbour[n];

  memset(vertex_marks, 0, sizeof(vertex_marks));
  memset(first_neighbour, 0, sizeof(first_neighbour));
  memset(second_neighbour, 0, sizeof(second_neighbour));

  // per ogni nodo memorizzo i 2 nodi vicini
  // (nota che i lati i cui indici (di var. cplex)
  // sono contenuti in edge_indices, formano un insieme di subtour)
  for (i = 0; i < n; ++i) {

    pos = edge_indices[i];
    vertices_from_pos(hash_table, &x, &y, pos);
    //printf("%d: %d (%d %d)\n", i+1, edge_indices[i], x, y);

    if (first_neighbour[x-1] == 0)
      first_neighbour[x-1] = y;
    else
      second_neighbour[x-1] = y;

    if (first_neighbour[y-1] == 0)
      first_neighbour[y-1] = x;
    else
      second_neighbour[y-1] = x;
  }


  start_vertex = 1;
  mark = 1;
  termination = FALSE;

  while (!termination) {
    num_of_subtours = mark;

    // marca il secondo vertice del subtour
    next_vertex = first_neighbour[start_vertex-1];
    vertex_marks[next_vertex-1] = mark;

    // marca il terzo vertice del subtour
    next_vertex = (first_neighbour[next_vertex-1] != start_vertex) ? first_neighbour[next_vertex-1] : second_neighbour[next_vertex-1];
    vertex_marks[next_vertex-1] = mark;



    // marca i vertici successivi
    next_vertex = (vertex_marks[ first_neighbour[next_vertex-1]-1 ] == 0) ? first_neighbour[next_vertex-1] : second_neighbour[next_vertex-1];
    while(next_vertex != start_vertex) { //marco i vertici successivi
      vertex_marks[next_vertex-1] = mark;
      next_vertex = (vertex_marks[ first_neighbour[next_vertex-1]-1 ] == 0) ? first_neighbour[next_vertex-1] : second_neighbour[next_vertex-1];
    }

    // marca il primo vertice del subtour
    vertex_marks[start_vertex-1] = mark;

    // cerca il primo nodo di un subtour non ancora marcato
    start_vertex = 0;
    for (i = 0; i < n; i++) {
      if(vertex_marks[i] == 0) {
        start_vertex = i+1;
        break;
      }
    }

    if (start_vertex > 0) {
      mark++;
    }
    else {
      termination = TRUE;
    }

  }

  // vertici adiacenti hanno uno stesso mark
  // associa il mark di due vertici adiacenti al lato che li collega
  for (i = 0; i < n; i++) {
    pos = edge_indices[i];
    vertices_from_pos(hash_table, &x, &y, pos);
    edge_marks[i] = vertex_marks[x-1];
  }

  return num_of_subtours;
}


/**
 * mark every subtour with a different label,
 * using an alternative way suggested by a Kruskal-like procedure
 * @param  env          CPLEX environment
 * @param  lp           problem
 * @param  G            graph
 * @param  hash_table   hash table to retrieve edge position in the arrays
 *                        given their nodes, and viceversa
 * @param  n            number of elements in the arrays
 * @return              number of subtours found
 */
int cpx_mark_subtours_the_kruskal_way(CPXENVptr  env,
                                      CPXLPptr   lp,
                                      graph     *G,
                                      cpx_table *hash_table,
                                      int        n)
{
  // iteration controls, coordinates, marks
  int i,
      j,
      k,
      x,
      y,
      pos,
      mark,
      mark1,
      mark2,
      status,
      min_index,
      num_of_subtours = 1;

  // marker arrays, neighbours
  int marked[n],
      edge_marks[n],
      edge_indices[n],
      vertex_marks[n],
      predecessors[n],
      first_neighbour[n],
      second_neighbour[n];

  // costs
  double min,
         cost;

  memset(marked, 0, sizeof(marked));
  memset(edge_indices, 0, sizeof(edge_indices));
  memset(predecessors, 0, sizeof(predecessors));
  memset(first_neighbour, 0, sizeof(first_neighbour));
  memset(second_neighbour, 0, sizeof(second_neighbour));

  // 1-tree
  onetree OT;
  onetree_init(&OT, n);
  compute_ot(G, &OT);

  mark = 1;

  for (i = 0; i < n; ++i) {
    edge_marks[i]   = i + 1;
    vertex_marks[i] = i + 1;

    pos_from_vertices(hash_table,
                      i+1,
                      onetree_get_pred(&OT, i+1),
                      &edge_indices[i]);

#ifdef DEBUG
    printf("%d ", edge_indices[i]);
#endif
  }

  printf("\n");

  for(i = 1 ; i <= OT.n - 2 ; i++) {
    //memset(marked, 0, sizeof(marked));

#ifdef DEBUG
    printf("i = %d\n", i);
#endif

    min       = BIG;
    min_index = -1;

    // look for min-cost, non-selected edge on the 1-tree
    for (j = 1; j <= OT.n; ++j) {
      // printf("-- j = %d\n", j);
      if (i != j && marked[j-1] == 0) {
        /**/printf("%d ", j);fflush(stdout);
        printf("%d, %d,%d \n", onetree_get_pred(&OT, j), vertex_marks[j-1], vertex_marks[onetree_get_pred(&OT, j)-1]);/**/
        cost = onetree_get_edge_cost(&OT, j, onetree_get_pred(&OT, j));
        // printf("cost of edge (%d, %d): %f\n", j, onetree_get_pred(OT, j), cost);
        if (cost < min) {
          min_index = j;
          min       = cost;
        }
      }
    }

    // take out the so found edge
    marked[min_index - 1] = 1;
    //vertex_marks[min_index - 1] = mark;
    mark1 = vertex_marks[min_index - 1];
    mark2 = vertex_marks[onetree_get_pred(&OT, min_index)-1];
    for (j = 0; j < n; ++j) {
      if (vertex_marks[j] == mark2) {
        printf("node %d has just changed its mark from %d to %d\n", j, mark1, mark2);
        vertex_marks[j] = mark1;

#ifdef DEBUG
        printf("marks: ");
        for (k = 0; k < n; ++k) {
          printf("%d ", vertex_marks[k]);
        }
        printf("\n");
#endif

        char ch = getchar();
      }
    }

    // vertici adiacenti hanno uno stesso mark
    // associa il mark di due vertici adiacenti al lato che li collega
    for (j = 0; j < n; j++) {
      pos = edge_indices[j];
      //printf("### %d\n", pos);
      vertices_from_pos(hash_table, &x, &y, pos);
      edge_marks[j] = vertex_marks[x-1];
    }

    // memset(marked, 0, sizeof(marked));

    for (k = 0; k < n; ++k) {
      if (marked[vertex_marks[k]] != 0) {
        continue;
      } else {
        marked[vertex_marks[k]]++;
      }

#ifdef DEBUG
      printf("mark = %d\n", vertex_marks[k]);
#endif

/////////////////////////////////////////////////
    int subtour_length, my_mark =vertex_marks[k];
    #ifdef DEBUG
      printf("\nTour has:\n");
    #endif
      subtour_length = 0;
      for (j = 0; j < n; j++) {
        if (edge_marks[j] == my_mark) {
          subtour_length++;

    #ifdef DEBUG
          printf("%d ", j);
    #endif

        }
      }
    #ifdef DEBUG
      printf("\n");
    #endif

      if (subtour_length > 2) {
        int rmatind[subtour_length];
        int kk = 0;
        for (j = 0; j < n; j++) {
          // aggiungo al sec solo i lati con mark uguale a my_mark
          // (cioè i lati appartenenti al subtour associato a my_mark)
          if (edge_marks[j] == my_mark) {
            rmatind[kk] = edge_indices[j]-1;
            // indici dei lati partono da 0 dentro cplex
            kk++;
          }
        }


        double rmatval[subtour_length];
        for (j = 0; j < subtour_length; j++) {
          rmatval[j] = 1.0;
        }

        int rmatbeg[1];
        rmatbeg[0] = 0;

        double rhs[1];
        rhs[0] = subtour_length-1;

        char sense[1];
        sense[0] = 'L';

        /*status = CPXaddrows(env, lp, 0, 1, subtour_length,
                            rhs, sense, rmatbeg, rmatind, rmatval,
                            NULL, NULL);*/

        status = CPXaddrows(env, lp, 0, 1, subtour_length,
                                   &rhs, &sense, &rmatbeg, &rmatind,
                                   &rmatval, NULL, NULL);
        if (status) {
          fprintf(stderr, "Fatal error in solvers/cpx/cpx_add_sec.c ::\n");
          fprintf(stderr, " CPXcutcallbackadd : %d\n", status);
          fprintf(stderr, "Error while inserting a new constraint.\n");
          exit(1);
        } else {
          printf("CONSTRAINT ADDED\n");
          char ch = getchar();
        }

      }
  ////////////////////////////////////////////////////////////////
    }

  } // end for i

  onetree_delete(&OT);

  return status;
}
