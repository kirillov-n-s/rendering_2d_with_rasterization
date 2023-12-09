#include <numeric>
#include "model3dwireandpoly.h"

namespace Core3d {

Model3dWireAndPoly::Model3dWireAndPoly(
	const HomogCoords3d& vertices,
	const AdjacencyMat& adjacency,
	const IndexVec& triangleVertexIndices)
	: Model3dBase(vertices),
	  Model3dWireframe(vertices, adjacency),
	  Model3dPoly(vertices, triangleVertexIndices)
{}
}
