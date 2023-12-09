#pragma once

#include "affine3d.h"

namespace Core3d {

	class Model3dBase
	{
	public:
		Model3dBase(const HomogCoords3d& vertices);

		HomogCoords3d vertices() const;

		void applyTransform(const Affine3d::Mat& transform);
		void resetTransform();

	private:
		HomogCoords3d m_verticesInit;
		HomogCoords3d m_verticesCurrent;
		Affine3d::Mat m_transform = Affine3d::identity();
	};
}
