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
	m_verticesCurrent = Affine2d::transformVertices(transform, m_verticesInit);
}
}
