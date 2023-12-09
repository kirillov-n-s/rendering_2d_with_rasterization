#pragma once

#include "model3dwireframe.h"
#include "model3dpoly.h"

namespace Core3d {

	class Model3dWireAndPoly : public Model3dWireframe, public Model3dPoly
	{
	public:
		Model3dWireAndPoly(
			const HomogCoords3d& vertices,
			const AdjacencyMat& adjacency,
			const IndexVec &triangleVertexIndices);
	};
}
