#pragma once

#include "bresenham.h"

namespace Rasterization {

std::vector<Line> modelToLines(
	const Coords2d &screenVertices,
	const AdjacencyMat &adjacency);

std::vector<Line> cullInvisible(
	const std::vector<Line>& lines,
	const int canvasWidth,
	const int canvasHeight);

void rasterizeModel(
	const Coords2d& screenVertices,
	const AdjacencyMat& adjacency,
	const Color color,
	Bitmap &bitmap);
}
