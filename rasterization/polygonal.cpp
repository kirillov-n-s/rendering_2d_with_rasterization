#include <glm/common.hpp>
#include "polygonal.h"

namespace Rasterization {

std::vector<Triangle> modelToTriangles(
	const Coords2d& screenVertices,
	const IndexVec& triangleVertexIndices)
{
	const int nTriangles = triangleVertexIndices.size() / 3;
	std::vector<Triangle> triangles;
	triangles.reserve(nTriangles);
	for (int triangleInd = 0; triangleInd < nTriangles; ++triangleInd) {
		const int aInd = triangleVertexIndices[3 * triangleInd + 0];
		const int bInd = triangleVertexIndices[3 * triangleInd + 1];
		const int cInd = triangleVertexIndices[3 * triangleInd + 2];
		const PixelCoord2d a = glm::round(screenVertices[aInd]);
		const PixelCoord2d b = glm::round(screenVertices[bInd]);
		const PixelCoord2d c = glm::round(screenVertices[cInd]);
		triangles.push_back({ a, b, c });
	}
	return triangles;
}

void rasterizeTriangles(
	const Coords2d& screenVertices,
	const IndexVec& triangleVertexIndices,
	const Color color,
	Bitmap& bitmap)
{
	const std::vector<Triangle> triangles = modelToTriangles(
		screenVertices,
		triangleVertexIndices);
	for (const Triangle& triangle : triangles)
		scanlineTriangle(triangle, color, bitmap);
}
}
