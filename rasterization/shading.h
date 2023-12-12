#pragma once

#include "../common_types.h"

namespace Rasterization {

void flatShading(
	const Coords3d& faceNormals,
	const Coord3d& lightDirection,
	ColorVec& inOutFaceColors);
}
