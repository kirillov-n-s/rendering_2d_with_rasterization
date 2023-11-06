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

Mat rotationAroundX(const float angle);
Mat rotationAroundY(const float angle);
Mat rotationAroundZ(const float angle);
Mat rotation(const Coord3d& yawPitchRoll);
Mat rotationAroundAxis(const Coord3d& axis, const float angle);

Mat translation(const Coord3d& param);

Mat scale(const Coord3d& param);

Mat scaleByAxes(
	const Coord3d& axesCenter,
	const float axesAngle,
	const Coord3d& param);
}
}
