#include <glm/common.hpp>
#include "wireframe.h"

namespace Rasterization {

std::vector<Line> modelToLines(
	const Coords2d& screenVertices,
	const AdjacencyMat& adjacency)
{
	const int nVertices = adjacency.size();
	std::vector<Line> lines;
	for (int vertexInd = 0; vertexInd < nVertices; ++vertexInd) {
		const Coord2d& vertex = screenVertices[vertexInd];
		const AdjacencyVec& vertexAdjacency = adjacency[vertexInd];
		for (int adjInd = vertexInd + 1; adjInd < nVertices; ++adjInd) {
			if (vertexAdjacency[adjInd]) {
				const Coord2d& adjVertex = screenVertices[adjInd];
				const PixelCoord2d first = glm::round(vertex);
				const PixelCoord2d second = glm::round(adjVertex);
				lines.emplace_back(first, second);
			}
		}
	}
	return lines;
}

void rasterizeWireframe(
	const Coords2d& screenVertices,
	const AdjacencyMat& adjacency,
	const Color color,
	Bitmap& bitmap)
{
	const std::vector<Line> lines = modelToLines(
		screenVertices,
		adjacency);
	for (const Line& line : lines)
		bresenhamLine(line, color, bitmap);
}
}
