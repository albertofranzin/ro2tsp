#include "branchBoundUtils.h"

branchingInfo *createBranchingInfo(node *branchNode, node **editedNodes,
		double *deltas, int numEditedNodes){
	branchingInfo *bi = malloc(sizeof(branchingInfo *));
	//memset(bi, 0, sizeof(branchingInfo));

	bi->numEditedNodes = numEditedNodes;
	bi->branchNode = branchNode;
	bi->editedNodes = editedNodes;
	bi->deltas = deltas;

	return bi;
}

void deleteBranchingInfo(branchingInfo *bi) {
	free(bi->branchNode);
	free(bi->editedNodes);
	free(bi->deltas);
	free(bi);
}
