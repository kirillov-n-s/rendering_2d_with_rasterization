#include "common_types.h"

AdjacencyMat makeZeroAdjacency(const int nVertices) {
	AdjacencyMat adjacency;
	adjacency.reserve(nVertices);
	for (int vertexInd = 0; vertexInd < nVertices; ++vertexInd)
		adjacency.emplace_back(nVertices, false);
	return adjacency;
}

AdjacencyMat modifyAdjacency(
	const AdjacencyMat& adjacency,
	const AdjacencyVec& modVec,
	const std::function<bool(bool, bool)>& reduceFn)
{
	AdjacencyMat newAdjacency = adjacency;
	const int nVertices = newAdjacency.size();
	for (int vertexInd = 0; vertexInd < nVertices; ++vertexInd) {

		AdjacencyVec& adjVec = newAdjacency[vertexInd];
		const bool rowModVal = modVec[vertexInd];
		for (int adjInd = 0; adjInd < nVertices; ++adjInd) {

			const bool adjVal = adjVec[adjInd];
			const bool colModVal = modVec[adjInd];
			adjVec[adjInd] = reduceFn(reduceFn(adjVal, colModVal), rowModVal);

		}
	}
	return newAdjacency;
}
