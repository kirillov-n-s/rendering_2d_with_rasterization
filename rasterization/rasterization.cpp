#include <glm/common.hpp>
#include <glm/vector_relational.hpp>
#include "rasterization.h"

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

void rasterizeModel(
	const Coords2d& screenVertices,
	const AdjacencyMat& adjacency,
	const Color color,
	Bitmap& bitmap)
{
	const std::vector<Line> lines = modelToLines(
		screenVertices,
		adjacency);
	const PixelCoord2d zero(0, 0);
	const PixelCoord2d bitmapExtents(bitmap.width(), bitmap.height());
	for (const Line& line : lines) {

		/*const PixelCoord2d start = line.first;
		const PixelCoord2d end = line.second;
		const bool outOfBounds = glm::any(
			(glm::lessThan(start, zero) || glm::lessThan(end, zero))
			||
			(glm::greaterThanEqual(start, bitmapExtents) || glm::greaterThanEqual(end, bitmapExtents)));
		if (outOfBounds)
			continue;*/
		bresenhamLine(line, color, bitmap);
	}
}
}
