#include <numeric>
#include "model2d.h"

namespace Core2d {

Model2d::Model2d(
	const HomogCoords2d &vertices,
	const AdjacencyMat &adjacency)
	: m_verticesInit(vertices), m_verticesCurrent(vertices),
	  m_adjacency(adjacency)
{}

HomogCoords2d Model2d::vertices() const
{
	return m_verticesCurrent;
}


AdjacencyMat Model2d::adjacency() const
{
	return m_adjacency;
}

void Model2d::applyTransform(const Affine2d::Mat &transform)
{
	m_transform = transform * m_transform;
	m_verticesCurrent = Affine2d::transformVertices(m_transform, m_verticesInit);
}

void Model2d::resetTransform()
{
	m_transform = Affine2d::identity();
	m_verticesCurrent = m_verticesInit;
}

AdjacencyMat makePolylineAdjacency(const int nVertices)
{
	if (nVertices < 2)
		return {{false}};
	AdjacencyMat adjacency;
	adjacency.reserve(nVertices);
	adjacency.emplace_back(nVertices);
	for (int vertexInd = 1; vertexInd < nVertices; ++vertexInd) {
		adjacency.emplace_back(nVertices);
		adjacency[vertexInd - 1][vertexInd] = true;
		adjacency[vertexInd][vertexInd - 1] = true;
	}
	return adjacency;
}

HomogCoords2d makeVerticesFromFunction(
	const std::function<float(float)>& function,
	const float lowerBound, const float upperBound, const float stepSize,
	const float scale)
{
	HomogCoords2d vertices;
	vertices.reserve(size_t((upperBound - lowerBound) / stepSize));
	for (float x = lowerBound; x < upperBound; x += stepSize) {
		const float y = function(x);
		vertices.emplace_back(x * scale, y * scale, 1.0f);
	}
	return vertices;
}
}
