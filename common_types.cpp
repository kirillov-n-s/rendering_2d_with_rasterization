#include "common_types.h"

AdjacencyMat makeZeroAdjacency(const int nVertices) {
	AdjacencyMat adjacency;
	adjacency.reserve(nVertices);
	for (int vertexInd = 0; vertexInd < nVertices; ++vertexInd)
		adjacency.emplace_back(nVertices, false);
	return adjacency;
}
