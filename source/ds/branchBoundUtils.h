#ifndef __BRANCHBOUNDUTILS_H
#define __BRANCHBOUNDUTILS_H

#include "matrixGraph.h"

typedef struct _branchingInfo {
	node *branchNode;
	node **editedNodes;
	double *deltas;
	int numEditedNodes;
} branchingInfo;

branchingInfo *createBranchingInfo(node *, node **, double *, int);
void deleteBranchingInfo(branchingInfo *);

#endif