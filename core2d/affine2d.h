#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>

namespace Core2d::Affine2d {

using Mat = glm::mat3x3;

void transformVertices(
	const Mat &transform,
	std::vector<glm::vec3> &vertices);

Mat identity();

Mat rotationOrigin(const float angle);

Mat translation(const glm::vec2 &param);

Mat scale(const glm::vec2 &param);

Mat mirrorX();
Mat mirrorY();
Mat mirrorOrigin();
}
