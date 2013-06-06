#include "cpx_components.h"


int cpx_mark_components(cpx_env *ce,
                        double  *x,
			int     x_size,
                        int     *vrtx_comp,
			int     vrtx_comp_size,
                        int     *numcomp) 
{


  int n = ce->G.n;
  int vrtx_nn1[n];
  int vrtx_nn2[n];
  
  int i, j, k, idx, start, curr, my_comp;


  if (x_size != n * (n - 1) / 2 || vrtx_comp_size != n) return 1;


  for (i = 0; i < n; i++) {

	vrtx_comp[i] = -1;
	vrtx_nn1[i]  = -1;
	vrtx_nn2[i]  = -1;
  
  }


  k = 0;
  for (idx = 0; idx < x_size; idx++) {
	if (x[idx] > 0.9)  {

		vrtx_from_idx(&ce->T, &i, &j, idx);
		if (vrtx_nn1[i] == -1) vrtx_nn1[i] = j;
		else                   vrtx_nn2[i] = j;
		if (vrtx_nn1[j] == -1) vrtx_nn1[j] = i;
		else                   vrtx_nn2[j] = i;
		k++;

	}
  }


  if (k != n) return 1;


  start = my_comp = 0;
  while (start >= 0) {


	curr = vrtx_nn1[start];
	vrtx_comp[curr] = my_comp;

	curr = ( vrtx_nn1[curr] != start ) ?
			    vrtx_nn1[curr] :
			    vrtx_nn2[curr];
	vrtx_comp[curr] = my_comp;


	while (curr != start) {

		curr = ( vrtx_comp[ vrtx_nn1[curr] ] == -1 ) ?
					       vrtx_nn1[curr]:
					       vrtx_nn2[curr];
		vrtx_comp[curr] = my_comp;

	}

  
	start = -1;
	for (i = 0; i < n; i++) {
		if (vrtx_comp[i] == -1) {

			start = i;    
			break;

		}
	}


	if (start == -1) break;
	my_comp++;
  
  }


  *numcomp = my_comp + 1;
  return 0;

}

