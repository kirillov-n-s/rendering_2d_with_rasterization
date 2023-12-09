#pragma once

#include "model3dbase.h"

namespace Core3d {

	class Model3dPoly : public virtual Model3dBase
	{
	public:
		Model3dPoly(
			const HomogCoords3d& vertices,
			const IndexVec& triangleVertexIndices);

		IndexVec triangleVertexIndices() const;

	private:
		IndexVec m_triangleVertexIndices;
	};
}
