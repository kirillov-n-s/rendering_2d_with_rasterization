#include "model2d.h"

namespace Core2d {

Model2d::Model2d(
	const std::vector<glm::vec3> &vertices,
	const std::vector<std::vector<char>> &adjacency)
	: m_verticesInit(vertices), m_verticesCurrent(vertices),
	  m_adjacency(adjacency)
{}

std::vector<glm::vec3> Model2d::vertices() const
{
	return m_verticesCurrent;
}


std::vector<std::vector<char>> Model2d::adjacency() const
{
	return m_adjacency;
}

void Model2d::applyTransform(const Affine2d::Mat &transform)
{
	Affine2d::transformVertices(transform, m_verticesCurrent);
	m_transform = transform * m_transform;
}
}
