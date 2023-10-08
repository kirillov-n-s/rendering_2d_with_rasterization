#include "affine2d.h"

namespace Core2d::Affine2d {

void transformVertices(
	const Mat& transform,
	std::vector<glm::vec3>& vertices)
{
	for (glm::vec3& vertex : vertices)
		vertex = transform * vertex;
}

Mat identity()
{
	return Mat(1.0f);
}

Mat rotationOrigin(const float angle)
{
	const float sinA = std::sin(angle);
	const float cosA = std::cos(angle);
	return Mat(
		cosA, -sinA, 0.0f,
		sinA, cosA, 0.0f,
		0.0f, 0.0f, 1.0f
	);
}

Mat translation(const glm::vec2 &param)
{
	return Mat(
		1.0f, 0.0f, param.x,
		0.0f, 1.0f, param.y,
		0.0f, 0.0f, 1.0f
	);
}

Mat scale(const glm::vec2 &param)
{
	return Mat(
		param.x, 0.0f, 0.0f,
		0.0f, param.y, 0.0f,
		0.0f, 0.0f, 1.0f
	);
}

Mat mirrorX()
{
	return Mat(
		1.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	);
}

Mat mirrorY()
{
	return Mat(
		-1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	);
}

Mat mirrorOrigin()
{
	return Mat(
		-1.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	);
}
}
