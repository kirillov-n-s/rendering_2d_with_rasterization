#pragma once

#include "scanline.h"

namespace Rasterization {

std::vector<Triangle> modelToTriangles(
	const Coords3d& screenVerticesWithDepth,
	const IndexVec& triangleVertexIndices);

void rasterizeTrianglesWithZBuffer(
	const Coords3d& screenVerticesWithDepth,
	const IndexVec& triangleVertexIndices,
	const std::vector<Color> colorPerTriangle,
	Bitmap& bitmap,
	Bitmap& zBuffer);

void rasterizeDepthMap(
	const Coords3d& screenVerticesWithDepth,
	const IndexVec& triangleVertexIndices,
	Bitmap& bitmap,
	Bitmap& zBuffer);

void zBufferToDepthMap(
	const Bitmap& zBuffer,
	Bitmap& depthMap);
}
