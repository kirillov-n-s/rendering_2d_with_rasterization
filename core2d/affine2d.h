#pragma once

#include <glm/mat3x3.hpp>
#include "../common_types.h"

namespace Core2d::Affine2d {

using Mat = glm::mat3x3;

HomogCoords2d transformVertices(
	const Mat &transform,
	const HomogCoords2d &vertices);

Mat identity();

Mat rotationOrigin(const float angle);

Mat translation(const Coord2d &param);

Mat scale(const Coord2d &param);

Mat mirrorX();
Mat mirrorY();
Mat mirrorOrigin();
}
