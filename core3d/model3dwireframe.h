#pragma once

#include "model3dbase.h"

namespace Core3d {

	class Model3dWireframe : public virtual Model3dBase
	{
	public:
		Model3dWireframe(
			const HomogCoords3d& vertices,
			const AdjacencyMat& adjacency);

		AdjacencyMat adjacency() const;

	private:
		AdjacencyMat m_adjacency;
	};
}
