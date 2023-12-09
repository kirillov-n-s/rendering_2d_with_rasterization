#include <numeric>
#include "model3dbase.h"

namespace Core3d {

Model3dBase::Model3dBase(const HomogCoords3d& vertices)
	: m_verticesInit(vertices), m_verticesCurrent(vertices)
{}

HomogCoords3d Model3dBase::vertices() const
{
	return m_verticesCurrent;
}

void Model3dBase::applyTransform(const Affine3d::Mat& transform)
{
	m_transform = transform * m_transform;
	m_verticesCurrent = Affine3d::transformVertices(m_transform, m_verticesInit);
}

void Model3dBase::resetTransform()
{
	m_transform = Affine3d::identity();
	m_verticesCurrent = m_verticesInit;
}
}
