#include "cpx_mark_subtours.h"

int cpx_mark_subtours(cpx_table* hash_table, int* edge_indexes, int* edge_marks, int n) {
  int i, x, y, pos, start_vertex, next_vertex, mark, termination, num_of_subtours; 

  int* vertex_marks[n];
  int first_neighbour[n];
  int second_neighbour[n];

  for (i = 0; i < n; i++) {
    first_neighbour[i] = 0;
    second_neighbour[i] = 0;
  }

  for (i = 0; i < n; i++) { // per ogni nodo memorizzo i 2 nodi vicini (nota che i lati i cui indici (di var. cplex) sono contenuti in edge_indexes, formano un insieme di subtour)
    
    pos = edge_indexes[i];
    vertices_from_pos(hash_table, &x, &y, pos);

    if (first_neighbour[x-1] == 0)
      first_neighbour[x-1] = y;
    else
      second_neighbour[x-1] = y;

    if (first_neighbour[y-1] == 0)
      first_neighbour[y-1] = x;
    else
      second_neighbour[y-1] = x;
  }
   
  for (i = 0; i < n; i++) {
    vertex_marks[i] = 0;
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
    pos = edge_indexes[i];
    vertices_from_pos(hash_table, &x, &y, pos);
    edge_marks[i] = vertex_marks[x-1];
  }

  return num_of_subtours;
}





