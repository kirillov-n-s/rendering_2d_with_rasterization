#pragma once

#include <string>
#include "../common_types.h"

namespace IO {

std::string readObjVerticesFaces(
	const std::string& path,
	HomogCoords3d& outVertices,
	IndexVec& outPolygonVertexIndices,
	IndexVec &outPolygonStarts);

IndexVec triangulateFaces(
	const IndexVec& polygonVertexIndices,
	const IndexVec& polygonStarts);

AdjacencyMat facesToAdjacency(
	const IndexVec& triangleVertexIndices,
	const int nVertices);

std::string verticesAndAdjacencyFromObj(
	const std::string& path,
	HomogCoords3d& outVertices,
	AdjacencyMat& outAdjacency);
}

