#include "affine2d.h"

namespace Core2d::Affine2d {

HomogCoords2d transformVertices(
	const Mat& transform,
	const HomogCoords2d& vertices)
{
	HomogCoords2d result = vertices;
	for (glm::vec3& vertex : result)
		vertex = transform * vertex;
	return result;
}

Mat identity()
{
	return Mat(1.0f);
}

Mat rotation(const float angle)
{
	const float sinA = std::sin(angle);
	const float cosA = std::cos(angle);
	return glm::transpose(Mat(
		cosA, -sinA, 0.0f,
		sinA, cosA, 0.0f,
		0.0f, 0.0f, 1.0f
	));
}

Mat translation(const Coord2d &param)
{
	return glm::transpose(Mat(
		1.0f, 0.0f, param.x,
		0.0f, 1.0f, param.y,
		0.0f, 0.0f, 1.0f
	));
}

Mat scale(const Coord2d &param)
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

Mat rotationAroundPoint(const Coord2d& point, const float angle)
{
	return translation(point) * rotation(angle) * translation(-point);
}

Mat scaleByAxes(
	const Coord2d& axesCenter,
	const float axesAngle,
	const Coord2d& param)
{
	const Mat& axesRotation = rotation(axesAngle);
	const Mat& invAxesRotation = glm::transpose(axesRotation);
	return translation(axesCenter) * axesRotation * scale(param) * invAxesRotation * translation(-axesCenter);
}
}
