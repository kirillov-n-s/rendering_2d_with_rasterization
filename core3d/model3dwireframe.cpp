#include <numeric>
#include "model3dwireframe.h"

namespace Core3d {

Model3dWireframe::Model3dWireframe(
	const HomogCoords3d& vertices,
	const AdjacencyMat& adjacency)
	: Model3dBase(vertices),
	m_adjacency(adjacency)
{}

AdjacencyMat Model3dWireframe::adjacency() const
{
	return m_adjacency;
}
}
