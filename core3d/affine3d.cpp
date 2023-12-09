#include "affine3d.h"

namespace Core3d {
namespace Affine3d {

HomogCoords3d transformVertices(
	const Mat& transform,
	const HomogCoords3d& vertices)
{
	HomogCoords3d result = vertices;
	for (HomogCoord3d& vertex : result)
		vertex = transform * vertex;
	return result;
}

Mat identity()
{
	return Mat(1.0f);
}

Mat rotationX(const float angle)
{
	const float cos = std::cos(angle);
	const float sin = std::sin(angle);
	Mat mat = identity();
	mat[1][1] = cos;
	mat[1][2] = -sin;
	mat[2][1] = sin;
	mat[2][2] = cos;
	return mat;
}

Mat rotationY(const float angle)
{
	const float cos = std::cos(angle);
	const float sin = std::sin(angle);
	Mat mat = identity();
	mat[0][0] = cos;
	mat[0][2] = -sin;
	mat[2][0] = sin;
	mat[2][2] = cos;
	return mat;
}

Mat rotationZ(const float angle)
{
	const float cos = std::cos(angle);
	const float sin = std::sin(angle);
	Mat mat = identity();
	mat[0][0] = cos;
	mat[0][1] = -sin;
	mat[1][0] = sin;
	mat[1][1] = cos;
	return mat;
}

Mat rotation(const Coord3d& yawPitchRoll)
{
	const float yaw = yawPitchRoll[0];
	const float pitch = yawPitchRoll[1];
	const float roll = yawPitchRoll[2];
	return rotationZ(yaw) * rotationY(pitch) * rotationX(roll);
}

Mat translation(const Coord3d& param)
{
	Mat mat = identity();
	mat[3] = homog3d(param);
	return mat;
}

Mat scale(const Coord3d& param)
{
	Mat mat = identity();
	mat[0][0] = param[0];
	mat[1][1] = param[1];
	mat[2][2] = param[2];
	return mat;
}

Mat rotationWithPivot(
	const Coord3d& pivotPosition,
	const Coord3d& pivotYawPitchRoll,
	const Coord3d& yawPitchRoll)
{
	const Mat& pivotRotation = rotation(pivotYawPitchRoll);
	const Mat& invPivotRotation = glm::transpose(pivotRotation);
	return translation(pivotPosition)
		* pivotRotation
		* rotation(yawPitchRoll)
		* invPivotRotation
		* translation(-pivotPosition);
}

Mat scaleWithPivot(
	const Coord3d& pivotPosition,
	const Coord3d& pivotYawPitchRoll,
	const Coord3d& param)
{
	const Mat& pivotRotation = rotation(pivotYawPitchRoll);
	const Mat& invPivotRotation = glm::transpose(pivotRotation);
	return translation(pivotPosition)
		* pivotRotation
		* scale(param)
		* invPivotRotation
		* translation(-pivotPosition);
}
}
}
