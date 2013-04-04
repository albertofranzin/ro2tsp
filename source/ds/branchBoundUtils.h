#ifndef __BRANCHBOUNDUTILS_H
#define __BRANCHBOUNDUTILS_H

#include "matrixGraph.h"



typedef struct _branchingInfo {
	node *branchNode;
	edge **editedEdges;
	double *deltas;
	int numEditedEdges;
	int deniedEdges;
	int imposedEdges;
} branchingInfo;

//const double BB_COST_DELTA = 100.0;


branchingInfo *createBranchingInfo();
void deleteBranchingInfo(branchingInfo *);
branchingInfo *cloneBranchingInfo(branchingInfo *);

node *chooseBranchingNode(matrixGraph *, edge **);

short isHamilton(matrixGraph *, edge **);

double sumDeltas(branchingInfo *);

short stoppingCriteria(matrixGraph *, edge **, double *, double *);

#endif