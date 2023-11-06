#pragma once

#include <glm/mat3x3.hpp>
#include "../common_types.h"

namespace Core2d {
namespace Affine2d {

using Mat = glm::mat3x3;

HomogCoords2d transformVertices(
	const Mat& transform,
	const HomogCoords2d& vertices);

Mat identity();

Mat rotation(const float angle);

Mat translation(const Coord2d& param);

Mat scale(const Coord2d& param);

Mat mirrorX();
Mat mirrorY();
Mat mirrorOrigin();

Mat rotationAroundPoint(const Coord2d& point, const float angle);

Mat scaleByAxes(
	const Coord2d& axesCenter,
	const float axesAngle,
	const Coord2d& param);
}
}
