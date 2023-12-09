#pragma once

#include "bresenham.h"

namespace Rasterization {

std::vector<Line> modelToLines(
	const Coords2d &screenVertices,
	const AdjacencyMat &adjacency);

void rasterizeWireframe(
	const Coords2d& screenVertices,
	const AdjacencyMat& adjacency,
	const Color color,
	Bitmap &bitmap);
}
