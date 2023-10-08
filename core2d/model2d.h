#pragma once

#include "affine2d.h"

namespace Core2d {

class Model2d
{
public:
	Model2d(
		const HomogCoords2d &vertices,
		const AdjacencyMat &adjacency);

	HomogCoords2d vertices() const;
	AdjacencyMat adjacency() const;

	void applyTransform(const Affine2d::Mat &transform);

private:
	HomogCoords2d m_verticesInit;
	HomogCoords2d m_verticesCurrent;
	Affine2d::Mat m_transform = Affine2d::identity();

	AdjacencyMat m_adjacency;
};
}
