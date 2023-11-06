#pragma once

#include "../common_types.h"

namespace Core2d {

class Camera2d
{
public:
	Camera2d(
		const int screenWidth, const int screenHeight,
		const float worldL, const float worldR,
		const float worldB, const float worldT);

	int screenWidth() const;
	int screenHeight() const;

	void move(const Coord2d &screenDelta);
	void zoom(const HomogCoord2d &worldFixedPoint, const float factor);
	void resizeKeepTopBottom(const int newScreenWidth, const int newScreenHeight);
	void reset();

	Coord2d worldToScreen(
		const HomogCoord2d& worldVertex) const;
	HomogCoord2d screenToWorld(
		const Coord2d& screenVertex) const;

	Coords2d worldToScreen(
		const HomogCoords2d &worldVertices) const;
	HomogCoords2d screenToWorld(
		const Coords2d& screenVertices) const;

private:
	struct WorldExtents
	{
		float worldL;
		float worldR;
		float worldB;
		float worldT;
	};

	float worldWidth() const;
	float worldHeight() const;

	int m_screenWidth = 0;
	int m_screenHeight = 0;

	WorldExtents m_extents;
	WorldExtents m_extentsInit;
};
}
