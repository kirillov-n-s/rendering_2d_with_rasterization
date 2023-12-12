#include <glm/common.hpp>
#include "polygonal.h"

namespace Rasterization {

std::vector<Triangle> modelToTriangles(
	const Coords3d& screenVerticesWithDepth,
	const IndexVec& triangleVertexIndices)
{
	const int nTriangles = triangleVertexIndices.size() / 3;
	std::vector<Triangle> triangles;
	triangles.reserve(nTriangles);
	for (int triangleInd = 0; triangleInd < nTriangles; ++triangleInd) {
		const int aInd = triangleVertexIndices[3 * triangleInd + 0];
		const int bInd = triangleVertexIndices[3 * triangleInd + 1];
		const int cInd = triangleVertexIndices[3 * triangleInd + 2];
		const PixelCoord3d a = glm::round(screenVerticesWithDepth[aInd]);
		const PixelCoord3d b = glm::round(screenVerticesWithDepth[bInd]);
		const PixelCoord3d c = glm::round(screenVerticesWithDepth[cInd]);
		triangles.push_back({ a, b, c });
	}
	return triangles;
}

void rasterizeTrianglesWithZBuffer(
	const Coords3d& screenVerticesWithDepth,
	const IndexVec& triangleVertexIndices,
	const std::vector<Color> colorPerTriangle,
	Bitmap& bitmap,
	Bitmap& zBuffer)
{
	const std::vector<Triangle> triangles = modelToTriangles(
		screenVerticesWithDepth,
		triangleVertexIndices);
	const int nTriangles = triangles.size();
	for (int triangleInd = 0; triangleInd < nTriangles; ++triangleInd) {
		const Triangle& triangle = triangles[triangleInd];
		const Color color = colorPerTriangle[triangleInd];
		scanlineTriangle(triangle, color, bitmap, zBuffer);
	}
}

void zBufferToDepthMap(
	const Bitmap& zBuffer,
	Bitmap& depthMap)
{
	const auto maxDepthIt = std::max_element(
		zBuffer.data(),
		zBuffer.data() + zBuffer.physicalSize());
	const int maxDepth = *maxDepthIt;
	for (int y = 0; y < depthMap.height(); ++y) {
		for (int x = 0; x < depthMap.width(); ++x) {
			const int depthDenorm = colorToInt(zBuffer.getPixel(x, y));
			if (depthDenorm < 0) {
				depthMap.setPixel(x, y, colorBlack);
				continue;
			}
			const float depthNorm = float(depthDenorm) / maxDepth;
			const Color depthGrayscale = colorGray(byte(depthNorm * 255));
			depthMap.setPixel(x, y, depthGrayscale);
		}
	}
}
}
