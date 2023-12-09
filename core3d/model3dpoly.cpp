#include <numeric>
#include "model3dpoly.h"

namespace Core3d {

Model3dPoly::Model3dPoly(
	const HomogCoords3d& vertices,
	const IndexVec& triangleVertexIndices)
	: Model3dBase(vertices),
	m_triangleVertexIndices(triangleVertexIndices)
{}

IndexVec Model3dPoly::triangleVertexIndices() const
{
	return m_triangleVertexIndices;
}
}
