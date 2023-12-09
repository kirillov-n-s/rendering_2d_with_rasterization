#pragma once

#include <glm/mat4x4.hpp>
#include "../common_types.h"

namespace Core3d {
namespace Affine3d {

using Mat = glm::mat4x4;

HomogCoords3d transformVertices(
	const Mat& transform,
	const HomogCoords3d& vertices);

Mat identity();

Mat rotationX(const float angle);
Mat rotationY(const float angle);
Mat rotationZ(const float angle);
Mat rotation(const Coord3d& yawPitchRoll);

Mat translation(const Coord3d& param);

Mat scale(const Coord3d& param);

Mat rotationWithPivot(
	const Coord3d& pivotPosition,
	const Coord3d& pivotYawPitchRoll,
	const Coord3d& yawPitchRoll);

Mat scaleWithPivot(
	const Coord3d& pivotPosition,
	const Coord3d& pivotYawPitchRoll,
	const Coord3d& param);
}
}
