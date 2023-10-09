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
	void resetTransform();

private:
	HomogCoords2d m_verticesInit;
	HomogCoords2d m_verticesCurrent;
	Affine2d::Mat m_transform = Affine2d::identity();

	AdjacencyMat m_adjacency;
};

AdjacencyMat makePolylineAdjacency(const int nVertices);

template <typename CoordType>
int closestPointInd(
	const CoordType &closestTo,
	const std::vector<CoordType> &closestFrom);
}









// IMPLEMENTATION
namespace Core2d {

template <typename CoordType>
int closestPointInd(
	const CoordType& closestTo,
	const std::vector<CoordType>& closestFrom)
{
	const int nPoints = closestFrom.size();
	int currentClosestInd = 0;
	auto currentDistance = glm::distance(closestFrom[0], closestTo);
	for (int pointInd = 1; pointInd < nPoints; ++pointInd) {
		const CoordType& candidate = closestFrom[pointInd];
		const auto candidateDistance = glm::distance(candidate, closestTo);
		if (candidateDistance < currentDistance) {
			currentDistance = candidateDistance;
			currentClosestInd - pointInd;
		}
	}
	return currentClosestInd;
}
}
