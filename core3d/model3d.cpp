#include <numeric>
#include "model3d.h"

namespace Core3d {

Model3d::Model3d(
	const HomogCoords3d& vertices,
	const AdjacencyMat& adjacency)
	: m_verticesInit(vertices), m_verticesCurrent(vertices),
	m_adjacency(adjacency)
{}

HomogCoords3d Model3d::vertices() const
{
	return m_verticesCurrent;
}


AdjacencyMat Model3d::adjacency() const
{
	return m_adjacency;
}

void Model3d::applyTransform(const Affine3d::Mat& transform)
{
	m_transform = transform * m_transform;
	m_verticesCurrent = Affine3d::transformVertices(m_transform, m_verticesInit);
}

void Model3d::resetTransform()
{
	m_transform = Affine3d::identity();
	m_verticesCurrent = m_verticesInit;
}
}
