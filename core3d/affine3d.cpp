#include "affine3d.h"

namespace Core3d::Affine3d {

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

Mat scale(const Coord3d& param)
{
	Mat mat = identity();
	mat[0][0] = param[0];
	mat[1][1] = param[1];
	mat[2][2] = param[2];
	return mat;
}
}
