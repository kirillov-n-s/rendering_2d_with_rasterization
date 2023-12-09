#pragma once

#include "scanline.h"

namespace Rasterization {

std::vector<Triangle> modelToTriangles(
	const Coords2d& screenVertices,
	const IndexVec& triangleVertexIndices);

void rasterizeTriangles(
	const Coords2d& screenVertices,
	const IndexVec& triangleVertexIndices,
	const Color color,
	Bitmap& bitmap);
}
