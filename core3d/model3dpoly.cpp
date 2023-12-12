#include <numeric>
#include <glm/geometric.hpp>
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

Coords3d Model3dPoly::triangleNormals() const
{
	const HomogCoords3d &vertices = this->vertices();
	const int nTriangles = m_triangleVertexIndices.size() / 3;
	Coords3d normals;
	normals.reserve(nTriangles);
	for (int triangleInd = 0; triangleInd < nTriangles; ++triangleInd) {
		const int aInd = m_triangleVertexIndices[3 * triangleInd + 0];
		const int bInd = m_triangleVertexIndices[3 * triangleInd + 1];
		const int cInd = m_triangleVertexIndices[3 * triangleInd + 2];
		const Coord3d a = vertices[aInd];
		const Coord3d b = vertices[bInd];
		const Coord3d c = vertices[cInd];
		const Coord3d normal = glm::normalize(glm::cross(b - a, c - a));
		normals.push_back(normal);
	}
	return normals;
}
}
